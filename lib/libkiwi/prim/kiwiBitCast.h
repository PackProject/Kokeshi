#ifndef LIBKIWI_PRIM_BITCAST_H
#define LIBKIWI_PRIM_BITCAST_H
#include <types.h>

namespace kiwi {
namespace {

/**
 * @brief std::bit_cast for C++03
 */
template <typename TTo, typename TFrom> inline TTo BitCast(const TFrom& x) {
    // Not to standard, I know! But this will do.
    return reinterpret_cast<const TTo&>(x);
}

} // namespace
} // namespace kiwi

#endif
