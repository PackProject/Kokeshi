#ifndef LIBKIWI_PRIM_VECTOR_H
#define LIBKIWI_PRIM_VECTOR_H
#include <algorithm>
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_prim
//! @{

/**
 * @brief Dynamically-sized, contiguous array (std::vector)
 */
template <typename T> class TVector {
public:
    /**
     * @brief Constructor
     */
    TVector() : mpData(nullptr), mCapacity(0), mSize(0) {}

    /**
     * @brief Constructor
     *
     * @param capacity Buffer capacity
     */
    explicit TVector(u32 capacity) : mpData(nullptr), mCapacity(0), mSize(0) {
        Reserve(capacity);
    }

    /**
     * @brief Constructor
     * @details Copy constructor
     *
     * @param rOther Vector to copy
     */
    TVector(const TVector& rOther) : mpData(nullptr), mCapacity(0), mSize(0) {
        CopyFrom(rOther);
    }

#ifdef LIBKIWI_CPP1X
    /**
     * @brief Constructor
     * @details Move constructor
     *
     * @param rOther Vector to move
     */
    TVector(TVector&& rOther) : mpData(nullptr), mCapacity(0), mSize(0) {
        MoveFrom(std::move(rOther));
    }
#endif

    /**
     * @brief Destructor
     */
    ~TVector() {
        // Destroy contents
        Clear();

        // Free array buffer
        delete mpData;
    }

    /**
     * @brief Vector copy assignment
     *
     * @param rOther Vector to copy
     */
    TVector& operator=(const TVector& rOther) {
        CopyFrom(rOther);
        return *this;
    }

#ifdef LIBKIWI_CPP1X
    /**
     * @brief Vector move assignment
     *
     * @param rOther Vector to move
     */
    TVector& operator=(TVector&& rOther) {
        MoveFrom(std::move(rOther));
        return *this;
    }
#endif

    /**
     * @brief Gets the number of elements in the vector
     */
    u32 Size() const {
        return mSize;
    }

    /**
     * @brief Tests whether the vector is empty
     */
    bool Empty() const {
        return mSize == 0;
    }

    /**
     * @brief Accesses element
     *
     * @param i Element index
     * @return Reference to element
     */
    T& operator[](u32 i) {
        K_ASSERT(i < mSize);
        K_ASSERT(mpData != nullptr);
        return Buffer()[i];
    }
    /**
     * @brief Accesses element (read-only)
     *
     * @param i Element index
     * @return Reference to element
     */
    const T& operator[](u32 i) const {
        K_ASSERT(i < mSize);
        K_ASSERT(mpData != nullptr);
        return Buffer()[i];
    }

    /**
     * @brief Clears vector contents
     */
    void Clear();

    /**
     * @brief Inserts a new element at the specified position
     *
     * @param rElem New element
     * @param pos Element position
     */
    void Insert(const T& rElem, u32 pos);

    /**
     * @brief Removes an element if it exists in the vector
     *
     * @param rElem Element to remove
     * @return Whether the element existed and was removed
     */
    bool Remove(const T& rElem);

    /**
     * @brief Removes an element at the specified position
     *
     * @param pos Element position
     */
    void RemoveAt(u32 pos);

    /**
     * @brief Inserts a new element at the back of the vector
     *
     * @param rElem New element
     */
    void PushBack(const T& rElem);

    /**
     * @brief Removes the last element from the vector
     */
    void PopBack();

private:
    /**
     * @brief Accesses underlying array buffer
     */
    T* Buffer() const {
        return reinterpret_cast<T*>(mpData);
    }

    /**
     * @brief Reserves space for elements in the vector
     *
     * @param capacity New capacity
     */
    void Reserve(u32 capacity);

    /**
     * @brief Copies vector contents
     *
     * @param other Vector to copy from
     */
    void CopyFrom(const TVector& rOther);

    /**
     * @brief Moves vector contents
     *
     * @param other Vector to move
     */
    void MoveFrom(TVector&& rOther);

private:
    u8* mpData;    // Allocated buffer
    u32 mCapacity; // Buffer size
    u32 mSize;     // Number of elements
};

//! @}
} // namespace kiwi

// Implementation header
#ifndef LIBKIWI_PRIM_VECTOR_IMPL_HPP
#include <libkiwi/prim/kiwiVectorImpl.hpp>
#endif

#endif
