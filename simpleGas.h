//
// Created by Michał Łaszczewski <michal@laszczewski.com> on 3/8/23.
//

#ifndef WASM_UTILS_SIMPLEGAS_H
#define WASM_UTILS_SIMPLEGAS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  int addGasMetering(uint8_t* binary, uint32_t binarySize, uint8_t** out, size_t* outSize,
                   const char* gasImportModule, const char* gasImportName, const char* gasImport2Name);

#ifdef __cplusplus
}
#endif

#endif //WASM_UTILS_SIMPLEGAS_H
