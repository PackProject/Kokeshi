#ifndef LIBKIWI_KERNEL_MEMORY_MGR_H
#define LIBKIWI_KERNEL_MEMORY_MGR_H
#include <egg/core.h>
#include <libkiwi/util/kiwiStaticSingleton.hpp>
#include <types.h>

namespace kiwi {

/**
 * Module memory manager
 */
class MemoryMgr : public StaticSingleton<MemoryMgr> {
    friend class StaticSingleton<MemoryMgr>;

public:
    void* Alloc(u32 size, s32 align);
    void Free(void* block);
    u32 GetFreeSize();

    /**
     * Access underlying heap for use with EGG functions
     * TODO: Remove this and reimplement required EGG features
     */
    EGG::Heap* GetEggHeap() {
        return mpHeap;
    }

private:
    MemoryMgr();
    ~MemoryMgr();

private:
    // Main heap
    EGG::ExpHeap* mpHeap;

    // Main heap size
    static const u32 scHeapSize = OS_MEM_KB_TO_B(256);
};

} // namespace kiwi

#endif
