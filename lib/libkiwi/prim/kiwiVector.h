#ifndef LIBKIWI_PRIM_VECTOR_H
#define LIBKIWI_PRIM_VECTOR_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {

/**
 * @brief Dynamically-sized, contiguous array (std::vector)
 */
template <typename T> class TVector {
public:
    /**
     * @brief Constructor
     */
    TVector() : mpData(NULL), mCapacity(0), mSize(0) {}

    /**
     * @brief Constructor
     *
     * @param capacity Buffer capacity
     */
    explicit TVector(u32 capacity) : mpData(NULL), mCapacity(0), mSize(0) {
        Reserve(capacity);
    }

    /**
     * @brief Constructor
     *
     * @param other Vector to copy
     */
    TVector(const TVector& other) : mpData(NULL), mCapacity(0), mSize(0) {
        CopyFrom(other);
    }

    /**
     * @brief Destructor
     */
    ~TVector() {
        // Destroy objects
        Clear();

        // Free array buffer
        delete mpData;
        mpData = NULL;
    }

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
     * @brief Access element
     *
     * @param i Element index
     * @return Reference to element
     */
    T& operator[](u32 i) {
        K_ASSERT(i < mSize);
        K_ASSERT(mpData != NULL);
        return Buffer()[i];
    }

    /**
     * @brief Access element (read-only)
     *
     * @param i Element index
     * @return Reference to element
     */
    const T& operator[](u32 i) const {
        K_ASSERT(i < mSize);
        K_ASSERT(mpData != NULL);
        return Buffer()[i];
    }

    /**
     * @brief Clear vector contents
     */
    void Clear();

    /**
     * @brief Insert a new element at the specified position
     *
     * @param t New element
     * @param pos Element position
     */
    void Insert(const T& t, u32 pos);

    /**
     * @brief Remove an element if it exists in the vector
     *
     * @param t Element to remove
     * @return Success
     */
    bool Remove(const T& t);

    /**
     * @brief Remove an element at the specified position
     *
     * @param pos Element position
     */
    void RemoveAt(u32 pos);

    /**
     * @brief Insert a new element at the back of the vector
     *
     * @param t New element
     */
    void PushBack(const T& t);

    /**
     * @brief Remove the last element from the vector
     */
    void PopBack();

private:
    /**
     * @brief Access underlying array buffer
     */
    T* Buffer() const {
        return reinterpret_cast<T*>(mpData);
    }

    void Reserve(u32 capacity);
    void CopyFrom(const TVector& other);

private:
    u8* mpData;    // Allocated buffer
    u32 mCapacity; // Buffer size
    u32 mSize;     // Number of elements
};

} // namespace kiwi

// Implementation header
#ifndef LIBKIWI_PRIM_VECTOR_IMPL_HPP
#include <libkiwi/prim/kiwiVectorImpl.hpp>
#endif

#endif
