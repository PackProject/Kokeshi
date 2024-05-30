#ifndef LIBKIWI_PRIM_STL_H
#define LIBKIWI_PRIM_STL_H
#include <cstdio>
#include <cstring>
#include <libkiwi/k_types.h>

namespace ksl {

// cctype
bool isdigit(char c);
bool isalpha(char c);
bool isupper(char c);
bool islower(char c);

// cstring
char* strchr(const char* str, char c);
size_t strnlen(const char* str, size_t maxlen);

// cstdlib
s32 strtol(const char* str, char** endptr = NULL, int base = 0);
u32 strtoul(const char* str, char** endptr = NULL, int base = 0);
f64 atof(const char* str);

// cwchar
wchar_t* wcsncat(wchar_t* dst, const wchar_t* src, size_t maxlen);
int wcsncmp(const wchar_t* wcs1, const wchar_t* wcs2, size_t maxlen);
const wchar_t* wcsstr(const wchar_t* str, const wchar_t* seq);

} // namespace ksl

#endif
