#include <cstdio>
#include <libkiwi.h>

namespace ksl {

/**
 * Tests if character is a decimal digit
 *
 * @param c Character
 */
bool isdigit(char c) {
    return c >= '0' && c <= '9';
}

/**
 * Tests if character is alphabetic
 *
 * @param c Character
 */
bool isalpha(char c) {
    return isupper(c) || islower(c);
}

/**
 * Tests if character is uppercase
 *
 * @param c Character
 */
bool isupper(char c) {
    return c >= 'A' && c <= 'Z';
}

/**
 * Tests if character is lowercase
 *
 * @param c Character
 */
bool islower(char c) {
    return c >= 'a' && c <= 'z';
}

/**
 * Finds first occurrence of character in string
 *
 * @param str String to search
 * @param c Character to search for
 * @return Pointer to occurrence (NULL if not found)
 */
char* strchr(const char* str, char c) {
    if (str == NULL) {
        return NULL;
    }

    for (; *str != '\0'; str++) {
        if (*str == c) {
            return const_cast<char*>(str);
        }
    }

    return NULL;
}

/**
 * Gets string length, up to specified limit
 *
 * @param str String to check
 * @param maxlen Maximum length
 * @return Length of string (not including null terminator)
 */
size_t strnlen(const char* str, size_t maxlen) {
    size_t len = 0;

    if (str == NULL) {
        return 0;
    }

    for (; len < maxlen; len++) {
        if (*str++ == '\0') {
            break;
        }
    }

    return len;
}

/**
 * Converts string to signed long
 *
 * @param str String to convert
 * @param[out] endptr Pointer to string data after the converted number
 * @param base Number base (specify 0 to auto-detect)
 *
 * @copyright Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. [rescinded 22 July 1999]
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
s32 strtol(const char* str, char** endptr, int base) {
    K_ASSERT(str != NULL);

    // Trim leading whitespace
    while (*str == ' ') {
        if (*str == '\0') {
            return 0;
        }
        str++;
    }

    // Sign (positive by default)
    bool positive = true;
    if (*str == '-') {
        positive = false;
        str++;
    } else if (*str == '+') {
        str++;
    }

    // Check prefix
    char c0 = *str;
    char c1 = *str + 1;

    // Auto-detect base
    // If base is octal/hex, just check for optional prefix
    if (base == 0 || base == 16) {
        // Valid hex prefix
        if (c0 == '0' && (c1 == 'x' || c1 == 'X')) {
            base = 16;
            str += sizeof('0x');
        }
    } else if (base == 0 || base == 8) {
        // Valid octal prefix
        if (c0 == '0') {
            base = 8;
            str += sizeof('0');
        }
    }

    K_ASSERT_EX(base != 0, "Failed to auto-detect base");
    K_ASSERT_EX(2 <= base <= 36, "Invalid base");

    // Parse digits
    s32 ret = 0;
    for (; *str != '\0'; str++) {
        char digit = *str;

        if (isdigit(digit)) {
            ret *= base;
            ret += digit - '0';
        } else if (isalpha(digit)) {
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
            if (endptr != NULL) {
                *endptr = const_cast<char*>(str);
            }
            break;
        }
    }

    return ret;
}

/**
 * Converts string to unsigned long
 *
 * @param str String to convert
 * @param[out] endptr Pointer to string data after the converted number
 * @param base Number base (specify 0 to auto-detect)
 */
u32 strtoul(const char* str, char** endptr, int base) {
    return static_cast<u32>(strtol(str, endptr, base));
}

/**
 * Converts string to double value
 *
 * @param str String to convert
 * @return f64 Resulting value
 */
f64 atof(const char* str) {
    K_ASSERT(str != NULL);

    // Skip whitespace
    while (*str == ' ') {
        ++str;
    }

    // Convert value
    f64 value;
    int num = std::sscanf("%lf", str, &value);

    // Failure -> zero
    if (num != 1) {
        return 0.0l;
    }

    return value;
}

/**
 * @brief Sort helper
 *
 * @param ptr Array base
 * @param count Number of elements
 * @param size Element size
 * @param comp Compare function
 */
void qsort(void* ptr, u32 count, u32 size, QSortFunc comp) {
    // K_ASSERT(ptr != NULL);
    K_ASSERT(comp != NULL);

    // Nothing to sort
    if (count < 1) {
        return;
    }

    ;
}

/**
 * Concatenates wide-char strings, up to the specified limit of
 * characters
 *
 * @param dst String to concatenate to
 * @param src String to concatenate from
 * @param maxlen Concatenation limit, in bytes
 * @return wchar_t* Pointer to resulting string
 */
wchar_t* wcsncat(wchar_t* dst, const wchar_t* src, size_t maxlen) {
    // Backup original argument
    wchar_t* backup = dst;

    // Find null terminator
    while (*dst != L'\0') {
        dst++;
    }

    // Append sequence
    while (maxlen-- > 0) {
        *dst++ = *src++;

        // Hit end of sequence
        if (*src == L'\0') {
            break;
        }
    }

    // Null terminator
    *dst = L'\0';

    return backup;
}

/**
 * Compares two wide-char strings, up to the specified limit of
 * characters
 *
 * @param wcs1 String 1
 * @param wcs2 String 2
 * @param maxlen Maximum number of characters to compare
 * @return Relationship between the two strings
 */
int wcsncmp(const wchar_t* wcs1, const wchar_t* wcs2, size_t maxlen) {
    for (int i = 0; i < maxlen; i++) {
        if (*wcs1 != *wcs2) {
            return *wcs1 - *wcs2;
        }

        if (*wcs1 == L'\0') {
            return 0;
        }

        wcs1++;
        wcs2++;
    }

    return 0;
}

/**
 * Finds the first occurrence of a sequence in a string
 *
 * @param str String to search
 * @param seq Sequence to search for
 * @return Pointer to occurrence (NULL if not found)
 */
const wchar_t* wcsstr(const wchar_t* str, const wchar_t* seq) {
    // No sequence/empty sequence
    if (seq == NULL || *seq == L'\0') {
        return str;
    }

    // First character in sequence
    wchar_t begin = *seq;

    // Check for sequence
    while (*str != L'\0') {
        // Matches beginning of sequence
        if (*str == begin) {
            // Backup original pointers
            const wchar_t* check = str;
            const wchar_t* expect = seq;

            // Look for rest of sequence
            while (*++check == *++expect) {
                ;
            }

            // Did we hit the end?
            if (*expect == '\0') {
                return str;
            }
        }

        // Next character
        str++;
    }

    return NULL;
}

} // namespace ksl
