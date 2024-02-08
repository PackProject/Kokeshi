#include <libkiwi.h>

namespace kiwi {
namespace {

/**
 * @brief Print heap information
 *
 * @param name Heap name
 * @param heap Heap object
 */
void LogHeap(const char* name, EGG::Heap* heap) {
    if (heap == NULL) {
        K_LOG_EX("[%s] NULL ->", name);
        return;
    }

    K_LOG_EX("[%s] %p-> %.2fKB free", name, heap,
             OS_MEM_B_TO_KB(static_cast<f32>(heap->getAllocatableSize())));
}

} // namespace

/**
 * Constructor
 */
MemoryMgr::MemoryMgr() {
    LogHeap("RootMEM1", RPSysSystem::getRootHeapMem1());
    LogHeap("RootMEM2", RPSysSystem::getRootHeapMem2());
    LogHeap("System", RPSysSystem::getSystemHeap());

    mpHeap = EGG::ExpHeap::create(scHeapSize, RPSysSystem::getSystemHeap(), 0);
    LogHeap("libkiwi", mpHeap);

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
void* MemoryMgr::Alloc(u32 size, s32 align) {
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
u32 MemoryMgr::GetFreeSize() {
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
