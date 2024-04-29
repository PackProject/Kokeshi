#ifndef LIBKIWI_PRIM_STRING_H
#define LIBKIWI_PRIM_STRING_H
#include <libkiwi/prim/kiwiBitCast.h>
#include <libkiwi/prim/kiwiHashMap.h>
#include <types.h>

namespace kiwi {

/**
 * String wrapper
 */
template <typename T> class StringImpl {
public:
    /**
     * Constructor
     */
    StringImpl() : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Clear();
    }

    /**
     * Constructor
     * @details Copy constructor
     *
     * @param str String to copy
     */
    StringImpl(const StringImpl& str)
        : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Assign(str);
    }

    /**
     * Constructor
     * @details Substring constructor
     *
     * @param str String to copy
     * @param pos Substring start index
     * @param len Substring length
     */
    StringImpl(const StringImpl& str, u32 pos, u32 len = npos)
        : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Assign(str.Substr(pos, len));
    }

    /**
     * Constructor
     * @details C-style string constructor
     *
     * @param s C-style string
     */
    StringImpl(const T* s) : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Assign(s);
    }

    /**
     * Constructor
     * @details Buffer/sequence constructor
     *
     * @param s Buffer/sequence
     * @param n Number of characters to copy
     */
    StringImpl(const T* s, u32 n) : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Assign(s, n);
    }

    /**
     * Constructor
     * @details Character constructor
     *
     * @param c Character
     */
    StringImpl(char c) : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Assign(c);
    }

    /**
     * Constructor
     * @details Reserve space
     *
     * @param n Number of characters to reserve
     */
    explicit StringImpl(u32 n) : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Reserve(n);
    }

    /**
     * Destructor
     */
    ~StringImpl() {
        // Don't delete static memory
        if (mpBuffer == scEmptyCStr) {
            return;
        }

        delete mpBuffer;
        mpBuffer = NULL;
    }

    /**
     * Implicit conversion operator to C-style string
     */
    operator const T*() const {
        return CStr();
    }

    /**
     * Gets the length of the underlying string (not including term)
     */
    u32 Length() const {
        return mLength;
    }

    /**
     * Tests whether the string is empty
     */
    bool Empty() const {
        return Length() == 0;
    }

    /**
     * Gets the underlying C-style string
     */
    const T* CStr() const {
        return mpBuffer;
    }

    /**
     * Accesses a character in the string
     *
     * @param i Character index
     * @return Reference to character
     */
    T& operator[](u32 i) {
        K_ASSERT(i < mLength);
        return mpBuffer[i];
    }

    /**
     * Accesses a character in the string
     *
     * @param i Character index
     * @return Reference to character
     */
    const T& operator[](u32 i) const {
        K_ASSERT(i < mLength);
        return mpBuffer[i];
    }

    void Clear();
    StringImpl Substr(u32 pos = 0, u32 len = npos) const;

    u32 Find(const StringImpl& str, u32 pos = 0) const;
    u32 Find(const T* s, u32 pos = 0) const;
    u32 Find(T c, u32 pos = 0) const;

    StringImpl& operator+=(const StringImpl& str) {
        Append(str);
        return *this;
    }
    StringImpl& operator+=(const T* s) {
        K_ASSERT(s != NULL);
        Append(s);
        return *this;
    }
    StringImpl& operator+=(T c) {
        Append(c);
        return *this;
    }

    StringImpl& operator=(const StringImpl& str) {
        Assign(str);
        return *this;
    }
    StringImpl& operator=(const T* s) {
        Assign(s);
        return *this;
    }
    StringImpl& operator=(T c) {
        Assign(c);
        return *this;
    }

    bool operator==(const StringImpl& str) const;
    bool operator==(const T* s) const;
    bool operator==(const T c) const {
        return mLength == 1 && mpBuffer[0] == c;
    }

    bool operator!=(const StringImpl& str) const {
        return (*this == str) == false;
    }
    bool operator!=(const T* s) const {
        return (*this == s) == false;
    }
    bool operator!=(T c) const {
        return (*this == c) == false;
    }

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
    void Reserve(u32 n);
    void Shrink();

    void Assign(const StringImpl& str);
    void Assign(const T* s, u32 n = npos);
    void Assign(T c);

    void Append(const StringImpl& str);
    void Append(const T* s);
    void Append(T c);

private:
    // String buffer
    T* mpBuffer;
    // Allocated buffer size
    u32 mCapacity;
    // String length
    u32 mLength;

    // Static string for empty StringImpls
    static const T* scEmptyCStr;

