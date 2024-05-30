#ifndef LIBKIWI_CORE_MEMORY_MGR_H
#define LIBKIWI_CORE_MEMORY_MGR_H
#include <egg/core.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiStaticSingleton.h>

namespace kiwi {

/**
 * @brief Memory region from which to allocate
 */
enum EMemory { EMemory_MEM1, EMemory_MEM2, EMemory_Max };

/**
 * Module memory manager
 */
class MemoryMgr : public StaticSingleton<MemoryMgr> {
    friend class StaticSingleton<MemoryMgr>;

public:
    void* Alloc(u32 size, s32 align, EMemory region);
    void Free(void* block);
    u32 GetFreeSize(EMemory region);

    /**
     * @brief Access MEM1 heap
     */
    EGG::Heap* GetHeapMEM1() const {
        return mpHeapMEM1;
    }

    /**
     * @brief Access MEM2 heap
     */
    EGG::Heap* GetHeapMEM2() const {
        return mpHeapMEM2;
    }

private:
    MemoryMgr();
    ~MemoryMgr();

private:
    EGG::Heap* mpHeapMEM1; // Heap in MEM1 region
    EGG::Heap* mpHeapMEM2; // Heap in MEM2 region

    // Main heap size
    static const u32 scHeapSize = OS_MEM_KB_TO_B(1024);
};

} // namespace kiwi

void* operator new(std::size_t size);
void* operator new[](std::size_t size);

void* operator new(std::size_t size, s32 align);
void* operator new[](std::size_t size, s32 align);

void operator delete(void* block);
void operator delete[](void* block);

void* operator new(std::size_t size, kiwi::EMemory memory);
void* operator new[](std::size_t size, kiwi::EMemory memory);

void* operator new(std::size_t size, s32 align, kiwi::EMemory memory);
void* operator new[](std::size_t size, s32 align, kiwi::EMemory memory);

#endif
