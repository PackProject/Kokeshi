#include <cstring>
#include <cwchar>
#include <libkiwi.h>

namespace kiwi {

template <> const char* StringImpl<char>::scEmptyCStr = "";
template <> const wchar_t* StringImpl<wchar_t>::scEmptyCStr = L"";

/**
 * char/wchar_t template helper functions
 */
namespace {

template <typename T> u32 StrLen(const T* s);
template <typename T> T* StrNCat(T* dst, const T* src, u32 n);
template <typename T> int StrNCmp(const T* s1, const T* s2, u32 n);
template <typename T> T* StrNCpy(T* dst, const T* src, u32 n);
template <typename T> const T* StrChr(const T* s, T c);
template <typename T> const T* StrStr(const T* s1, const T* s2);

} // namespace

/**
 * Clears string buffer (yields empty string)
 */
template <typename T> void StringImpl<T>::Clear() {
    // Don't delete static memory
    if (mpBuffer == scEmptyCStr) {
        return;
    }

    // Delete string buffer
    delete mpBuffer;
    mpBuffer = NULL;

    // Update size/length
    mCapacity = 0;
    mLength = 0;

    // Set empty string
    mpBuffer = const_cast<T*>(scEmptyCStr);
}

/**
 * Generates substring of this string
 *
 * @param pos Substring start position
 * @param len Substring size
 */
template <typename T>
StringImpl<T> StringImpl<T>::Substr(u32 pos, u32 len) const {
    K_ASSERT(pos <= mLength);

    // Empty string if the substring begins at the end
    if (pos == mLength) {
        return StringImpl();
    }

    // Clamp substring length
    len = kiwi::Min(len, mLength - pos);

    // Create C-style substring
    T* buffer = new T[len + 1];
    StrNCpy(buffer, mpBuffer + pos, len);

    // Construct string wrapper
    StringImpl<T> str = StringImpl(buffer);

    delete buffer;
    return str;
}

/**
 * Finds first occurrence of sequence in string
 *
 * @param str Sequence to search for
 * @param pos Search offset (from string start)
 * @return Match position if found, otherwise npos
 */
template <typename T>
u32 StringImpl<T>::Find(const StringImpl<T>& str, u32 pos) const {
    // Cannot match empty string
    if (str.Empty()) {
        return npos;
    }

    // Cannot match past end of string
    if (pos >= mLength) {
        return npos;
    }

    return Find(str.CStr(), pos);
}

/**
 * Finds first occurrence of sequence in string
 *
 * @param s Sequence to search for
 * @param pos Search offset (from string start)
 * @return Match position if found, otherwise npos
 */
template <typename T> u32 StringImpl<T>::Find(const T* s, u32 pos) const {
    K_ASSERT(s != NULL);

    // Cannot match past end of string
    if (pos >= mLength) {
        return npos;
    }

    // Call down to C STL
    const T* result = StrStr(mpBuffer + pos, s);

    // Not found
    if (result == NULL) {
        return npos;
    }

    // Determine index by pointer arithmetic
    return kiwi::PtrDistance(mpBuffer, result) / sizeof(T);
}

/**
 * Finds first occurrence of sequence in string
 *
 * @param c Character to search for
 * @param pos Search offset (from string start)
 * @return Match position if found, otherwise npos
 */
template <typename T> u32 StringImpl<T>::Find(T c, u32 pos) const {
    // Cannot match past end of string
    if (pos >= mLength) {
        return npos;
    }

    // Call down to C STL
    const T* result = StrChr(mpBuffer + pos, c);

    // Not found
    if (result == NULL) {
        return npos;
    }

    // Determine index by pointer arithmetic
    return kiwi::PtrDistance(mpBuffer, result) / sizeof(T);
}

/**
 * Tests for equality between string and specified data
 *
 * @param str String to compare against
 */
template <typename T>
bool StringImpl<T>::operator==(const StringImpl<T>& str) const {
    // Don't bother comparing data if lengths are different
    if (mLength != str.Length()) {
        return false;
    }

    // Compare string data
    return StrNCmp(mpBuffer, str.CStr(), mLength) == 0;
}

/**
 * Tests for equality between string and specified data
 *
 * @param str C-style string to compare against
 */
template <typename T> bool StringImpl<T>::operator==(const T* s) const {
    K_ASSERT(s != NULL);

    // Compare string data
    return StrNCmp(mpBuffer, s, mLength) == 0;
}

/**
 * Reserves string buffer of specified size
 *
 * @param n Number of characters to reserve (ignoring null terminator)
 */
template <typename T> void StringImpl<T>::Reserve(u32 n) {
    // Already have enough space
    if (mCapacity >= n + 1) {
        return;
    }

    // Reallocate buffer
    T* buffer = new T[n + 1];

    // Copy existing data
    StrNCpy(buffer, mpBuffer, mLength);
    buffer[mLength] = '\0';

    // Delete old data
    if (mpBuffer != scEmptyCStr) {
        delete mpBuffer;
    }

    // Set new configuration
    mpBuffer = buffer;
    mCapacity = n + 1;
}

/**
 * Shrink buffer to fit string contents
 */
template <typename T> void StringImpl<T>::Shrink() {
    K_ASSERT(mCapacity > Length());

    mCapacity = 0;
    Reserve(Length());
}

/**
 * Assigns data to string
 * @note Data is copied, not owned
 *
 * @param str String to copy
 */
template <typename T> void StringImpl<T>::Assign(const StringImpl<T>& str) {
    // Reserve string buffer
    Reserve(str.Length());

    // Copy data
    StrNCpy(mpBuffer, str.CStr(), str.Length());
    mLength = str.Length();

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

/**
 * Assigns data to string
 * @note Data is copied, not owned
 *
 * @param s C-style string to copy
 * @param n Number of characters to copy
 */
template <typename T> void StringImpl<T>::Assign(const T* s, u32 n) {
    K_ASSERT(s != NULL);

    // Reserve string buffer
    u32 len = n != npos ? n : StrLen(s);
    Reserve(len);

    // Copy data
    StrNCpy(mpBuffer, s, len);
    mLength = len;

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

/**
 * Assigns data to string
 *
 * @param c Character to write
 */
template <typename T> void StringImpl<T>::Assign(T c) {
    // Reserve string buffer
    Reserve(1);

    // Copy data
    mpBuffer[0] = c;
    mLength = 1;

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

/**
 * Appends a string to this string
 *
 * @param str String to append
 */
template <typename T> void StringImpl<T>::Append(const StringImpl<T>& str) {
    // Reserve string buffer
    Reserve(mLength + str.Length());

    // Concatenate data
    StrNCat(mpBuffer, str.CStr(), str.Length());
    mLength += str.Length();

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

/**
 * Appends a C-style string to this string
 *
 * @param s C-style string to append
 */
template <typename T> void StringImpl<T>::Append(const T* s) {
    // Reserve string buffer
    u32 len = StrLen(s);
    Reserve(mLength + len);

    // Concatenate data
    StrNCat(mpBuffer, s, len);
    mLength += len;

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

/**
 * Appends a character to this string
 *
 * @param c Character to append
 */
template <typename T> void StringImpl<T>::Append(T c) {
    // Reserve string buffer
    Reserve(mLength + 1);

    // Concatenate data
    mpBuffer[mLength] = c;
    mLength++;

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

namespace {

/**
 * strlen wrapper function
 */
template <> u32 StrLen<char>(const char* s) {
    return std::strlen(s);
}
template <> u32 StrLen<wchar_t>(const wchar_t* s) {
    return std::wcslen(s);
}

/**
 * strncat wrapper function
 */
template <> char* StrNCat(char* dst, const char* src, u32 n) {
    return std::strncat(dst, src, n);
}
template <> wchar_t* StrNCat(wchar_t* dst, const wchar_t* src, u32 n) {
    return ksl::wcsncat(dst, src, n);
}

/**
 * strncmp wrapper function
 */
template <> int StrNCmp<char>(const char* s1, const char* s2, u32 n) {
    return std::strncmp(s1, s2, n);
}
template <> int StrNCmp<wchar_t>(const wchar_t* s1, const wchar_t* s2, u32 n) {
    return ksl::wcsncmp(s1, s2, n);
}

/**
 * strncpy wrapper function
 */
template <> char* StrNCpy<char>(char* dst, const char* src, u32 n) {
    return std::strncpy(dst, src, n);
}
template <> wchar_t* StrNCpy<wchar_t>(wchar_t* dst, const wchar_t* src, u32 n) {
    return std::wcsncpy(dst, src, n);
}

/**
 * strchr wrapper function
 */
template <> const char* StrChr<char>(const char* s, char c) {
    return std::strchr(s, c);
}
template <> const wchar_t* StrChr<wchar_t>(const wchar_t* s, wchar_t c) {
    return std::wcschr(s, c);
}

/**
 * strstr wrapper function
 */
template <> const char* StrStr<char>(const char* s1, const char* s2) {
    return std::strstr(s1, s2);
}
template <>
const wchar_t* StrStr<wchar_t>(const wchar_t* s1, const wchar_t* s2) {
    return ksl::wcsstr(s1, s2);
}

} // namespace

// Instantiate supported string types
template class StringImpl<char>;
template class StringImpl<wchar_t>;

} // namespace kiwi
