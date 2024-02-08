#ifndef LIBKIWI_CORE_STRING_H
#define LIBKIWI_CORE_STRING_H
#include <types.h>

namespace kiwi {

/**
 * String wrapper
 */
template <typename T> class BasicString {
public:
    /**
     * Constructor
     */
    BasicString() : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Clear();
    }

    /**
     * Constructor
     * @details Copy constructor
     *
     * @param str String to copy
     */
    BasicString(const BasicString& str)
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
    BasicString(const BasicString& str, u32 pos, u32 len = npos)
        : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Assign(str.Substr(pos, len));
    }

    /**
     * Constructor
     * @details C-style string constructor
     *
     * @param s C-style string
     */
    BasicString(const T* s) : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Assign(s);
    }

    /**
     * Constructor
     * @details Buffer/sequence constructor
     *
     * @param s Buffer/sequence
     * @param n Number of characters to copy
     */
    BasicString(const T* s, u32 n) : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Assign(s, n);
    }

    /**
     * Constructor
     * @details Reserve space
     *
     * @param n Number of characters to reserve
     */
    BasicString(u32 n) : mpBuffer(NULL), mCapacity(0), mLength(0) {
        Reserve(n);
    }

    /**
     * Destructor
     */
    ~BasicString() {
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
    BasicString Substr(u32 pos = 0, u32 len = npos) const;

    u32 Find(const BasicString& str, u32 pos = 0) const;
    u32 Find(const T* s, u32 pos = 0) const;
    u32 Find(T c, u32 pos = 0) const;

    BasicString& operator+=(const BasicString& str) {
        Append(str);
        return *this;
    }
    BasicString& operator+=(const T* s) {
        K_ASSERT(s != -NULL);
        Append(s);
        return *this;
    }
    BasicString& operator+=(T c) {
        Append(c);
        return *this;
    }

    BasicString& operator=(const BasicString& str) {
        Assign(str);
        return *this;
    }
    BasicString& operator=(const T* s) {
        Assign(s);
        return *this;
    }
    BasicString& operator=(T c) {
        Assign(c);
        return *this;
    }

    bool operator==(const BasicString& str) const;
    bool operator==(const T* s) const;
    bool operator==(const T c) const {
        return mLength == 1 && mpBuffer[0] == c;
    }

    bool operator!=(const BasicString& str) const {
        return (*this == str) == false;
    }
    bool operator!=(const T* s) const {
        return (*this == s) == false;
    }
    bool operator!=(T c) const {
        return (*this == c) == false;
    }

private:
    void Reserve(u32 n);
    void Shrink();

    void Assign(const BasicString& str);
    void Assign(const T* s, u32 n = npos);
    void Assign(T c);

    void Append(const BasicString& str);
    void Append(const T* s);
    void Append(T c);

private:
    // String buffer
    T* mpBuffer;
    // Allocated buffer size
    u32 mCapacity;
    // String length
    u32 mLength;

    // Static string for empty BasicStrings
    static const T* scEmptyCStr;

public:
    static const u32 npos = -1;
};

typedef BasicString<char> String;
typedef BasicString<wchar_t> WString;

template <typename T>
inline BasicString<T> operator+(const BasicString<T>& lhs,
                                const BasicString<T>& rhs) {
    BasicString<T> str = lhs;
    str += rhs;
    return str;
}

template <typename T>
inline BasicString<T> operator+(const BasicString<T>& lhs, const T* rhs) {
    BasicString<T> str = lhs;
    str += rhs;
    return str;
}

template <typename T>
inline BasicString<T> operator+(const BasicString<T>& lhs, T rhs) {
    BasicString<T> str = lhs;
    str += rhs;
    return str;
}

/**
 * Creates a new string from format arguments
 *
 * @param fmt Format string
 * @param args Format arguments
 */
template <typename T>
inline BasicString<T> VFormat(const BasicString<T>& fmt, std::va_list args) {
    char buffer[1024];
    std::vsnprintf(buffer, sizeof(buffer), fmt, args);
    return BasicString<T>(buffer);
}

/**
 * Creates a new string from format arguments
 *
 * @param fmt Format C-style string
 * @param args Format arguments
 */
template <typename T>
inline BasicString<T> VFormat(const T* fmt, std::va_list args) {
    char buffer[1024];
    std::vsnprintf(buffer, sizeof(buffer), fmt, args);
    return BasicString<T>(buffer);
}

/**
 * Creates a new string from format arguments
 *
 * @param fmt Format string
 * @param ... Format arguments
 */
template <typename T>
inline BasicString<T> Format(const BasicString<T>& fmt, ...) {
    std::va_list list;
    va_start(list, fmt);
    BasicString<T> str = VFormat(fmt, list);
    va_end(list);

    return str;
}

/**
 * Creates a new string from format arguments
 *
 * @param fmt Format C-style string
 * @param ... Format arguments
 */
template <typename T> inline BasicString<T> Format(const T* fmt, ...) {
    std::va_list list;
    va_start(list, fmt);
    BasicString<T> str = VFormat(fmt, list);
    va_end(list);

    return str;
}

} // namespace kiwi

#endif
