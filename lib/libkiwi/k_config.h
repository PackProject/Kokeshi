#ifndef LIBKIWI_CONFIG_H
#define LIBKIWI_CONFIG_H

// Your custom defines
#ifdef LIBKIWI_USER_CONFIG
#include LIBKIWI_USER_CONFIG
#endif

// Default to big endian byte order
#ifndef LIBKIWI_BIG_ENDIAN
#define LIBKIWI_BIG_ENDIAN
#endif

// Expose private members only to Kamek hooks
#ifdef LIBKIWI_INTERNAL
#define LIBKIWI_KAMEK_PUBLIC public:
#else
#define LIBKIWI_KAMEK_PUBLIC
#endif

#endif
