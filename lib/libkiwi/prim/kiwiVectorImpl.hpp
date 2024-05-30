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
 * @brief Clear vector contents
 */
template <typename T> void TVector<T>::Clear() {
    K_ASSERT(mSize == 0 || mpData != NULL);

    for (u32 i = 0; i < mSize; i++) {
        Buffer()[i].~T();
    }

    mSize = 0;
}

/**
 * @brief Insert a new element at the specified position
 *
 * @param t New element
 * @param pos Element position
 */
template <typename T> void TVector<T>::Insert(const T& t, u32 pos) {
    K_ASSERT(pos <= mSize);

    // Need to reallocate
    if (mSize >= mCapacity) {
        Reserve(mSize + 1);
    }

    K_ASSERT(mpData != NULL);

    // Inserted in the middle, copy forward
    if (pos < mSize) {
        std::memcpy(Buffer() + pos + 1, Buffer() + pos,
                    (mSize - pos) * sizeof(T));
    }

    // Copy construct in-place
    new (&Buffer()[pos]) T(t);
    mSize++;
}

/**
 * @brief Remove an element if it exists in the vector
 *
 * @param t Element to remove
 * @return Success
 */
template <typename T> bool TVector<T>::Remove(const T& t) {
    K_ASSERT(mpData != NULL);

    // Linear search for the target
    for (u32 i = 0; i < mSize; i++) {
        if (Buffer()[i] == t) {
            RemoveAt(i);
            return true;
        }
    }

    // Element not in the vector
    return false;
}

/**
 * @brief Remove an element at the specified position
 *
 * @param pos Element position
 */
template <typename T> void TVector<T>::RemoveAt(u32 pos) {
    K_ASSERT(pos < mSize);
    K_ASSERT(mpData != NULL);

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
 * @brief Insert a new element at the back of the vector
 *
 * @param t New element
 */
template <typename T> void TVector<T>::PushBack(const T& t) {
    Insert(t, mSize);
}

/**
 * @brief Remove the last element from the vector
 */
template <typename T> void TVector<T>::PopBack() {
    K_ASSERT(mSize > 0);
    Remove(mSize - 1);
}

/**
 * @brief Reserve space for elements in the vector
 *
 * @param capacity New capacity
 */
template <typename T> void TVector<T>::Reserve(u32 capacity) {
    // All good!
    if (mCapacity >= capacity) {
        return;
    }

    // Need to reallocate
    u8* buffer = new u8[capacity * sizeof(T)];
    K_ASSERT(buffer != NULL);

    // Copy in old data
    if (mpData != NULL) {
        std::memcpy(buffer, mpData, mSize * sizeof(T));
        delete mpData;
    }

    // Swap buffer
    mpData = buffer;
    mCapacity = capacity;
}

/**
 * @brief Copy vector contents
 *
 * @param other Vector to copy
 */
template <typename T> void TVector<T>::CopyFrom(const TVector& other) {
    // Destroy existing contents
    Clear();

    // Make sure we can fit the contents
    Reserve(other.mSize);
    std::memcpy(mpData, other.mpData, other.mSize * sizeof(T));
}

} // namespace kiwi

#endif
