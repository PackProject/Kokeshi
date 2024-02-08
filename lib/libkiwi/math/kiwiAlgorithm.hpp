#ifndef LIBKIWI_MATH_ALGORITHM_H
#define LIBKIWI_MATH_ALGORITHM_H
#include <types.h>

namespace kiwi {
namespace {

/**
 * Get maximum of two values
 */
template <typename T> inline const T& Max(const T& a, const T& b) {
    return a < b ? b : a;
}

/**
 * Get minimum of two values
 */
template <typename T> inline const T& Min(const T& a, const T& b) {
    return b < a ? b : a;
}

/**
 * Absolute value
 */
template <typename T> inline T Abs(const T& x) {
#ifdef __MWCC__
    return __abs(x);
#else
    return x < 0 ? -x : x;
#endif
}

/**
 * Clamp value to range
 */
template <typename T> inline T Clamp(const T& x, const T& min, const T& max) {
    if (x < min) {
        return min;
    }

    if (x > max) {
        return max;
    }

    return x;
}

/**
 * Distance between two pointers
 */
inline std::ptrdiff_t PtrDistance(const void* start, const void* end) {
    return reinterpret_cast<std::uintptr_t>(end) -
           reinterpret_cast<std::uintptr_t>(start);
}

/**
 * Add offset to pointer
 */
inline const void* AddToPtr(const void* ptr, s32 ofs) {
    return static_cast<const char*>(ptr) + ofs;
}

/**
 * Add offset to pointer
 */
inline void* AddToPtr(void* ptr, s32 ofs) {
    return static_cast<char*>(ptr) + ofs;
}

} // namespace
} // namespace kiwi

#endif
