#include <libkiwi.h>

#include <cstdio>
#include <cstring>
#include <cwchar>

namespace ksl {

/**
 * @brief Tests whether character is a decimal digit
 *
 * @param c Character
 */
int isdigit(char c) {
    return c >= '0' && c <= '9';
}

/**
 * @brief Tests whether character is alphabetic
 *
 * @param c Character
 */
int isalpha(char c) {
    return isupper(c) || islower(c);
}

/**
 * @brief Tests whether character is uppercase
 *
 * @param c Character
 */
int isupper(char c) {
    return c >= 'A' && c <= 'Z';
}

/**
 * @brief Tests whether character is lowercase
 *
 * @param c Character
 */
int islower(char c) {
    return c >= 'a' && c <= 'z';
}

/**
 * @brief Finds first occurrence of the specified character in the string
 *
 * @param pStr String to search
 * @param c Character to search for
 */
char* strchr(const char* pStr, char c) {
    if (pStr == nullptr) {
        return nullptr;
    }

    for (; *pStr != '\0'; pStr++) {
        if (*pStr == c) {
            return const_cast<char*>(pStr);
        }
    }

    return nullptr;
}

/**
 * @brief Finds the first occurrence of a sequence in a string
 *
 * @param pStr String to search
 * @param pSeq Sequence to search for
 */
const char* strstr(const char* pStr, const char* pSeq) {
    K_ASSERT(pStr != nullptr);

    // No sequence/empty sequence
    if (pSeq == nullptr || *pSeq == L'\0') {
        return pStr;
    }

    // First character in sequence
    char begin = *pSeq;

    // Check for sequence
    while (*pStr != L'\0') {
        // Matches beginning of sequence
        if (*pStr == begin) {
            // Backup original pointers
            const char* pCheck = pStr;
            const char* pExpect = pSeq;

            // Look for rest of sequence
            while (*++pCheck == *++pExpect) {
                ;
            }

            // Did we hit the end?
            if (*pExpect == '\0') {
                return pStr;
            }
        }

        // Next character
        pStr++;
    }

    return nullptr;
}

/**
 * @brief Determines string length (up to the specified limit)
 * @note Null terminator not included
 *
 * @param pStr String to check
 * @param maxlen Maximum length
 */
size_t strnlen(const char* pStr, size_t maxlen) {
    size_t len = 0;

    if (pStr == nullptr) {
        return 0;
    }

    for (; len < maxlen; len++) {
        if (*pStr++ == '\0') {
            break;
        }
    }

    return len;
}

/**
 * @brief Converts string to signed long
 *
 * @param pStr String to convert
 * @param[out] pEndPtr Pointer to string data after the converted number
 * @param base Number base (specify 0 to auto-detect)
 */
s32 strtol(const char* pStr, char** pEndPtr, int base) {
    K_ASSERT(pStr != nullptr);

    // Trim leading whitespace
    while (*pStr == ' ') {
        pStr++;
    }

    if (*pStr == '\0') {
        return 0;
    }

    // Sign (positive by default)
    bool positive = true;
    if (*pStr == '-') {
        positive = false;
        pStr++;
    }
    // Can still specify '+' though
    else if (*pStr == '+') {
        pStr++;
    }

    // Check prefix
    char c0 = *pStr;
    char c1 = *pStr + 1;

    // Auto-detect base
    // If base is octal/hex, just check for optional prefix
    if (base == 0 || base == 16) {
        // Valid hex prefix
        if (c0 == '0' && (c1 == 'x' || c1 == 'X')) {
            base = 16;
            pStr += sizeof('0x');
        }
    } else if (base == 0 || base == 8) {
        // Valid octal prefix
        if (c0 == '0') {
            base = 8;
            pStr += sizeof('0');
        }
    }

    // Assume decimal
    if (base == 0) {
        base = 10;
    }

    K_ASSERT_EX(2 <= base <= 36, "Invalid base");

    // Parse digits
    s32 ret = 0;
    for (; *pStr != '\0'; pStr++) {
        char digit = *pStr;

        if (isdigit(digit)) {
            ret *= base;
            ret += digit - '0';
        } else if (isalpha(digit)) {
            K_ASSERT(base > 10);

            // Uppercase
            if (isupper(digit)) {
                ret *= base;
                ret += 0xA + digit - 'A';
            }
            // Lowercase
            else if (islower(digit)) {
                ret *= base;
                ret += 0xA + digit - 'a';
            }
        }
        // Conversion end
        else {
            if (pEndPtr != nullptr) {
                *pEndPtr = const_cast<char*>(pStr);
            }
            break;
        }
    }

    return ret;
}

/**
 * @brief Converts string to unsigned long
 *
 * @param pStr String to convert
 * @param[out] pEndPtr Pointer to string data after the converted number
 * @param base Number base (specify 0 to auto-detect)
 */
u32 strtoul(const char* pStr, char** pEndPtr, int base) {
    return static_cast<u32>(strtol(pStr, pEndPtr, base));
}

/**
 * @brief Converts string to double value
 *
 * @param pStr String to convert
 */
f64 atof(const char* pStr) {
    K_ASSERT(pStr != nullptr);

    // Skip whitespace
    while (*pStr == ' ') {
        ++pStr;
    }

    // Convert value
    f64 value;
    int num = std::sscanf(pStr, "%lf", &value);

    // Failure -> zero
    if (num != 1) {
        return 0.0l;
    }

    return value;
}

/**
 * @brief Concatenates two wide-char strings (up to the specified character
 * limit)
 *
 * @param pwDst String to concatenate to
 * @param pwSrc String to concatenate from
 * @param maxlen Maximum number of characters to concatenate
 */
wchar_t* wcsncat(wchar_t* pwDst, const wchar_t* pwSrc, size_t maxlen) {
    K_ASSERT(pwDst != nullptr);
    K_ASSERT(pwSrc != nullptr);

    // Backup original argument
    wchar_t* pBackup = pwDst;

    // Find null terminator
    while (*pwDst != L'\0') {
        pwDst++;
    }

    // Append sequence
    while (maxlen-- > 0) {
        *pwDst++ = *pwSrc++;

        // Hit end of sequence
        if (*pwSrc == L'\0') {
            break;
        }
    }

    // Null terminator
    *pwDst = L'\0';

    return pBackup;
}

/**
 * @brief Compares two wide-char strings (up to the specified character limit)
 *
 * @param pwStr1 String 1
 * @param pwStr2 String 2
 * @param maxlen Maximum number of characters to compare
 */
int wcsncmp(const wchar_t* pwStr1, const wchar_t* pwStr2, size_t maxlen) {
    K_ASSERT(pwStr1 != nullptr);
    K_ASSERT(pwStr2 != nullptr);

    for (int i = 0; i < maxlen; i++) {
        if (*pwStr1 != *pwStr2) {
            return *pwStr1 - *pwStr2;
        }

        if (*pwStr1 == L'\0') {
            return 0;
        }

        pwStr1++;
        pwStr2++;
    }

    return 0;
}

/**
 * @brief Finds the first occurrence of a sequence in a wide-char string
 *
 * @param pwStr String to search
 * @param pwSeq Sequence to search for
 */
const wchar_t* wcsstr(const wchar_t* pwStr, const wchar_t* pwSeq) {
    K_ASSERT(pwStr != nullptr);

    // No sequence/empty sequence
    if (pwSeq == nullptr || *pwSeq == L'\0') {
        return pwStr;
    }

    // First character in sequence
    wchar_t begin = *pwSeq;

    // Check for sequence
    while (*pwStr != L'\0') {
        // Matches beginning of sequence
        if (*pwStr == begin) {
            // Backup original pointers
            const wchar_t* pCheck = pwStr;
            const wchar_t* pExpect = pwSeq;

            // Look for rest of sequence
            while (*++pCheck == *++pExpect) {
                ;
            }

            // Did we hit the end?
            if (*pExpect == '\0') {
                return pwStr;
            }
        }

        // Next character
        pwStr++;
    }

    return nullptr;
}

/******************************************************************************
 *
 * Formatter implementation
 *
 ******************************************************************************/
// Internal STL declarations
extern "C" {
struct FormatCtx {
    char* pwDst;   // at 0x0
    size_t maxlen; // at 0x4
    size_t pos;    // at 0x8
};

typedef void (*WriterFunc)(FormatCtx* pCtx, const char* pFmt, size_t length);

struct WideFormatCtx {
    wchar_t* pwDst; // at 0x0
    size_t maxlen;  // at 0x4
    size_t pos;     // at 0x8
};

typedef void (*WideWriterFunc)(WideFormatCtx* pCtx, const wchar_t* pwFmt,
                               size_t length);

int __wpformatter(WideWriterFunc pWriter, WideFormatCtx* pContext,
                  const wchar_t* pwFmt, std::va_list args);

void __wStringWrite(WideFormatCtx* pCtx, const wchar_t* pwFmt, size_t length);
}

/**
 * @brief Applies the specified arguments to the specified wide-char format
 * string, placing the result in the output buffer.
 *
 * @param[out] pwDst Destination buffer
 * @param maxlen Buffer size
 * @param pwFmt Format string
 * @param args Format arguments
 * @return int Number of characters written
 */
int vswprintf(wchar_t* pwDst, size_t maxlen, const wchar_t* pwFmt,
              std::va_list args) {

    WideFormatCtx context = {pwDst, maxlen};
    int n = __wpformatter(__wStringWrite, &context, pwFmt, args);

    if (n >= 0) {
        if (n < maxlen) {
            pwDst[n] = L'\0';
        } else {
            pwDst[maxlen - 1] = L'\0';
            return -1;
        }
    }

    return n;
}

} // namespace ksl
