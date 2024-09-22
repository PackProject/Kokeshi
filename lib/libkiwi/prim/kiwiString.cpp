#include <cstring>
#include <cwchar>
#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Static string for empty String objects
 */
template <> const char* StringImpl<char>::scEmptyCStr = "";
/**
 * @brief Static string for empty String objects
 */
template <> const wchar_t* StringImpl<wchar_t>::scEmptyCStr = L"";

namespace {

template <typename T> u32 StrLen(const T* pStr);
template <typename T> T* StrNCat(T* pDst, const T* pSrc, u32 n);
template <typename T> int StrNCmp(const T* pStr1, const T* pStr2, u32 n);
template <typename T> T* StrNCpy(T* pDst, const T* pSrc, u32 n);
template <typename T> const T* StrChr(const T* pStr, T c);
template <typename T> const T* StrStr(const T* pStr1, const T* pStr2);

} // namespace

/**
 * @brief Reserves string buffer of specified size
 *
 * @param n Number of characters to reserve (ignoring null terminator)
 */
template <typename T> void StringImpl<T>::Reserve(u32 n) {
    // Already have enough space
    if (mCapacity >= n + 1) {
        return;
    }

    // Reallocate buffer
    T* pBuffer = new T[n + 1];

    // Copy existing data
    StrNCpy(pBuffer, mpBuffer, mLength);
    pBuffer[mLength] = '\0';

    // Delete old data
    if (mpBuffer != scEmptyCStr) {
        delete[] mpBuffer;
    }

    // Set new configuration
    mpBuffer = pBuffer;
    mCapacity = n + 1;
}

/**
 * @brief Shrinks buffer to fit string contents
 */
template <typename T> void StringImpl<T>::Shrink() {
    K_ASSERT(mCapacity > Length());

    mCapacity = 0;
    Reserve(Length());
}

/**
 * @brief Generates substring of this string
 *
 * @param pos Substring start position
 * @param len Substring size
 */
template <typename T>
StringImpl<T> StringImpl<T>::SubStr(u32 pos, u32 len) const {
    K_ASSERT(pos <= mLength);

    // Empty string if the substring begins at the end
    if (pos == mLength) {
        return StringImpl();
    }

    // Clamp substring length
    len = Min(len, mLength - pos);

    // Create C-style substring
    T* pBuffer = new T[len + 1];
    StrNCpy(pBuffer, mpBuffer + pos, len);
    pBuffer[len] = '\0';

    // Construct string wrapper
    StringImpl<T> str = StringImpl(pBuffer);

    delete[] pBuffer;
    return str;
}

/**
 * @brief Finds first occurrence of sequence in string
 *
 * @param rStr Sequence to search for
 * @param pos Search offset (from string start)
 * @return Match position if found, otherwise npos
 */
template <typename T>
u32 StringImpl<T>::Find(const StringImpl<T>& rStr, u32 pos) const {
    // Cannot match empty string
    if (rStr.Empty()) {
        return npos;
    }

    // Cannot match past end of string
    if (pos >= mLength) {
        return npos;
    }

    return Find(rStr.CStr(), pos);
}

/**
 * @brief Finds first occurrence of sequence in string
 *
 * @param pStr Sequence to search for
 * @param pos Search offset (from string start)
 * @return Match position if found, otherwise npos
 */
template <typename T> u32 StringImpl<T>::Find(const T* pStr, u32 pos) const {
    K_ASSERT(pStr != nullptr);

    // Cannot match past end of string
    if (pos >= mLength) {
        return npos;
    }

    // Call down to C STL
    const T* pResult = StrStr(mpBuffer + pos, pStr);

    // Not found
    if (pResult == nullptr) {
        return npos;
    }

    // Determine index by pointer arithmetic
    return PtrDistance(mpBuffer, pResult) / sizeof(T);
}

/**
 * @brief Finds first occurrence of sequence in string
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
    const T* pResult = StrChr(mpBuffer + pos, c);

    // Not found
    if (pResult == nullptr) {
        return npos;
    }

    // Determine index by pointer arithmetic
    return PtrDistance(mpBuffer, pResult) / sizeof(T);
}

/**
 * @brief Tests whether this string starts with the specified prefix
 *
 * @param rStr Prefix sequence
 */
template <typename T>
bool StringImpl<T>::StartsWith(const StringImpl<T>& rStr) const {
    return Find(rStr) == 0;
}

/**
 * @brief Tests whether this string starts with the specified prefix
 *
 * @param pStr Prefix sequence
 */
template <typename T> bool StringImpl<T>::StartsWith(const T* pStr) const {
    K_ASSERT(pStr != nullptr);
    return Find(pStr) == 0;
}

/**
 * @brief Tests whether this string ends with the specified suffix
 *
 * @param rStr Suffix sequence
 */
template <typename T>
bool StringImpl<T>::EndsWith(const StringImpl<T>& rStr) const {
    size_t pos = mLength - rStr.Length();
    return Find(rStr, pos) == pos;
}

/**
 * @brief Tests whether this string ends with the specified suffix
 *
 * @param pStr Suffix sequence
 */
template <typename T> bool StringImpl<T>::EndsWith(const T* pStr) const {
    K_ASSERT(pStr != nullptr);

    size_t pos = mLength - StrLen(pStr);
    return Find(pStr, pos) == pos;
}

/**
 * @brief Split this string into tokens by the specified delimiter
 *
 * @param rDelim Delimiter sequence
 */
