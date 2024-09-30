#ifndef LIBKIWI_MATH_ALGORITHM_H
#define LIBKIWI_MATH_ALGORITHM_H
#include <libkiwi/k_types.h>
#include <nw4r/math.h>

namespace kiwi {
//! @addtogroup libkiwi_math
//! @{
namespace {

/**
 * @brief Gets the minimum of two values
 *
 * @param rA First value
 * @param rB Second value
 */
template <typename T> K_INLINE const T& Min(const T& rA, const T& rB) {
    return rB < rA ? rB : rA;
}
/**
 * @brief Gets the maximum of two values
 *
 * @param rA First value
 * @param rB Second value
 */
template <typename T> K_INLINE const T& Max(const T& rA, const T& rB) {
    return rA < rB ? rB : rA;
}

/**
 * @brief Calculates the absolute value of the specified value
 *
 * @param rX Initial value
 */
template <typename T> K_INLINE T Abs(const T& rX) {
#ifdef __MWCC__
    return __abs(rX);
#else
    return rX < 0 ? -rX : rX;
#endif
}

/**
 * @brief Calculates the logarithm (base 10) of the specified value
 *
 * @param rX Initial value
 */
template <typename T> K_INLINE T Log(const T& rX) {
    f32 y = nw4r::math::FLog(static_cast<f32>(rX));
    return static_cast<T>(y);
}

/**
 * @brief Clamps value to range
 *
 * @param rValue Initial value
 * @param rMin Range minimum (exclusive)
 * @param rMax Range maximum (exclusive)
 */
template <typename T>
K_INLINE T Clamp(const T& rValue, const T& rMin, const T& rMax) {
    if (rValue < rMin) {
        return rMin;
    }

    if (rValue > rMax) {
        return rMax;
    }

    return rValue;
}

/**
 * @brief Gets the distance between two pointers
 *
 * @param pStart Start pointer
 * @param pEnd End pointer
 */
K_INLINE ptrdiff_t PtrDistance(const void* pStart, const void* pEnd) {
    return reinterpret_cast<uintptr_t>(pEnd) -
           reinterpret_cast<uintptr_t>(pStart);
}

/**
 * @brief Adds offset to pointer
 *
 * @param pAddr Base pointer
 * @param ofs Offset to apply
 */
K_INLINE void* AddToPtr(void* pAddr, ptrdiff_t ofs) {
    return static_cast<char*>(pAddr) + ofs;
}
/**
 * @brief Adds offset to pointer (const-view)
 *
 * @param pAddr Base pointer
 * @param ofs Offset to apply
 */
K_INLINE const void* AddToPtr(const void* pAddr, ptrdiff_t ofs) {
    return static_cast<const char*>(pAddr) + ofs;
}

/**
 * @brief Adds offset to pointer
 *
 * @tparam T Type of resulting pointer
 * @param pAddr Base pointer
 * @param ofs Offset to apply
 */
template <typename T> K_INLINE T* AddToPtr(void* pAddr, ptrdiff_t ofs) {
    return reinterpret_cast<T*>(static_cast<char*>(pAddr) + ofs);
}
/**
 * @brief Adds offset to pointer (const-view)
 *
 * @tparam T Type of resulting pointer
 * @param pAddr Base pointer
 * @param ofs Offset to apply
 */
template <typename T>
K_INLINE const T* AddToPtr(const void* pAddr, ptrdiff_t ofs) {
    return reinterpret_cast<const T*>(static_cast<const char*>(pAddr) + ofs);
}

} // namespace
//! @}
} // namespace kiwi

#endif
