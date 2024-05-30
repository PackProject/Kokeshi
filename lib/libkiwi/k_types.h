#ifndef LIBKIWI_TYPES_H
#define LIBKIWI_TYPES_H

/**
 * Include important headers
 */

#ifdef __cplusplus
#include <cstdarg> // va_arg
#include <cstddef> // NULL, offsetof, size_t
#include <cstdint> // Common integer typedefs (intXX_t, etc.)
#else
#include <stdarg.h> // va_arg
#include <stddef.h> // NULL, offsetof, size_t
#include <stdint.h> // Common integer typedefs (intXX_t, etc.)
#endif

#include <kamek.hpp> // Kamek hooks
#include <macros.h>  // Useful macros

/**
 * Features where C++ is optional
 */

// Don't conflict with types.h
#ifndef TYPES_H

// 64-bit integer types
typedef unsigned long long u64;
typedef signed long long s64;
// 32-bit integer types
typedef unsigned long u32;
typedef signed long s32;
// 16-bit integer types
typedef unsigned short u16;
typedef signed short s16;
// 8-bit integer types
typedef unsigned char u8;
typedef signed char s8;

// Decimal types
typedef float f32;
typedef double f64;

// Boolean type for C code
typedef int BOOL;

// Generic function pointer
typedef void (*funcptr_t)(void);

#endif // TYPES_H

/**
 * Features where C++ is required
 */

#ifdef __cplusplus

// Override RP_GET_INSTANCE to NULL-check
#ifdef RP_GET_INSTANCE
#undef RP_GET_INSTANCE
#endif

#define RP_GET_INSTANCE(T)                                                     \
    (K_ASSERT(T::GetInstance() != NULL), T::GetInstance())

#endif

#endif // LIBKIWI_TYPES_H
