#include <libkiwi.h>

namespace kiwi {
namespace {

/**
 * @brief Rotates bits left by a specified amount
 *
 * @param x Initial value
 * @param r Number of bits to rotate
 */
u32 rotl32(register u32 x, register int r) {
    // clang-format off
    asm {
        rotlw x, x, r
    }
    // clang-format on

    return x;
}

/**
 * @brief Forces all bits of a hash block to avalanche
 *
 * @param h Hash block
 */
u32 fmix32(u32 h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

} // namespace

/**
 * @brief Hashes data of a specified size
 * @details MurmurHash3 algorithm
 *
 * @param pKey Key
 * @param len Key length
 */
hash_t HashImpl(const void* pKey, s32 len) {
    K_ASSERT(pKey != nullptr);
    K_ASSERT(len > 0);

    const u8* pData = static_cast<const u8*>(pKey);
    int nblocks = len / 4;

    u32 h1 = 0xC70F6907;
    u32 c1 = 0xCC9E2D51;
    u32 c2 = 0x1B873593;

    const u32* pBlocks = reinterpret_cast<const u32*>(pData + (nblocks * 4));
    for (int i = -nblocks; i; i++) {
        u32 k1 = pBlocks[i];

        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotl32(h1, 13);
        h1 = h1 * 5 + 0xE6546B64;
    }

    const u8* pTail = pData + (nblocks * 4);
    u32 k1 = 0;

    switch (len & 3) {
    case 3: k1 ^= pTail[2] << 16;
    case 2: k1 ^= pTail[1] << 8;
    case 1:
        k1 ^= pTail[0];
        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;
        h1 ^= k1;
    };

    h1 ^= len;
    return fmix32(h1);
}

} // namespace kiwi
