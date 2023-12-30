/*
 * Kamek
 * Wii game patching engine
 * (c) Treeki 2010-2018
 */

#ifndef KAMEK_LOADER_H
#define KAMEK_LOADER_H
#include <kamekTypes.hpp>

namespace kamek {

typedef void* (*KamekAlloc_t)(u32 size, bool isForCode);
typedef void (*KamekFree_t)(void* buffer, bool isForCode);

struct loaderFunctions {
    KamekAlloc_t kamekAlloc;
    KamekFree_t kamekFree;
};

void loadKamekBinaryFromDisc(const loaderFunctions* funcs, const char* path);

} // namespace kamek

#endif
