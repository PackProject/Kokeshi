// Implementation header
#ifndef LIBKIWI_PRIM_VECTOR_IMPL_HPP
#define LIBKIWI_PRIM_VECTOR_IMPL_HPP

// Declaration header
#ifndef LIBKIWI_PRIM_VECTOR_H
#include <libkiwi/prim/kiwiVector.h>
#endif

#include <cstring>

namespace kiwi {

/**
 * @brief Clears vector contents
 */
template <typename T> K_INLINE void TVector<T>::Clear() {
    K_ASSERT(mSize == 0 || mpData != nullptr);

    for (u32 i = 0; i < mSize; i++) {
        Buffer()[i].~T();
    }

    mSize = 0;
}

/**
 * @brief Inserts a new element at the specified position
 *
 * @param rElem New element
 * @param pos Element position
 */
template <typename T>
K_INLINE void TVector<T>::Insert(const T& rElem, u32 pos) {
    K_ASSERT(pos <= mSize);

    // Make space for one extra element
    Reserve(mSize + 1);

    K_ASSERT(mpData != nullptr);

    // Inserted in the middle, copy forward
    if (pos < mSize) {
        std::memcpy(Buffer() + pos + 1, Buffer() + pos,
                    (mSize - pos) * sizeof(T));
    }

    // Copy construct in-place
    new (&Buffer()[pos]) T(rElem);
    mSize++;
}

/**
 * @brief Removes an element if it exists in the vector
 *
 * @param rElem Element to remove
 * @return Whether the element existed and was removed
 */
template <typename T> K_INLINE bool TVector<T>::Remove(const T& rElem) {
    K_ASSERT(mSize == 0 || mpData != nullptr);

    // Linear search for the target
    for (u32 i = 0; i < mSize; i++) {
        if (Buffer()[i] == rElem) {
            RemoveAt(i);
            return true;
        }
    }

    // Element not in the vector
    return false;
}

/**
 * @brief Removes an element at the specified position
 *
 * @param pos Element position
 */
template <typename T> K_INLINE void TVector<T>::RemoveAt(u32 pos) {
    K_ASSERT(pos < mSize);
    K_ASSERT(mpData != nullptr);

    // Destroy element
    Buffer()[pos].~T();

    // Removed from the middle, copy backward
    if (pos < mSize) {
        std::memcpy(Buffer() + pos, Buffer() + pos + 1,
                    (mSize - pos) * sizeof(T));
    }

    mSize--;
}

/**
 * @brief Inserts a new element at the back of the vector
 *
 * @param rElem New element
 */
template <typename T> K_INLINE void TVector<T>::PushBack(const T& rElem) {
    Insert(rElem, mSize);
}

/**
 * @brief Removes the last element from the vector
 */
template <typename T> K_INLINE void TVector<T>::PopBack() {
    K_ASSERT(mSize > 0);
    Remove(mSize - 1);
}

/**
 * @brief Reserves space for elements in the vector
 *
 * @param capacity New capacity
 */
template <typename T> K_INLINE void TVector<T>::Reserve(u32 capacity) {
    // All good!
    if (mCapacity >= capacity) {
        return;
    }

    // Need to reallocate
    u8* pBuffer = new u8[capacity * sizeof(T)];
    K_ASSERT(pBuffer != nullptr);

    // Copy in old data
    if (mpData != nullptr) {
        std::memcpy(pBuffer, mpData, mSize * sizeof(T));
        delete mpData;
    }

    // Swap buffer
    mpData = pBuffer;
    mCapacity = capacity;
}

/**
 * @brief Copies vector contents
 *
 * @param rOther Vector to copy from
 */
template <typename T>
K_INLINE void TVector<T>::CopyFrom(const TVector& rOther) {
    // Destroy existing contents
    Clear();

    // Make sure we can fit the contents
    Reserve(rOther.mSize);
    std::memcpy(mpData, rOther.mpData, rOther.mSize * sizeof(T));
}

/**
 * @brief Moves vector contents
 *
 * @param rOther Vector to move
 */
template <typename T> K_INLINE void TVector<T>::MoveFrom(TVector&& rOther) {
    // Destroy contents & free buffer
    Clear();
    delete mpData;

    mpData = rOther.mpData;
    mCapacity = rOther.mCapacity;
    mSize = rOther.mSize;

    rOther.mpData = nullptr;
    rOther.mCapacity = 0;
    rOther.mSize = 0;
}

} // namespace kiwi

#endif
