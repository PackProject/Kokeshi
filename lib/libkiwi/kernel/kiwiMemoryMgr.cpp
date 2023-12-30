#include <libkiwi.h>

namespace kiwi {

/**
 * Constructor
 */
MemoryMgr::MemoryMgr() {
    // Child of sAllocatableHeap
    mpHeap = EGG::ExpHeap::create(scHeapSize, NULL, 0);
    K_ASSERT(mpHeap != NULL);
}

/**
 * Destructor
 */
MemoryMgr::~MemoryMgr() {
    delete mpHeap;
    mpHeap = NULL;
}

/**
 * Allocates a block of memory
 *
 * @param size Block size
 * @param align Block alignment
 * @return void* Pointer to allocated block
 */
void* MemoryMgr::Alloc(std::size_t size, s32 align) {
    K_ASSERT_EX(mpHeap != NULL, "Please call MemoryMgr::Initialize");
    void* block = mpHeap->alloc(size, align);

    K_ASSERT_EX(block != NULL, "Out of memory (alloc %d)", size);
    return block;
}

/**
 * Frees a block of memory
 *
 * @param block Block
 */
void MemoryMgr::Free(void* block) {
    K_ASSERT_EX(mpHeap != NULL, "Please call MemoryMgr::Initialize");
    mpHeap->free(block);
}

/**
 * Gets total size of available heap memory
 */
std::size_t MemoryMgr::GetFreeSize() {
    K_ASSERT_EX(mpHeap != NULL, "Please call MemoryMgr::Initialize");
    return mpHeap->getAllocatableSize(4);
}

} // namespace kiwi

void* operator new(std::size_t size) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4);
}
void* operator new[](std::size_t size) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4);
}

void* operator new(std::size_t size, s32 align) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align);
}
void* operator new[](std::size_t size, s32 align) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align);
}

void operator delete(void* block) {
    kiwi::MemoryMgr::GetInstance().Free(block);
}
void operator delete[](void* block) {
    kiwi::MemoryMgr::GetInstance().Free(block);
}
