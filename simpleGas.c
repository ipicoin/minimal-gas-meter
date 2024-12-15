#include <string.h>
#include "simpleGas.h"
#include "leb128.h"
#include "opcodes.h"
#include <malloc.h>
#include <stdint.h>

void grow(uint32_t to, uint8_t** buf, uint8_t** end, uint8_t** p) {
  uint32_t oldSize = *end - *buf;
  uint32_t at = *p - *buf;
  uint32_t newSize = oldSize;
  while(newSize < to) {
    newSize *= 2;
  }
  if(newSize == oldSize) return;
  uint8_t* newBuf = (uint8_t*)malloc(newSize);
  memcpy(newBuf, *buf, at);
  free(*buf);
  *buf = newBuf;
  *end = *buf + newSize;
  *p = *buf + at;
}

void ensureSpace(uint32_t size, uint8_t** buf, uint8_t** end, uint8_t** p) {
  if(*p + size > *end) {
    grow(p + size - buf, buf, end, p);
  }
}

uint32_t appendVectorSection(
    uint8_t** p, uint8_t** end, uint32_t sectionSize, uint8_t** out, uint8_t** o, uint8_t** outEnd,
  uint8_t* data, uint32_t dataLen, uint8_t elementCount) {
  uint32_t Id;
  uint32_t count;
  uint32_t oldVecSizeLen = leb128ReadU32(p, *end, &count);
  Id = count;
  uint32_t newVecSizeLen = leb128SizeU32(count + elementCount);
  uint32_t newSectionSize = sectionSize - oldVecSizeLen + newVecSizeLen + dataLen;
  ensureSpace(newSectionSize, out, outEnd, o);
  leb128WriteU32(o, newSectionSize);
  leb128WriteU32(o, count + elementCount);
  memcpy(*o, *p, sectionSize - oldVecSizeLen); // copy existing types
  *o += sectionSize - oldVecSizeLen;
  *p += sectionSize - oldVecSizeLen;
  memcpy(*o, data, dataLen);
  *o += dataLen;
  return Id;
}

