#ifndef LIBKIWI_UTIL_IOS_OBJECT_H
#define LIBKIWI_UTIL_IOS_OBJECT_H
#include <cstring>
#include <libkiwi/core/kiwiMemoryMgr.h>
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>
#include <libkiwi/util/kiwiIosVector.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Memory buffer for IOS I/O
 */
template <typename T> class IosBuffer : public IosVector {
public:
    /**
     * @brief Constructor
     *
     * @param size Buffer size
     */
    explicit IosBuffer(u32 size) {
        K_ASSERT(size > 0);

        u8* buffer = new (32, EMemory_MEM2) u8[size * sizeof(T)];
        K_ASSERT(buffer != nullptr);

        Set(buffer, size);
    }

    /**
     * @brief Destructor
     */
    virtual ~IosBuffer() {
        delete static_cast<u8*>(Base());
    }

    /**
     * @brief Access buffer memory
     */
    T* Ptr() const {
        return reinterpret_cast<T*>(Base());
    }
};

/**
 * @brief Strongly typed memory buffer for IOS I/O
 *
 * @tparam T Underlying object type
 */
template <typename T> class IosObject : public IosBuffer<u8> {
public:
    /**
     * @brief Constructor
     */
    IosObject() : IosBuffer(sizeof(T)) {
        new (Base()) T();
    }

    /**
     * @brief Constructor
     * @details Copy constructor
     *
     * @param other Object to copy
     */
    IosObject(const IosObject& other) : IosBuffer<u8>(sizeof(T)) {
        new (Base()) T(other.Ref());
    }

    /**
     * @brief Constructor
     * @details Value constructor
     *
     * @param value Value to copy
     */
    IosObject(const T& value) : IosBuffer<u8>(sizeof(T)) {
        new (Base()) T(value);
    }

    /**
     * @brief Destructor
     */
    virtual ~IosObject() {
        // IosBuffer frees this memory
        Ptr()->~T();
    }

    /**
     * @brief Access underlying object (pointer)
     */
    T* Ptr() const {
        K_ASSERT(Base() != nullptr);
        return reinterpret_cast<T*>(Base());
    }

    /**
     * @brief Access underlying object (reference)
     */
    T& Ref() {
        return *Ptr();
    }
    /**
     * @brief Access underlying object (reference, read-only)
     */
    const T& Ref() const {
        return *Ptr();
    }

    /**
     * @brief Dereference pointer
     */
    T& operator*() {
        return Ref();
    }
    /**
     * @brief Dereference pointer (read-only)
     */
    const T& operator*() const {
        return Ref();
    }

    /**
     * @brief Pointer access
     */
    T* operator->() {
        return Ptr();
    }
    /**
     * @brief Pointer access (read-only)
     */
    const T* operator->() const {
        return Ptr();
    }
};

/**
 * @brief IOS object specialization for strings
 *
 * @tparam T Type of character in string
 */
template <typename T> class IosString : public IosBuffer<T> {
public:
    /**
     * @brief Constructor
     *
     * @param capacity String capacity
     */
    explicit IosString(u32 capacity) : IosBuffer<T>(capacity + 1) {
        *this->Ptr() = 0;
    }

    /**
     * @brief Constructor
     * @details Value constructor
     *
     * @param value Existing string to copy
     */
    IosString(const StringImpl<T>& value);

    /**
     * @brief Convert to String type
     */
    operator StringImpl<T>() const {
        return StringImpl<T>(this->Ptr(), this->Length());
    }
};

/**
 * @brief Constructor
 * @details Value constructor (8-bit characters)
 *
 * @param value Existing string to copy
 */
template <>
K_INLINE IosString<char>::IosString(const StringImpl<char>& value)
    : IosBuffer<char>(value.Length() + 1) {
    std::strncpy(Ptr(), value.CStr(), value.Length());
    Ptr()[value.Length()] = 0;
}

/**
 * @brief Constructor
 * @details Value constructor (16-bit characters)
 *
 * @param value Existing string to copy
 */
template <>
K_INLINE IosString<wchar_t>::IosString(const StringImpl<wchar_t>& value)
    : IosBuffer<wchar_t>(value.Length() + 1) {
    std::wcsncpy(Ptr(), value.CStr(), value.Length());
    Ptr()[value.Length()] = 0;
}

//! @}
} // namespace kiwi

#endif
