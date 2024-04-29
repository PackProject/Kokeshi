#include <libkiwi.h>

namespace kiwi {
namespace {

/**
 * @brief Print heap information
 *
 * @param name Heap name
 * @param heap Heap object
 */
void LogHeap(const String& name, EGG::Heap* heap) {
    if (heap == NULL) {
        K_LOG_EX("[%s] NULL ->\n", name.CStr());
        return;
    }

    K_LOG_EX("[%s] %p-> %.2fKB free\n", name.CStr(), heap,
             OS_MEM_B_TO_KB(static_cast<f32>(heap->getAllocatableSize())));
}

} // namespace

/**
 * Constructor
 */
MemoryMgr::MemoryMgr() {
    // clang-format off
    mpHeapMEM1 = EGG::ExpHeap::create(scHeapSize, RPSysSystem::getSystemHeap(), 0);
    LogHeap("RPSysSystem:System", RPSysSystem::getSystemHeap());
    LogHeap("libkiwi:MEM1", mpHeapMEM1);

    K_ASSERT(mpHeapMEM1 != NULL);
    K_ASSERT(OSIsMEM1Region(mpHeapMEM1));

    mpHeapMEM2 = EGG::ExpHeap::create(scHeapSize, RPSysSystem::getInstance()->getResourceHeap(), 0);
    LogHeap("RPSysSystem:Resource", RPSysSystem::getInstance()->getResourceHeap());
    LogHeap("libkiwi:MEM2", mpHeapMEM2);

    K_ASSERT(mpHeapMEM2 != NULL);
    K_ASSERT(OSIsMEM2Region(mpHeapMEM2));
    // clang-format on
}

/**
 * Destructor
 */
MemoryMgr::~MemoryMgr() {
    delete mpHeapMEM1;
    mpHeapMEM1 = NULL;

    delete mpHeapMEM2;
    mpHeapMEM2 = NULL;
}

/**
 * Allocates a block of memory
 *
 * @param size Block size
 * @param align Block alignment
 * @param memory Target memory region
 * @return void* Pointer to allocated block
 */
void* MemoryMgr::Alloc(u32 size, s32 align, EMemory memory) {
    K_ASSERT(memory < EMemory_Max);
    K_ASSERT(mpHeapMEM1 != NULL && mpHeapMEM2 != NULL);

    EGG::Heap* heap = memory == EMemory_MEM1 ? mpHeapMEM1 : mpHeapMEM2;
    void* block = heap->alloc(size, align);

    K_ASSERT_EX(block != NULL, "Out of memory (alloc %d)", size);
    K_ASSERT(memory == EMemory_MEM1 ? OSIsMEM1Region(block)
                                    : OSIsMEM2Region(block));

    return block;
}

/**
 * Frees a block of memory
 *
 * @param block Block
 */
void MemoryMgr::Free(void* block) {
    K_ASSERT(mpHeapMEM1 != NULL && mpHeapMEM2 != NULL);
    EGG::Heap::free(block, NULL);
}

/**
 * Gets total size of available heap memory
 *
 * @param memory Target memory region
 */
u32 MemoryMgr::GetFreeSize(EMemory memory) {
    K_ASSERT(memory < EMemory_Max);
    K_ASSERT(mpHeapMEM1 != NULL && mpHeapMEM2 != NULL);

    EGG::Heap* heap = memory == EMemory_MEM1 ? mpHeapMEM1 : mpHeapMEM2;
    return heap->getAllocatableSize();
}

} // namespace kiwi

void* operator new(std::size_t size) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4, kiwi::EMemory_MEM1);
}
void* operator new[](std::size_t size) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4, kiwi::EMemory_MEM1);
}

void* operator new(std::size_t size, s32 align) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align,
                                                kiwi::EMemory_MEM1);
}
void* operator new[](std::size_t size, s32 align) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align,
                                                kiwi::EMemory_MEM1);
}

void* operator new(std::size_t size, kiwi::EMemory memory) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4, memory);
}
void* operator new[](std::size_t size, kiwi::EMemory memory) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, 4, memory);
}

void* operator new(std::size_t size, s32 align, kiwi::EMemory memory) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align, memory);
}
void* operator new[](std::size_t size, s32 align, kiwi::EMemory memory) {
    return kiwi::MemoryMgr::GetInstance().Alloc(size, align, memory);
}

void operator delete(void* block) {
    kiwi::MemoryMgr::GetInstance().Free(block);
}
void operator delete[](void* block) {
    kiwi::MemoryMgr::GetInstance().Free(block);
}