int meterCode(uint8_t** pp, uint8_t* end, uint8_t** coutp, uint8_t** co, uint8_t** coutEndp,
              uint32_t gasTypeId, uint32_t gasImportId) {
  uint8_t* p = *pp;
  uint8_t* out = *coutp;
  uint8_t* o = *co;
  uint8_t* outEnd = *coutEndp;

  uint32_t localEntriesCount;
  leb128ReadU32(&p, end, &localEntriesCount);
  leb128WriteU32(&o, localEntriesCount);
  int allLocalCount = 0;
  for (int i = 0; i < localEntriesCount; i++) {
    ensureSpace(5, &out, &outEnd, &o);
    uint32_t localCount;
    leb128ReadU32(&p, end, &localCount);
    leb128WriteU32(&o, localCount);
    uint8_t localType = *p++;
    *o++ = localType;
    allLocalCount += localCount;
  }

  #define readOpCode(code, bytes, uvecs, u32s, u64s) \
    for(int i = 0; i < uvecs; i++) {                        \
      uint32_t vectorSize;                                  \
      leb128ReadU32(&p, end, &vectorSize);                  \
      for(int e = 0; e < vectorSize; e++) {                 \
        uint32_t element;                                   \
        leb128ReadU32(&p, end, &element);                   \
      }                                                     \
    }                                                       \
    for(int i = 0; i < u32s; i++) {                         \
      uint32_t param;                                       \
      leb128ReadU32(&p, end, &param);                       \
    }                                                       \
    for(int i = 0; i < u64s; i++) {                         \
      uint64_t param;                                       \
      leb128ReadU64(&p, end, &param);                       \
    }                                                       \
    for(int i = 0; i < bytes; i++) {                        \
      p++;                                                  \
    }

  #define copyOpCode \
    ensureSpace(p - opcodeStart, &out, &outEnd, &o); \
    memcpy(o, opcodeStart, p - opcodeStart); \
    o += end - p;

  while(p < end) {
    int complexMetering = 0;
    int branchCost = 0;
    uint8_t* opcodeStart;
    uint8_t* branchStart = p;
    while(p < end) {
      opcodeStart = p;
      uint16_t opcode = *p++;

      switch (opcode) {
        #define visitOp(code, name, gas, bytes, uvecs, u32s, u64s) \
          case code: {                                             \
            uint32_t extension;                                    \
            leb128ReadU32(&p, end, &extension);                    \
            opcode = opcode << 8 | extension;                     \
          } break;
        ENUMERATE_EXTENSION_OPCODES(visitOp)
        #undef visitOp
      }

      int branch = 0;

      //printf("opcode %x\n", opcode);
      switch(opcode) {
        #define visitOp(code, name, gas, bytes, uvecs, u32s, u64s ) \
          case code: {                                              \
            readOpCode(code, bytes, uvecs, u32s, u64s)              \
            if(gas < 0) {                                           \
              branchCost += -gas;                                   \
              complexMetering = code;                               \
              break;                                                \
            }                                                       \
            branchCost +=  gas;                                     \
          } break;

        ENUMERATE_NON_BRANCHING_OPCODES(visitOp)
        #undef visitOp

        #define visitOp(code, name, gas, bytes, uvecs, u32s, u64s ) \
          case code: {                                              \
            readOpCode(code, bytes, uvecs, u32s, u64s)              \
            branchCost +=  gas;                                     \
            branch = 1;                                          \
          } break;

        ENUMERATE_CONTROL_OPCODES(visitOp)
        #undef visitOp


        default: {
          printf("error: unknown opcode %x\n", opcode);
          return 1;
        } break;
      }
      if(branch || complexMetering) break;
    }
    if(complexMetering) {
      ensureSpace(p - branchStart + 1 + leb128SizeU32(branchCost) + 1 + leb128SizeU32(gasImportId),
                  &out, &outEnd, &o);
      *o++ = 0x41; // i32.const
      leb128WriteS32(&o, branchCost);
      *o++ = 0x10; // i32.call
      leb128WriteU32(&o, gasImportId);
      memcpy(o, branchStart, opcodeStart - branchStart);
      o += opcodeStart - branchStart;

      #define METER_GAS_MULTIPLIED(gasPerElement, gasDivider)  \
        ensureSpace(128                            \
          + 1 + leb128SizeU32(gasPerElement)       \
          + 1 + leb128SizeU32(gasImportId)         \
          ,&out, &outEnd, &o);                     \
        *o++ = 0x41;                               \
        leb128WriteS32(&o, gasPerElement);         \
        *o++ = 0x41;                               \
        leb128WriteS32(&o, gasDivider);            \
        *o++ = 0x10;                               \
        leb128WriteU32(&o, gasImportId + 1);

      #define METER_GAS_CASE(code, name) \
        case code: { METER_GAS_MULTIPLIED(name##_GAS_FACTOR, name##_GAS_DIVIDER)} break;
      switch(complexMetering) {
        METER_GAS_CASE(0x40, MEMORY_GROW)
        METER_GAS_CASE(0xFC0B, MEMORY_FILL)
        METER_GAS_CASE(0xFC0A, MEMORY_COPY)
        METER_GAS_CASE(0xFC08, MEMORY_INIT)
        METER_GAS_CASE(0xFC0C, TABLE_INIT)
        METER_GAS_CASE(0xFC0E, TABLE_COPY)
        METER_GAS_CASE(0xFC0F, TABLE_GROW)
        METER_GAS_CASE(0xFC11, TABLE_FILL)

        default:
          printf("complex metering of opcode %x not implemented\n", complexMetering);
          return 1;
      }
      ensureSpace(p - opcodeStart, &out, &outEnd, &o);
      memcpy(o, opcodeStart, p - opcodeStart);
      o += p - opcodeStart;
    } else if(branchCost > 0) {
      ensureSpace(p - branchStart + 1 + leb128SizeU32(branchCost) + 1 + leb128SizeU32(gasImportId),
                  &out, &outEnd, &o);
      *o++ = 0x41; // i32.const
      leb128WriteS32(&o, branchCost);
      *o++ = 0x10; // i32.call
      leb128WriteU32(&o, gasImportId);
      memcpy(o, branchStart, p - branchStart);
      o += p - branchStart;
    } else {
      ensureSpace(p - branchStart, &out, &outEnd, &o);
      memcpy(o, branchStart, p - branchStart);
      o += p - branchStart;
    }
  }

  #undef readOpCode
  #undef copyOpCode
/*
  ensureSpace(end - p, &out, &outEnd, &o);
  memcpy(o, p, end - p);
  o += end - p;*/

  *coutp = out;
  *co = o;
  *coutEndp = outEnd;

  return 0;
}

/// https://github.com/ewasm/wasm-metering/blob/master/index.js

int addGasMetering(uint8_t* binary, uint32_t binarySize, uint8_t** outp, size_t* outSize,
                   const char* gasImportModule, const char* gasImportName, const char* gasImport2Name) {
  uint8_t* p = binary;
  uint8_t* end = binary + binarySize;

  uint8_t* out = (uint8_t*) malloc(1024);
  uint8_t* outEnd = out + 1024;
  uint8_t* o = out;

  memcpy(o, p, 8); p += 8; o += 8; /// copy header

  uint32_t gasTypeId;
  char typeAdded = 0;
  uint8_t gasTypes[] = {0x60, 0x01, 0x7f, 0x00, 0x60, 0x03, 0x7f, 0x7f, 0x7f, 0x01, 0x7f};

  uint32_t gasImportId;
  char importAdded = 0;
  uint32_t gasImportNameLen = strlen(gasImportName);
  uint32_t gasImport2NameLen = strlen(gasImport2Name);
  uint32_t gasImportModuleLen = strlen(gasImportModule);
  uint32_t gasImportSize;
  uint8_t* gasImport;

  while(p < end) {
    uint8_t section = *p;
    if(section > 1 && !typeAdded) {
      printf("No type section found! Adding!\n");
      ensureSpace(8, &out, &outEnd, &o);
      *o++ = 1; // type section
      leb128WriteU32(&o, sizeof(gasTypes) + 1);
      *o++ = 2; // 2 types
      memcpy(o, gasTypes, sizeof(gasTypes));
      gasTypeId = 0;
      typeAdded = 1;
    }
    if(section > 1) {
      /// Generate gas imports
      gasImportSize =  leb128SizeU32(gasImportModuleLen) + gasImportModuleLen +
                                leb128SizeU32(gasImportNameLen) + gasImportNameLen +
                                1 + leb128SizeU32(gasTypeId) +
                                leb128SizeU32(gasImportModuleLen) + gasImportModuleLen +
                                leb128SizeU32(gasImport2NameLen) + gasImport2NameLen +
                                1 + leb128SizeU32(gasTypeId + 1);
      gasImport = (uint8_t*)malloc(gasImportSize);
      uint8_t* gasImportP = gasImport;

      leb128WriteU32(&gasImportP, gasImportModuleLen); // import module length
      memcpy(gasImportP, gasImportModule, gasImportModuleLen); gasImportP += gasImportModuleLen;
      leb128WriteU32(&gasImportP, gasImportNameLen); // import name length
      memcpy(gasImportP, gasImportName, gasImportNameLen); gasImportP += gasImportNameLen;
      *gasImportP++ = 0; // import kind
      leb128WriteU32(&gasImportP, gasTypeId); // import type Id

      leb128WriteU32(&gasImportP, gasImportModuleLen); // import module length
      memcpy(gasImportP, gasImportModule, gasImportModuleLen); gasImportP += gasImportModuleLen;
      leb128WriteU32(&gasImportP, gasImport2NameLen); // import name length
      memcpy(gasImportP, gasImport2Name, gasImport2NameLen); gasImportP += gasImport2NameLen;
      *gasImportP++ = 0; // import kind
      leb128WriteU32(&gasImportP, gasTypeId + 1); // import type Id
    }
    if(section > 2 && !importAdded) {
      ensureSpace(8, &out, &outEnd, &o);
      *o++ = 2; // import section
      leb128WriteU32(&o, gasImportSize + 1);
      *o++ = 2; // 2 imports
      memcpy(o, gasImport, gasImportSize);
      o += gasImportSize;
      gasImportId = 0;
      importAdded = 1;
    }
    p++;
    *o++ = section;
    uint32_t sectionSize;
    leb128ReadU32(&p, end, &sectionSize);
    uint8_t* sectionEnd = p + sectionSize;
    switch(section) {
      case 1 : if(typeAdded) break; { // type section
        gasTypeId = appendVectorSection(&p, &end, sectionSize, &out, &o, &outEnd,
                                         gasTypes, sizeof(gasTypes), 2);
        typeAdded = 1;
      } break;
      case 2: if(importAdded) break; {
        uint8_t* sp = p;
        uint32_t count;
        leb128ReadU32(&p, end, &count);
        uint32_t functionCount = 0;
        for(uint32_t i = 0; i < count; ++i) {
          uint32_t moduleLen;
          leb128ReadU32(&p, end, &moduleLen);
          uint8_t* module = p;
          p += moduleLen;
          uint32_t nameLen;
          leb128ReadU32(&p, end, &nameLen);
          uint8_t* name = p;
          p += nameLen;
          uint8_t kind = *p++;
          if((memcmp(name, gasImportName, nameLen) == 0 || memcmp(name, gasImport2Name, nameLen) == 0) &&
             memcmp(module, gasImportModule, moduleLen) == 0 &&
             kind == 0) {
            printf("gas import already exists! exiting!\n");
            return 2;
          }
          switch(kind) {
            case 0: { // function
              uint32_t typeId;
              leb128ReadU32(&p, end, &typeId);
              functionCount ++;
            } break;
            case 1: { // table
              uint8_t elementType = *p++;
              uint8_t flags = *p++;
              uint32_t t;
              leb128ReadU32(&p, end, &t);
              if (flags & 1) leb128ReadU32(&p, end, &t);
            } break;
            case 2: { // memory
              uint8_t flags = *p++;
              uint32_t t;
              leb128ReadU32(&p, end, &t);
              if (flags & 1) leb128ReadU32(&p, end, &t);
            } break;
            case 3: { // global
              *p+= 2; // valuetype + mutability
            } break;
          }
        }

        p = sp;

        appendVectorSection(&p, &end, sectionSize, &out, &o, &outEnd,
                                           gasImport, gasImportSize, 2);
        gasImportId = functionCount;
        importAdded = 1;
      } break;
      case 7: {
        uint32_t count;
        uint8_t* eout = malloc(128);
        uint8_t* eoutEnd = eout + 128;
        uint8_t* eo = eout;
        leb128ReadU32(&p, end, &count);
        leb128WriteU32(&eo, count);
        for(uint32_t i = 0; i < count; ++i) {
          uint32_t nameLen;
          leb128ReadU32(&p, end, &nameLen);
          ensureSpace(8 + nameLen + 10, &eout, &eoutEnd, &eo);
          leb128WriteU32(&eo, nameLen);
          uint8_t* name = p;
          memcpy(eo, name, nameLen);
          p += nameLen;
          eo += nameLen;
          uint8_t kind = *p++;
          *eo++ = kind;
          switch(kind) {
            case 0: { // function
              uint32_t funcId;
              leb128ReadU32(&p, end, &funcId);
              if(funcId >= gasImportId) {
                funcId+=2;
              }
              leb128WriteU32(&eo, funcId);
            } break;
            case 1: { // table
              uint32_t tableId;
              leb128ReadU32(&p, end, &tableId);
              leb128WriteU32(&eo, tableId);
            } break;
            case 2: { // memory
              uint32_t memoryId;
              leb128ReadU32(&p, end, &memoryId);
              leb128WriteU32(&eo, memoryId);
            } break;
            case 3: { // global
              uint32_t globalId;
              leb128ReadU32(&p, end, &globalId);
              leb128WriteU32(&eo, globalId);
            } break;
          }
        }
        ensureSpace(8 + eo - eout, &out, &outEnd, &o);
        leb128WriteU32(&o, eo - eout);
        memcpy(o, eout, eo - eout);
        o += eo - eout;
        free(eout);
      } break;
      case 10: {
        uint32_t count;
        uint8_t* sout = malloc(128);
        uint8_t* soutEnd = sout + 128;
        uint8_t* so = sout;
        leb128ReadU32(&p, end, &count);
        leb128WriteU32(&so, count);
        for(uint32_t i = 0; i < count; ++i) {
          uint32_t bodySize;
          leb128ReadU32(&p, end, &bodySize);
          uint8_t* cout = malloc(128);
          uint8_t* coutEnd = cout + 128;
          uint8_t* co = cout;
          int ret = meterCode(&p, p + bodySize, &cout, &co, &coutEnd, gasTypeId, gasImportId);
          if(ret) {
            printf("Metering failed %d\n", ret);
            return ret;
          }
          ensureSpace(8 + co - cout, &sout, &soutEnd, &so);
          leb128WriteU32(&so, co - cout);
          memcpy(so, cout, co - cout);
          p += bodySize;
          so += co - cout;
          free(cout);
        }
        ensureSpace(8 + so - sout, &out, &outEnd, &o);
        leb128WriteU32(&o, so - sout);
        memcpy(o, sout, so - sout);
        o += so - sout;
        free(sout);
      } break;
      default: {
        printf("Unknown section %d\n", section);
        ensureSpace(sectionSize + 8, &out, &outEnd, &o);
        leb128WriteU32(&o, sectionSize);
        memcpy(o, p, sectionSize);
        p += sectionSize;
        o += sectionSize;
      }
    }
    if(p != sectionEnd) {
      printf("Section %d, sectionSize %d, read %ld\n", section, sectionSize, p - sectionEnd);
      return 1;
    }
  }

  *outp = out;
  *outSize = o - out;
  return 0;
}