template <typename T>
TVector<StringImpl<T> > StringImpl<T>::Split(const StringImpl& rDelim) const {
    K_ASSERT(rDelim.Length() > 0);

    TVector<StringImpl> tokens;

    // Search window
    u32 start = 0;
    u32 end = 0;

    while (start < mLength) {
        // Next occurrence in search window
        end = Find(rDelim, start);

        // No more occurrences in the string
        if (end == npos) {
            break;
        }

        // Split off token
        tokens.PushBack(SubStr(start, end - start));
        // Search window now ignores previous characters
        start = end + rDelim.Length();
    }

    // Push back very last token
    tokens.PushBack(SubStr(start));
    return tokens;
}

/**
 * @brief Tests for equality between string and specified data
 *
 * @param rStr String to compare against
 */
template <typename T>
bool StringImpl<T>::operator==(const StringImpl<T>& rStr) const {
    // Don't bother comparing data if lengths are different
    if (mLength != rStr.Length()) {
        return false;
    }

    // Compare string data
    return StrNCmp(mpBuffer, rStr.CStr(), mLength) == 0;
}

/**
 * @brief Tests for equality between string and specified data
 *
 * @param pStr C-style string to compare against
 */
template <typename T> bool StringImpl<T>::operator==(const T* pStr) const {
    K_ASSERT(pStr != nullptr);

    // Compare string data
    return StrNCmp(mpBuffer, pStr, mLength) == 0;
}

/**
 * @brief Assigns data to string
 *
 * @param rStr String to copy
 */
template <typename T> void StringImpl<T>::Assign(const StringImpl<T>& rStr) {
    // Reserve string buffer
    Reserve(rStr.Length());

    // Copy data
    StrNCpy(mpBuffer, rStr.CStr(), rStr.Length());
    mLength = rStr.Length();

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

/**
 * @brief Assigns data to string
 *
 * @param pStr C-style string to copy
 * @param n Number of characters to copy
 */
template <typename T> void StringImpl<T>::Assign(const T* pStr, u32 n) {
    K_ASSERT(pStr != nullptr);

    // Reserve string buffer
    u32 len = n != npos ? n : StrLen(pStr);
    Reserve(len);

    // Copy data
    StrNCpy(mpBuffer, pStr, len);
    mLength = len;

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

/**
 * @brief Assigns data to string
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
 * @brief Appends a string to this string
 *
 * @param rStr String to append
 */
template <typename T> void StringImpl<T>::Append(const StringImpl<T>& rStr) {
    // Reserve string buffer
    Reserve(mLength + rStr.Length());

    // Concatenate data
    StrNCat(mpBuffer, rStr.CStr(), rStr.Length());
    mLength += rStr.Length();

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

/**
 * @brief Appends a C-style string to this string
 *
 * @param pStr C-style string to append
 */
template <typename T> void StringImpl<T>::Append(const T* pStr) {
    // Reserve string buffer
    u32 len = StrLen(pStr);
    Reserve(mLength + len);

    // Concatenate data
    StrNCat(mpBuffer, pStr, len);
    mLength += len;

    // Null terminator
    mpBuffer[mLength] = static_cast<T>(0);
}

/**
 * @brief Appends a character to this string
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
template <> u32 StrLen<char>(const char* pStr) {
    return std::strlen(pStr);
}
template <> u32 StrLen<wchar_t>(const wchar_t* pStr) {
    return std::wcslen(pStr);
}

/**
 * strncat wrapper function
 */
template <> char* StrNCat(char* pDst, const char* pSrc, u32 n) {
    return std::strncat(pDst, pSrc, n);
}
template <> wchar_t* StrNCat(wchar_t* pDst, const wchar_t* pSrc, u32 n) {
    return ksl::wcsncat(pDst, pSrc, n);
}

/**
 * strncmp wrapper function
 */
template <> int StrNCmp<char>(const char* pStr1, const char* pStr2, u32 n) {
    return std::strncmp(pStr1, pStr2, n);
}
template <>
int StrNCmp<wchar_t>(const wchar_t* pStr1, const wchar_t* pStr2, u32 n) {
    return ksl::wcsncmp(pStr1, pStr2, n);
}

/**
 * strncpy wrapper function
 */
template <> char* StrNCpy<char>(char* pDst, const char* pSrc, u32 n) {
    return std::strncpy(pDst, pSrc, n);
}
template <>
wchar_t* StrNCpy<wchar_t>(wchar_t* pDst, const wchar_t* pSrc, u32 n) {
    return std::wcsncpy(pDst, pSrc, n);
}

/**
 * strchr wrapper function
 */
template <> const char* StrChr<char>(const char* pStr, char c) {
    return std::strchr(pStr, c);
}
template <> const wchar_t* StrChr<wchar_t>(const wchar_t* pStr, wchar_t c) {
    return std::wcschr(pStr, c);
}

/**
 * strstr wrapper function
 */
template <> const char* StrStr<char>(const char* pStr1, const char* pStr2) {
    return std::strstr(pStr1, pStr2);
}
template <>
const wchar_t* StrStr<wchar_t>(const wchar_t* pStr1, const wchar_t* pStr2) {
    return ksl::wcsstr(pStr1, pStr2);
}

} // namespace

// Instantiate supported string types
template class StringImpl<char>;
template class StringImpl<wchar_t>;

} // namespace kiwi
