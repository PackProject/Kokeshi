#ifndef EGG_CORE_DVDRIPPER_H
#define EGG_CORE_DVDRIPPER_H
#include "types_egg.h"

namespace EGG {
struct DvdRipper {
    enum EAllocDirection {
        ALLOC_TAIL,
        ALLOC_HEAD,
    };

    static void* loadToMainRAM(const char*, u8*, Heap*, EAllocDirection, u32,
                               u32*, u32*);
    static void* loadToMainRAM(DvdFile*, u8*, Heap*, EAllocDirection, u32, u32*,
                               u32*);

    static bool sErrorRetry;
};
} // namespace EGG

#endif
