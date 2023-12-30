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
    void* Alloc(std::size_t size, s32 align);
    void Free(void* block);
    std::size_t GetFreeSize();

private:
    MemoryMgr();
    ~MemoryMgr();

private:
    // Main heap
    EGG::ExpHeap* mpHeap;

    // Main heap size
    static const std::size_t scHeapSize = OS_MEM_KB_TO_B(256);
};

} // namespace kiwi

#endif
