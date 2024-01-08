#ifndef LIBKIWI_KERNEL_ASSERT_H
#define LIBKIWI_KERNEL_ASSERT_H
#include <types.h>

// General macros for external usage
#define LOG(msg) K_LOG(msg)
#define LOG_EX(msg, ...) K_LOG_EX(msg, __VA_ARGS__)
#define ASSERT(msg) K_ASSERT(msg)
#define ASSERT_EX(msg, ...) K_ASSERT_EX(msg, __VA_ARGS__)

#ifndef NDEBUG
// Log a message to the console
#define K_LOG(msg) kiwi_log(msg)

// Log a variadic message to the console
#define K_LOG_EX(msg, ...) kiwi_log(msg, __VA_ARGS__)

// Log a message to the console when a condition is met
#define K_WARN(exp, msg)                                                       \
    do {                                                                       \
        if ((exp)) {                                                           \
            K_LOG(msg);                                                        \
        }                                                                      \
    } while (0)

// Log a variadic message to the console when a condition is met
#define K_WARN_EX(exp, msg, ...)                                               \
    do {                                                                       \
        if ((exp)) {                                                           \
            K_LOG_EX(msg, __VA_ARGS__);                                        \
        }                                                                      \
    } while (0)

// Assert a condition and halt execution when it fails to hold
#define K_ASSERT(exp)                                                          \
    do {                                                                       \
        if (!(exp)) {                                                          \
            kiwi_fail_assert(__FILE__, __LINE__, #exp);                        \
        }                                                                      \
    } while (0)

// Assert a condition and halt execution when it fails to hold,
// displaying a custom error message
#define K_ASSERT_EX(exp, ...)                                                  \
    do {                                                                       \
        if (!(exp)) {                                                          \
            kiwi_fail_assert(__FILE__, __LINE__, __VA_ARGS__);                 \
        }                                                                      \
    } while (0)
#else
#define K_LOG(msg)
#define K_LOG_EX(msg, ...)
#define K_WARN(exp, msg)
#define K_WARN_EX(exp, msg, ...)
#define K_ASSERT(exp, ...)
#define K_ASSERT_EX(exp, ...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

void kiwi_log(const char* msg, ...);
void kiwi_fail_assert(const char* file, int line, const char* msg, ...);

#ifdef __cplusplus
}
#endif

#endif
