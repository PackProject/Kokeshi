#ifndef LIBKIWI_PRIM_STRING_H
#define LIBKIWI_PRIM_STRING_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiBitCast.h>
#include <libkiwi/prim/kiwiHashMap.h>
#include <libkiwi/prim/kiwiVector.h>

namespace kiwi {
//! @addtogroup libkiwi_prim
//! @{

/**
 * @brief String wrapper
 */
template <typename T> class StringImpl {
public:
    /**
     * @brief Constructor
     */
    StringImpl() : mpBuffer(nullptr), mCapacity(0), mLength(0) {
        Clear();
    }

    /**
     * @brief Constructor
     * @details Copy constructor
     *
     * @param rOther String to copy
     */
    StringImpl(const StringImpl& rOther)
        : mpBuffer(nullptr), mCapacity(0), mLength(0) {
        Assign(rOther);
    }

    /**
     * @brief Constructor
     * @details Substring constructor
     *
     * @param rOther String to copy
     * @param pos Substring start index
     * @param len Substring length
     */
    StringImpl(const StringImpl& rOther, u32 pos, u32 len = npos)
        : mpBuffer(nullptr), mCapacity(0), mLength(0) {
        Assign(rOther.SubStr(pos, len));
    }

    /**
     * @brief Constructor
     * @details C-style string constructor
     *
     * @param pStr C-style string
     */
    StringImpl(const T* pStr) : mpBuffer(nullptr), mCapacity(0), mLength(0) {
        Assign(pStr);
    }

    /**
     * @brief Constructor
     * @details Buffer/sequence constructor
     *
     * @param pStr Buffer/sequence
     * @param n Number of characters to copy
     */
    StringImpl(const T* pStr, u32 n)
        : mpBuffer(nullptr), mCapacity(0), mLength(0) {
        Assign(pStr, n);
    }

    /**
     * @brief Constructor
     * @details Character constructor
     *
     * @param c Character
     */
    explicit StringImpl(char c) : mpBuffer(nullptr), mCapacity(0), mLength(0) {
        Assign(c);
    }

    /**
     * @brief Constructor
     * @details Reserve space
     *
     * @param n Number of characters to reserve
     */
    explicit StringImpl(u32 n) : mpBuffer(nullptr), mCapacity(0), mLength(0) {
        Reserve(n);
    }

    /**
     * @brief Destructor
     */
    ~StringImpl() {
        // Don't delete static memory
        if (mpBuffer == scEmptyCStr) {
            return;
        }

        delete[] mpBuffer;
        mpBuffer = nullptr;
    }

    /**
     * @brief Implicit conversion operator to C-style string
     */
    operator const T*() const {
        return CStr();
    }

    /**
     * @brief Gets the length of the underlying string
     * @note Null terminator is not included in the length
     */
    u32 Length() const {
        return mLength;
    }

    /**
     * @brief Tests whether the string is empty
     */
    bool Empty() const {
        return Length() == 0;
    }

    /**
     * @brief Gets the underlying C-style string
     */
    const T* CStr() const {
        return mpBuffer;
    }

    /**
     * @brief Accesses a character in the string
     *
     * @param i Character index
     * @return Reference to character
     */
    T& operator[](u32 i) {
        K_ASSERT(i < mLength);
        return mpBuffer[i];
    }

    /**
     * @brief Accesses a character in the string
     *
     * @param i Character index
     * @return Reference to character
     */
    const T& operator[](u32 i) const {
        K_ASSERT(i < mLength);
        return mpBuffer[i];
    }

    /**
     * @brief Clears string data, but does not change the buffer size
     */
    void Clear() {
        mLength = 0;
    }

    /**
     * @brief Reserves string buffer of specified size
     *
     * @param n Number of characters to reserve (ignoring null terminator)
     */
    void Reserve(u32 n);
    /**
     * @brief Shrinks buffer to fit string contents
     */
    void Shrink();

    /**
     * @brief Generates substring of this string
     *
     * @param pos Substring start position
     * @param len Substring size
     */
    StringImpl SubStr(u32 pos = 0, u32 len = npos) const;

    /**
     * @brief Finds first occurrence of sequence in string
     *
     * @param rStr Sequence to search for
     * @param pos Search offset (from string start)
     * @return Match position if found, otherwise npos
     */
    u32 Find(const StringImpl& rStr, u32 pos = 0) const;
    /**
     * @brief Finds first occurrence of sequence in string
     *
     * @param pStr Sequence to search for
     * @param pos Search offset (from string start)
     * @return Match position if found, otherwise npos
     */
    u32 Find(const T* pStr, u32 pos = 0) const;
    /**
     * @brief Finds first occurrence of sequence in string
     *
     * @param c Character to search for
     * @param pos Search offset (from string start)
     * @return Match position if found, otherwise npos
     */
    u32 Find(T c, u32 pos = 0) const;

    /**
     * @brief Tests whether this string starts with the specified prefix
     *
     * @param rStr Prefix sequence
     */
    bool StartsWith(const StringImpl& rStr) const;
    /**
     * @brief Tests whether this string starts with the specified prefix
     *
     * @param pStr Prefix sequence
     */
    bool StartsWith(const T* pStr) const;

    /**
     * @brief Tests whether this string ends with the specified suffix
     *
     * @param rStr Suffix sequence
     */
    bool EndsWith(const StringImpl& rStr) const;
    /**
     * @brief Tests whether this string ends with the specified suffix
     *
     * @param pStr Suffix sequence
     */
    bool EndsWith(const T* pStr) const;

    /**
     * @brief Split this string into tokens by the specified delimiter
     *
     * @param rDelim Delimiter sequence
     */
    TVector<StringImpl> Split(const StringImpl& rDelim) const;

    // clang-format off
    StringImpl& operator=(const StringImpl& rStr) { Assign(rStr); return *this; }
    StringImpl& operator=(const T* pStr)          { K_ASSERT(pStr != nullptr); Assign(pStr); return *this; }
    StringImpl& operator=(T c)                    { Assign(c); return *this; }

    StringImpl& operator+=(const StringImpl& rStr) { Append(rStr); return *this; }
    StringImpl& operator+=(const T* pStr)          { K_ASSERT(pStr != nullptr); Append(pStr); return *this; }
    StringImpl& operator+=(T c)                    { Append(c); return *this; }

    bool operator==(const StringImpl& rStr) const;
    bool operator==(const T* pStr) const;
    bool operator==(const T c) const { return mLength == 1 && mpBuffer[0] == c; }

    bool operator!=(const StringImpl& rStr) const { return (*this == rStr) == false; }
    bool operator!=(const T* pStr) const          { return (*this == pStr) == false; }
    bool operator!=(T c) const                    { return (*this == c)    == false; }
    // clang-format on

    friend StringImpl operator+(const StringImpl& lhs, const StringImpl& rhs) {
        StringImpl str = lhs;
        str += rhs;
        return str;
    }

    friend StringImpl operator+(const StringImpl& lhs, const T* rhs) {
        StringImpl str = lhs;
        str += rhs;
        return str;
    }

    friend StringImpl operator+(const StringImpl& lhs, T rhs) {
        StringImpl str = lhs;
        str += rhs;
        return str;
    }

private:
    /**
     * @brief Assigns data to string
     *
     * @param rStr String to copy
     */
    void Assign(const StringImpl& rStr);
    /**
     * @brief Assigns data to string
     *
     * @param pStr C-style string to copy
     * @param n Number of characters to copy
     */
    void Assign(const T* pStr, u32 n = npos);
    /**
     * @brief Assigns data to string
     *
     * @param c Character to write
     */
    void Assign(T c);

    /**
     * @brief Appends a string to this string
     *
     * @param rStr String to append
     */
    void Append(const StringImpl& rStr);
    /**
     * @brief Appends a string to this string
     *
     * @param pStr C-style string to append
     */
    void Append(const T* pStr);
    /**
     * @brief Appends a character to this string
     *
     * @param c Character to append
     */
    void Append(T c);

private:
    T* mpBuffer;   // String buffer
    u32 mCapacity; // Buffer size
    u32 mLength;   // String length (not including null terminator)

    // Static string for empty String objects
    static const T* scEmptyCStr;

public:
    static const u32 npos = -1;
};

typedef StringImpl<char> String;
typedef StringImpl<wchar_t> WString;

/**
 * @brief Creates a new string from format arguments
 *
 * @param rFmt Format string
 * @param args Format arguments
 */
template <typename T>
K_INLINE StringImpl<T> VFormat(const StringImpl<T>& rFmt, std::va_list args) {
    char buffer[1024];
    std::vsnprintf(buffer, sizeof(buffer), rFmt, args);
    return StringImpl<T>(buffer);
}
/**
 * @brief Creates a new string from format arguments
 *
 * @param pFmt Format C-style string
 * @param args Format arguments
 */
template <typename T>
K_INLINE StringImpl<T> VFormat(const T* pFmt, std::va_list args) {
    char buffer[1024];
    std::vsnprintf(buffer, sizeof(buffer), pFmt, args);
    return StringImpl<T>(buffer);
}

/**
 * @brief Creates a new string from format arguments
 *
 * @param rFmt Format string
 * @param ... Format arguments
 */
template <typename T>
K_INLINE StringImpl<T> Format(const StringImpl<T>& rFmt, ...) {
    std::va_list list;
    va_start(list, rFmt);
    StringImpl<T> str = VFormat(rFmt, list);
    va_end(list);

    return str;
}
/**
 * @brief Creates a new string from format arguments
 *
 * @param pFmt Format C-style string
 * @param ... Format arguments
 */
template <typename T> K_INLINE StringImpl<T> Format(const T* pFmt, ...) {
    std::va_list list;
    va_start(list, pFmt);
    StringImpl<T> str = VFormat(pFmt, list);
    va_end(list);

    return str;
}

/**
 * @brief Hashes a key of any type
 * @note Hash support for String types
 *
 * @param rKey Key
 */
template <typename T> K_INLINE hash_t Hash(const StringImpl<T>& rKey) {
    return HashImpl(rKey.CStr(), rKey.Length() * sizeof(T));
}

#define TO_STRING_PRIM(T, pFmt, val)                                           \
    K_INLINE String ToString(const T& x) {                                     \
        return Format(pFmt, val);                                              \
    }
#define TO_HEX_STRING_PRIM(T, pFmt, val)                                       \
    K_INLINE String ToHexString(const T& x) {                                  \
        return Format(pFmt, val);                                              \
    }

/**
 * @name Integer conversion
 */
/**@{*/
TO_STRING_PRIM(int, "%d", x);
TO_STRING_PRIM(s32, "%ld", x);
TO_STRING_PRIM(s64, "%lld", x);
TO_HEX_STRING_PRIM(int, "0x%08X", x);
TO_HEX_STRING_PRIM(s32, "0x%08X", x);
TO_HEX_STRING_PRIM(s64, "0x%016X", x);

TO_STRING_PRIM(unsigned int, "%u", x);
TO_STRING_PRIM(u32, "%lu", x);
TO_STRING_PRIM(u64, "%llu", x);
TO_HEX_STRING_PRIM(unsigned int, "0x%08X", x);
TO_HEX_STRING_PRIM(u32, "0x%08X", x);
TO_HEX_STRING_PRIM(u64, "0x%016X", x);
/**@}*/

/**
 * @name Decimal conversion
 */
/**@{*/
TO_STRING_PRIM(f32, "%f", x);
TO_STRING_PRIM(f64, "%f", x);
TO_HEX_STRING_PRIM(f32, "0x%08X", BitCast<u32>(x));
TO_HEX_STRING_PRIM(f64, "0x%016X", BitCast<u64>(x));
/**@}*/

/**
 * @name Boolean conversion
 */
/**@{*/
K_INLINE String ToString(bool x) {
    return x ? "true" : "false";
}
K_INLINE String ToHexString(bool x) {
    return x ? "0x01" : "0x00";
}
/**@}*/

/**
 * @name String(!) conversion
 */
/**@{*/
K_INLINE String ToString(const String& x) {
    return x;
}
K_INLINE String ToHexString(const String& x) {
    K_ASSERT_EX(false, "Please reconsider...");
    return x;
}
K_INLINE String ToString(char x) {
    return String(x);
}
K_INLINE String ToString(const char* x) {
    return String(x);
}
K_INLINE String ToHexString(const char* x) {
    K_ASSERT_EX(false, "Please reconsider...");
    return String(x);
}
/**@}*/

/**
 * @brief Placeholder string conversion for unsupported types
 * @note Specialize this for your own types
 *
 * @param x Value to convert
 */
template <typename T> K_INLINE String ToString(const T& x) {
    return Format("<object at %p>", &x);
}

#undef TO_STRING_PRIM
#undef TO_HEX_STRING_PRIM

//! @}
} // namespace kiwi

#endif
