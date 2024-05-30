#ifndef LIBKIWI_DEBUG_ASSERT_H
#define LIBKIWI_DEBUG_ASSERT_H
#include <libkiwi/k_types.h>

// General macros for external usage
#define LOG(msg) K_LOG(msg)
#define LOG_EX(msg, ...) K_LOG_EX(msg, __VA_ARGS__)

#define ASSERT(exp) K_ASSERT(exp)
#define ASSERT_EX(exp, ...) K_ASSERT_EX(exp, __VA_ARGS__)

#define STATIC_ASSERT(exp) K_STATIC_ASSERT(exp)
#define STATIC_ASSERT_EX(exp, msg) K_STATIC_ASSERT_EX(exp, msg)

// For compiling modern libraries
#define static_assert(exp, msg) K_STATIC_ASSERT_EX(exp, msg)

#ifndef NDEBUG
// Log a message to the console
#define K_LOG(msg) kiwi_log(msg)
// Log a variadic message to the console
#define K_LOG_EX(msg, ...) kiwi_log(msg, __VA_ARGS__)

// Log a message to the console when a condition is met
#define K_WARN(exp, msg) ((exp) ? K_LOG(msg) : (void)0)
// Log a variadic message to the console when a condition is met
#define K_WARN_EX(exp, msg, ...) ((exp) ? K_LOG_EX(msg, __VA_ARGS__) : (void)0)

// Assert a condition and halt execution when it fails to hold
#define K_ASSERT(exp)                                                          \
    (!(exp) ? kiwi_fail_assert(__FILE__, __LINE__, #exp) : (void)0)
// Assert a condition and halt execution when it fails to hold,
// displaying a custom error message
#define K_ASSERT_EX(exp, ...)                                                  \
    (!(exp) ? kiwi_fail_assert(__FILE__, __LINE__, __VA_ARGS__) : (void)0)
#else
#define K_LOG(msg) (void)0
#define K_LOG_EX(msg, ...) (void)0
#define K_WARN(exp, msg) (void)0
#define K_WARN_EX(exp, msg, ...) (void)0
#define K_ASSERT(exp, ...) (void)0
#define K_ASSERT_EX(exp, ...) (void)0
#endif

// Compile-time assertion
#define K_STATIC_ASSERT(exp) extern u8 __K_PREDICATE[(exp) ? 1 : -1]
#define K_STATIC_ASSERT_EX(exp, msg) K_STATIC_ASSERT(exp)

#ifdef __cplusplus
extern "C" {
#endif

void kiwi_log(const char* msg, ...);
void kiwi_fail_assert(const char* file, int line, const char* msg, ...);

#ifdef __cplusplus
}
#endif

#endif
