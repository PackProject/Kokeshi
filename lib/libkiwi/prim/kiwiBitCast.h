#ifndef LIBKIWI_PRIM_BITCAST_H
#define LIBKIWI_PRIM_BITCAST_H
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_prim
//! @{
namespace {

/**
 * @brief Reinterprets an object's bits as a different type
 *
 * @tparam TTo Destination type
 * @tparam TFrom Source type
 * @param rValue Source value
 */
template <typename TTo, typename TFrom>
K_INLINE TTo BitCast(const TFrom& rValue) {
    // Not to standard, I know! But this will do.
    return reinterpret_cast<const TTo&>(rValue);
}

} // namespace
//! @}
} // namespace kiwi

#endif
