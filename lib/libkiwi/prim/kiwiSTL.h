#ifndef LIBKIWI_PRIM_STL_H
#define LIBKIWI_PRIM_STL_H
#include <libkiwi/k_types.h>

namespace ksl {

/**
 * @brief Tests whether character is a decimal digit
 *
 * @param c Character
 */
bool isdigit(char c);
/**
 * @brief Tests whether character is alphabetic
 *
 * @param c Character
 */
bool isalpha(char c);
/**
 * @brief Tests whether character is uppercase
 *
 * @param c Character
 */
bool isupper(char c);
/**
 * @brief Tests whether character is lowercase
 *
 * @param c Character
 */
bool islower(char c);

/**
 * @brief Finds first occurrence of the specified character in the string
 *
 * @param pStr String to search
 * @param c Character to search for
 */
char* strchr(const char* pStr, char c);

/**
 * @brief Finds the first occurrence of a sequence in a string
 *
 * @param pStr String to search
 * @param pSeq Sequence to search for
 */
const char* strstr(const char* pStr, const char* pSeq);

/**
 * @brief Determines string length (up to the specified limit)
 * @note Null terminator not included
 *
 * @param pStr String to check
 * @param maxlen Maximum length
 */
size_t strnlen(const char* pStr, size_t maxlen);

/**
 * @brief Converts string to signed long
 * @details Specify zero as the base to auto-detect it
 *
 * @param pStr String to convert
 * @param[out] pEndPtr Pointer to string data after the converted number
 * @param base Number base
 */
s32 strtol(const char* pStr, char** pEndPtr = nullptr, int base = 0);

/**
 * @brief Converts string to unsigned long
 * @details Specify zero as the base to auto-detect it
 *
 * @param pStr String to convert
 * @param[out] pEndPtr Pointer to string data after the converted number
 * @param base Number base
 */
u32 strtoul(const char* pStr, char** pEndPtr = nullptr, int base = 0);

/**
 * @brief Converts string to double value
 *
 * @param pStr String to convert
 */
f64 atof(const char* pStr);

/**
 * @brief Concatenates two wide-char strings (up to the specified character
 * limit)
 *
 * @param pwDst String to concatenate to
 * @param pwSrc String to concatenate from
 * @param maxlen Maximum number of characters to concatenate
 */
wchar_t* wcsncat(wchar_t* pwDst, const wchar_t* pwSrc, size_t maxlen);

/**
 * @brief Compares two wide-char strings (up to the specified character limit)
 *
 * @param pwStr1 String 1
 * @param pwStr2 String 2
 * @param maxlen Maximum number of characters to compare
 */
int wcsncmp(const wchar_t* pwStr1, const wchar_t* pwStr2, size_t maxlen);

/**
 * @brief Finds the first occurrence of a sequence in a wide-char string
 *
 * @param pwStr String to search
 * @param pwSeq Sequence to search for
 */
const wchar_t* wcsstr(const wchar_t* pwStr, const wchar_t* pwSeq);

} // namespace ksl

#endif
