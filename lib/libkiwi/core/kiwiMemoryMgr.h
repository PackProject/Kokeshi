#ifndef LIBKIWI_CORE_MEMORY_MGR_H
#define LIBKIWI_CORE_MEMORY_MGR_H
#include <egg/core.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiStaticSingleton.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief Memory region
 */
enum EMemory {
    EMemory_MEM1, //!< 24MB RAM brought over from NGC. [0x80000000 - 0x817FFFFF]
    EMemory_MEM2, //!< 64MB RAM unique to RVL.         [0x90000000 - 0x93FFFFFF]

    EMemory_Max
};

/**
 * @brief Memory manager
 */
class MemoryMgr : public StaticSingleton<MemoryMgr> {
    friend class StaticSingleton<MemoryMgr>;

public:
    /**
     * @brief Allocates a block of memory
     *
     * @param size Block size
     * @param align Block alignment
     * @param memory Target memory region
     * @return Pointer to allocated block
     */
    void* Alloc(u32 size, s32 align, EMemory region) const;

    /**
     * @brief Frees a block of memory
     *
     * @param pBlock Block
     */
    void Free(void* pBlock) const;

    /**
     * @brief Gets total size of available heap memory
     *
     * @param memory Target memory region
     */
    u32 GetFreeSize(EMemory region) const;

    /**
     * @brief Tests whether an address points to an allocation from this manager
     *
     * @param pAddr Memory address
     */
    bool IsHeapMemory(const void* pAddr) const;

private:
    /**
     * @brief Constructor
     */
    MemoryMgr();
    /**
     * @brief Destructor
     */
    ~MemoryMgr();

    /**
     * @brief Gets the heap corresponding to the specified memory region
     *
     * @param memory Target memory region
     */
    EGG::Heap* GetHeap(EMemory memory) const;

private:
    //! Initial size for each heap
    static const u32 scHeapSize = OS_MEM_KB_TO_B(1024);

    EGG::Heap* mpHeapMEM1; //!< Heap in MEM1 region
    EGG::Heap* mpHeapMEM2; //!< Heap in MEM2 region
};

//! @}
} // namespace kiwi

//! @addtogroup libkiwi_core
//! @{

/**
 * @brief Allocates a block of memory
 *
 * @param size Block size
 * @return Pointer to allocated block
 */
void* operator new(size_t size);
/**
 * @brief Allocates a block of memory for an array
 *
 * @param size Block size
 * @return Pointer to allocated block
 */
void* operator new[](size_t size);

/**
 * @brief Allocates a block of memory
 *
 * @param size Block size
 * @param align Block address alignment
 * @return Pointer to allocated block
 */
void* operator new(size_t size, s32 align);
/**
 * @brief Allocates a block of memory for an array
 *
 * @param size Block size
 * @param align Block address alignment
 * @return Pointer to allocated block
 */
void* operator new[](size_t size, s32 align);

/**
 * @brief Allocates a block of memory
 *
 * @param size Block size
 * @param memory Target memory region
 * @return Pointer to allocated block
 */
void* operator new(size_t size, kiwi::EMemory memory);
/**
 * @brief Allocates a block of memory for an array
 *
 * @param size Block size
 * @param memory Target memory region
 * @return Pointer to allocated block
 */
void* operator new[](size_t size, kiwi::EMemory memory);

/**
 * @brief Allocates a block of memory
 *
 * @param size Block size
 * @param align Block address alignment
 * @param memory Target memory region
 * @return Pointer to allocated block
 */
void* operator new(size_t size, s32 align, kiwi::EMemory memory);
/**
 * @brief Allocates a block of memory for an array
 *
 * @param size Block size
 * @param align Block address alignment
 * @param memory Target memory region
 * @return Pointer to allocated block
 */
void* operator new[](size_t size, s32 align, kiwi::EMemory memory);

/**
 * @brief Frees a block of memory
 *
 * @param pBlock Block
 */
void operator delete(void* pBlock);
/**
 * @brief Frees a block of memory used by an array
 *
 * @param pBlock Block
 */
void operator delete[](void* pBlock);

//! @}

#endif