public:
    static const u32 npos = -1;
};

typedef StringImpl<char> String;
typedef StringImpl<wchar_t> WString;

/**
 * Creates a new string from format arguments
 *
 * @param fmt Format string
 * @param args Format arguments
 */
template <typename T>
inline StringImpl<T> VFormat(const StringImpl<T>& fmt, std::va_list args) {
    char buffer[1024];
    std::vsnprintf(buffer, sizeof(buffer), fmt, args);
    return StringImpl<T>(buffer);
}

/**
 * Creates a new string from format arguments
 *
 * @param fmt Format C-style string
 * @param args Format arguments
 */
template <typename T>
inline StringImpl<T> VFormat(const T* fmt, std::va_list args) {
    char buffer[1024];
    std::vsnprintf(buffer, sizeof(buffer), fmt, args);
    return StringImpl<T>(buffer);
}

/**
 * Creates a new string from format arguments
 *
 * @param fmt Format string
 * @param ... Format arguments
 */
template <typename T>
inline StringImpl<T> Format(const StringImpl<T>& fmt, ...) {
    std::va_list list;
    va_start(list, fmt);
    StringImpl<T> str = VFormat(fmt, list);
    va_end(list);

    return str;
}

/**
 * Creates a new string from format arguments
 *
 * @param fmt Format C-style string
 * @param ... Format arguments
 */
template <typename T> inline StringImpl<T> Format(const T* fmt, ...) {
    std::va_list list;
    va_start(list, fmt);
    StringImpl<T> str = VFormat(fmt, list);
    va_end(list);

    return str;
}

/**
 * @brief String hash support
 */
template <typename T> inline hash_t Hash(const StringImpl<T>& key) {
    return HashImpl(key.CStr(), key.Length() * sizeof(T));
}

// String conversion. Specialize these for custom types
#define K_TO_STRING_FMT_DEF(T, fmt, val)                                       \
    inline String ToString(const T& t) {                                       \
        return Format(fmt, val);                                               \
    }
#define K_TO_HEX_STRING_FMT_DEF(T, fmt, val)                                   \
    inline String ToHexString(const T& t) {                                    \
        return Format(fmt, val);                                               \
    }

/**
 * @brief Convert integer to string
 */
K_TO_STRING_FMT_DEF(int, "%d", t);
K_TO_STRING_FMT_DEF(s32, "%ld", t);
K_TO_STRING_FMT_DEF(s64, "%lld", t);
K_TO_HEX_STRING_FMT_DEF(int, "0x%08X", t);
K_TO_HEX_STRING_FMT_DEF(s32, "0x%08X", t);
K_TO_HEX_STRING_FMT_DEF(s64, "0x%016X", t);

K_TO_STRING_FMT_DEF(unsigned int, "%u", t);
K_TO_STRING_FMT_DEF(u32, "%lu", t);
K_TO_STRING_FMT_DEF(u64, "%llu", t);
K_TO_HEX_STRING_FMT_DEF(unsigned int, "0x%08X", t);
K_TO_HEX_STRING_FMT_DEF(u32, "0x%08X", t);
K_TO_HEX_STRING_FMT_DEF(u64, "0x%016X", t);

/**
 * @brief Convert decimal to string
 */
K_TO_STRING_FMT_DEF(f32, "%f", t);
K_TO_STRING_FMT_DEF(f64, "%f", t);
K_TO_HEX_STRING_FMT_DEF(f32, "0x%08X", BitCast<u32>(t));
K_TO_HEX_STRING_FMT_DEF(f64, "0x%016X", BitCast<u64>(t));

/**
 * @brief Convert boolean to string
 */
inline String ToString(bool t) {
    return t ? "true" : "false";
}
inline String ToHexString(bool t) {
    return t ? "0x01" : "0x00";
}

/**
 * @brief Convert string to string :D
 */
inline String ToString(const String& t) {
    return t;
}
inline String ToHexString(const String& t) {
    K_ASSERT_EX(false, "Please reconsider...");
    return t;
}
inline String ToString(char c) {
    return String(c);
}
inline String ToString(const char* t) {
    return String(t);
}
inline String ToHexString(const char* t) {
    K_ASSERT_EX(false, "Please reconsider...");
    return String(t);
}

/**
 * @brief Placeholder string conversion
 */
template <typename T> inline String ToString(const T& t) {
    return Format("<object at %p>", &t);
}

#undef K_TO_STRING_FMT_DEF
#undef K_TO_HEX_STRING_FMT_DEF

} // namespace kiwi

#endif
