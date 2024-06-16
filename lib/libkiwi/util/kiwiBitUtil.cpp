#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Counts the number of set bits in the bitfield
 *
 * @param bits Bitfield
 */
u32 BitUtil::Count(u32 bits) {
    u32 i = 0;

    while (bits != 0) {
        i += bits & 1;
        bits >>= 1;
    }

    return i;
}

/**
 * @brief Picks a random set bit from the bitfield
 *
 * @param bits Bitfield
 */
u32 BitUtil::RandomBit(u32 bits) {
    u32 max = Count(bits);

    // No set bits
    if (max == 0) {
        return 0;
    }

    // Choice means n'th set bit
    u32 choice = 1 + Random().NextU32(max);

    // Find the n'th set bit
    u32 idx = 0;
    for (int i = 0; i < choice; idx++) {
        if (bits & (1 << idx)) {
            if (++i >= choice) {
                break;
            }
        }
    }

    // Convert to bitmask
    return 1 << idx;
}

} // namespace kiwi
