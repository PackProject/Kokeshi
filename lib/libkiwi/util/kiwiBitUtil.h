#ifndef LIBKIWI_UTIL_BIT_UTIL_H
#define LIBKIWI_UTIL_BIT_UTIL_H
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Bit utilities
 */
class BitUtil {
public:
    /**
     * @brief Counts the number of set bits in the bitfield
     *
     * @param bits Bitfield
     */
    static u32 Count(u32 bits);

    /**
     * @brief Picks a random set bit from the bitfield
     *
     * @param bits Bitfield
     */
    static u32 RandomBit(u32 bits);
};

//! @}
} // namespace kiwi

#endif
