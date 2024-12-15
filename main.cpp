#include <inttypes.h>
#include <stdio.h>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include <chrono>
#include "WAVM/IR/Module.h"
#include "WAVM/IR/Types.h"
#include "WAVM/IR/Value.h"
#include "WAVM/Runtime/Intrinsics.h"
#include "WAVM/Runtime/Runtime.h"
#include "WAVM/WASTParse/WASTParse.h"
#include "WAVM/WASTPrint/WASTPrint.h"
#include "WAVM/WASM/WASM.h"

#include "leb128.h"
#include "simpleGas.h"

using namespace WAVM;
using namespace WAVM::IR;
using namespace WAVM::Runtime;

Memory* memory = nullptr;
int maxGas = 5000;
int availableGas = maxGas;

class OutOfGasException : public std::exception {};

WAVM_DEFINE_INTRINSIC_MODULE(embedder)
WAVM_DEFINE_INTRINSIC_FUNCTION(embedder, "useGas", void, useGas, U32 gas)
{
  //printf("USE GAS CALLED %d!\n", gas);
  availableGas -= gas;
  //printf("GAS LEFT %d!\n", availableGas);
  if(availableGas < 0) {
    throw OutOfGasException();
  }
}

WAVM_DEFINE_INTRINSIC_FUNCTION(embedder, "useGas2", U32, useGas2, U32 mul, U32 gas, U32 div)
{
  //printf("USE GAS 2 CALLED (%d x %d) / %d = %d!\n", mul, gas, div, (gas * mul) / div);
  if(mul >= (INT32_MAX - availableGas) / gas) {
    throw OutOfGasException();
  }
  availableGas -= (gas * mul) / div;
  //printf("GAS LEFT %d!\n", availableGas);
  if(availableGas < 0) {
    throw OutOfGasException();
  }
  //printf("RETURNING %d!\n", mul);
  return mul;
}

int main(int argc, char** argv)
{
  IR::Module irModule;
  std::vector<WAST::Error> wastErrors;
  printf("loading module %s\n", argv[1]);

  maxGas = argc > 2 ? atoi(argv[2]) : 2000;

  printf("max gas %d\n", maxGas);

  std::ifstream inFile(argv[1], std::ios::binary | std::ios::ate);
  size_t inDataSize = inFile.tellg();
  inFile.seekg(0, std::ios::beg);
  uint8_t *inData = (uint8_t*)malloc(inDataSize);
  inFile.read((char*)inData, inDataSize);

  printf("Adding gas metering! binary size %d\n", inDataSize);

  uint8_t* out;
  size_t outSize;
  int res = addGasMetering(inData, inDataSize, &out, &outSize,
                           "embedder", "useGas", "useGas2");
  if(res != 0) {
    printf("Error adding gas metering: %d", res);
    return 1;
  }

  std::fstream file2("gas.wasm", std::ios::out | std::ios::binary);
  file2.write((char*)out, outSize);
  file2.close();

  WASM::loadBinaryModule((const U8*)out, outSize, irModule);
  free(out);

  printf("Loaded module!\n");

  const FunctionType i32_to_void({}, {ValueType::i32});
  const FunctionType i32_to_i32({ValueType::i32}, {ValueType::i32});
  const FunctionType i32_i32_i32_to_i32({ValueType::i32},
                                        {ValueType::i32, ValueType::i32, ValueType::i32});

  /*std::string wast = WAST::print(irModule);
  printf("WAST: %s\n", wast.c_str());*/

  printf("Compiling module!\n");
  ModuleRef module = compileModule(irModule);

  printf("Compiled module!\n");
  // Create a WAVM compartment and context.
  GCPointer<Compartment> compartment = createCompartment();
  GCPointer<Context> context = createContext(compartment);

  // Create an instance that encapsulates the intrinsic function in a way that allows it to be
  // imported by WASM instances.
  Instance* intrinsicsInstance = WAVM::Intrinsics::instantiateModule(
      compartment, {WAVM_INTRINSIC_MODULE_REF(embedder)}, "embedder");
  Function* gasFunction
      = getTypedInstanceExport(intrinsicsInstance, "useGas", i32_to_void);
  Function* gas2Function
      = getTypedInstanceExport(intrinsicsInstance, "useGas2", i32_i32_i32_to_i32);

  catchRuntimeExceptions(
      [&]() {

        for(int i = 0; i < 23; i++) {
          auto initTime = std::chrono::high_resolution_clock::now();
          GCPointer<Memory> memory = createMemory(compartment,
                                                  MemoryType(false, IndexType::i32,
                                                             SizeConstraints{1, 40}),
                                                  "memory");
          GCPointer<Instance> instance = instantiateModule(compartment, module,
                                  {asObject(memory), asObject(gasFunction), asObject(gas2Function)},
                                  "hello"/*, resourceQuota*/);
          // Extract exports.
          Function* runFunction = getTypedInstanceExport(instance, "run", i32_to_i32);
          WAVM_ASSERT(runFunction);

          auto startTime = std::chrono::high_resolution_clock::now();

          //printf("Calling run(%d) ", i);
          availableGas = maxGas;
          UntaggedValue runArgs[1]{i};
          UntaggedValue runResults[1];
          std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
          try {
            invokeFunction(context, runFunction, i32_to_i32, runArgs, runResults);
            endTime = std::chrono::high_resolution_clock::now();
            printf("=> %u   GAS used: %d; left: %d\n", runResults[0].u32, maxGas - availableGas, availableGas);
          } catch(const OutOfGasException& e) {
            endTime = std::chrono::high_resolution_clock::now();
            printf(":: Out of gas!\n");
          }
          printf("Prepare: %dus\n",
                 std::chrono::duration_cast<std::chrono::microseconds>(startTime - initTime).count());
          printf("Run: %dus\n",
                 std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count());

        }
      },
      [&](Exception* exception) {
        // Treat any unhandled exception as a fatal error.
        Errors::fatalf("Runtime exception: %s", describeException(exception).c_str());
        destroyException(exception);
      });

  // Clean up the WAVM runtime objects.
  context = nullptr;
  WAVM_ERROR_UNLESS(tryCollectCompartment(std::move(compartment)));

  return 0;
}
