#include <egg/core.h>
#include <libkiwi.h>

namespace kiwi {
namespace {

/**
 * @brief Prints heap information
 *
 * @param pName Heap name
 * @param pHeap Heap object
 */
void LogHeap(const char* pName, EGG::Heap* pHeap) {
    if (pHeap == nullptr) {
        K_LOG_EX("[%s] nullptr ->\n", pName);
        return;
    }

    K_LOG_EX("[%s] %p-> %.2fKB free\n", pName, pHeap,
             OS_MEM_B_TO_KB(static_cast<f32>(pHeap->getAllocatableSize())));
}

/**
 * @brief Catches erroneous double-frees
 *
 * @param pBlock Target of delete operation
 */
void CheckDoubleFree(const void* pBlock) {
#ifndef NDEBUG
    // nullptr delete is OK
    if (pBlock == nullptr) {
        return;
    }

    // Catch invalid pointers while we're here
    K_ASSERT(OSIsMEM1Region(pBlock) || OSIsMEM2Region(pBlock));

    // Sanity check, should always be ExpHeap
    MEMiHeapHead* pHandle = MEMFindContainHeap(pBlock);
    K_ASSERT(pHandle != nullptr);
    K_ASSERT(pHandle->magic == 'EXPH');

    // Check that the block is still marked as used
    MEMiExpHeapMBlock* pMBlock = static_cast<MEMiExpHeapMBlock*>(
        AddToPtr(pMBlock, -sizeof(MEMiExpHeapMBlock)));
    K_ASSERT_EX(pMBlock->state == 'UD', "Double free!");
#endif
}

} // namespace

/**
 * @brief Constructor
 */
MemoryMgr::MemoryMgr() {
    // clang-format off
    mpHeapMEM1 = EGG::ExpHeap::create(scHeapSize, RP_GET_INSTANCE(RPSysSystem)->getSystemHeap(),   0);
    mpHeapMEM2 = EGG::ExpHeap::create(scHeapSize, RP_GET_INSTANCE(RPSysSystem)->getResourceHeap(), 0);

    LogHeap("RPSysSystem:System",   RP_GET_INSTANCE(RPSysSystem)->getSystemHeap());
    LogHeap("RPSysSystem:Resource", RP_GET_INSTANCE(RPSysSystem)->getResourceHeap());
    LogHeap("libkiwi:MEM1",         mpHeapMEM1);
    LogHeap("libkiwi:MEM2",         mpHeapMEM2);
    // clang-format on

    K_ASSERT(mpHeapMEM1 != nullptr);
    K_ASSERT(mpHeapMEM2 != nullptr);
    K_ASSERT(OSIsMEM1Region(mpHeapMEM1));
    K_ASSERT(OSIsMEM2Region(mpHeapMEM2));
}

/**
 * @brief Destructor
 */
MemoryMgr::~MemoryMgr() {
    delete mpHeapMEM1;
    delete mpHeapMEM2;
}

/**
 * @brief Gets the heap corresponding to the specified memory region
 *
 * @param memory Target memory region
 */
EGG::Heap* MemoryMgr::GetHeap(EMemory memory) const {
    K_ASSERT(memory < EMemory_Max);

    EGG::Heap* pHeap = memory == EMemory_MEM1 ? mpHeapMEM1 : mpHeapMEM2;
    K_ASSERT(pHeap != nullptr);

    return pHeap;
}

/**
 * @brief Allocates a block of memory
 *
 * @param size Block size
 * @param align Block alignment
 * @param memory Target memory region
 * @return void* Pointer to allocated block
 */
void* MemoryMgr::Alloc(u32 size, s32 align, EMemory memory) const {
    void* pBlock = GetHeap(memory)->alloc(size, align);
    K_ASSERT_EX(pBlock != nullptr, "Out of memory (alloc %d)", size);

    K_ASSERT(memory == EMemory_MEM1 ? OSIsMEM1Region(pBlock)
                                    : OSIsMEM2Region(pBlock));

    return pBlock;
}

/**
 * @brief Frees a block of memory
 *
 * @param pBlock Block
 */
void MemoryMgr::Free(void* pBlock) const {
    CheckDoubleFree(pBlock);
    EGG::Heap::free(pBlock, nullptr);
}

/**
 * @brief Gets total size of available heap memory
 *
 * @param memory Target memory region
 */
u32 MemoryMgr::GetFreeSize(EMemory memory) const {
    return GetHeap(memory)->getAllocatableSize();
}

/**
 * @brief Tests whether an address points to an allocation from this manager
 *
 * @param pAddr Memory address
 */
bool MemoryMgr::IsHeapMemory(const void* pAddr) const {
    K_ASSERT(mpHeapMEM1 != nullptr && mpHeapMEM2 != nullptr);

    // Check MEM1 heap
    if (pAddr >= mpHeapMEM1->getStartAddress() &&
        pAddr < mpHeapMEM1->getEndAddress()) {
        return true;
    }

    // Check MEM2 heap
    if (pAddr >= mpHeapMEM2->getStartAddress() &&
        pAddr < mpHeapMEM2->getEndAddress()) {
        return true;
    }

    return false;
}

} // namespace kiwi

/**
 * @brief Allocates a block of memory
 *
 * @param size Block size
 * @return Pointer to allocated block
 */
void* operator new(size_t size) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4, kiwi::EMemory_MEM1);
}
/**
 * @brief Allocates a block of memory for an array
 *
 * @param size Block size
 * @return Pointer to allocated block
 */
void* operator new[](size_t size) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4, kiwi::EMemory_MEM1);
}

/**
 * @brief Allocates a block of memory
 *
 * @param size Block size
 * @param align Block address alignment
 * @return Pointer to allocated block
 */
void* operator new(size_t size, s32 align) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align,
                                                kiwi::EMemory_MEM1);
}
/**
 * @brief Allocates a block of memory for an array
 *
 * @param size Block size
 * @param align Block address alignment
 * @return Pointer to allocated block
 */
void* operator new[](size_t size, s32 align) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align,
                                                kiwi::EMemory_MEM1);
}

/**
 * @brief Allocates a block of memory
 *
 * @param size Block size
 * @param memory Target memory region
 * @return Pointer to allocated block
 */
void* operator new(size_t size, kiwi::EMemory memory) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4, memory);
}
/**
 * @brief Allocates a block of memory for an array
 *
 * @param size Block size
 * @param memory Target memory region
 * @return Pointer to allocated block
 */
void* operator new[](size_t size, kiwi::EMemory memory) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4, memory);
}

/**
 * @brief Allocates a block of memory
 *
 * @param size Block size
 * @param align Block address alignment
 * @param memory Target memory region
 * @return Pointer to allocated block
 */
void* operator new(size_t size, s32 align, kiwi::EMemory memory) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align, memory);
}
/**
 * @brief Allocates a block of memory for an array
 *
 * @param size Block size
 * @param align Block address alignment
 * @param memory Target memory region
 * @return Pointer to allocated block
 */
void* operator new[](size_t size, s32 align, kiwi::EMemory memory) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align, memory);
}

/**
 * @brief Frees a block of memory
 *
 * @param pBlock Block
 */
void operator delete(void* pBlock) {
    kiwi::MemoryMgr::GetInstance().Free(pBlock);
}
/**
 * @brief Frees a block of memory used by an array
 *
 * @param pBlock Block
 */
void operator delete[](void* pBlock) {
    kiwi::MemoryMgr::GetInstance().Free(pBlock);
}
