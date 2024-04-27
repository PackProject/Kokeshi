#include <libkiwi.h>

namespace kiwi {
namespace {

inline u32 rotl32(register u32 x, register int r) {
    // clang-format off
    asm {
        rotlw x, x, r
    }
    // clang-format on

    return x;
}

inline u32 fmix32(u32 h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

} // namespace

/**
 * @brief Hash function (MurmurHash3)
 *
 * @param key Key
 * @param len Key length
 * @return 32-bit hash
 */
hash_t HashImpl(const void* key, s32 len) {
    K_ASSERT(key != NULL);
    K_ASSERT(len > 0);

    const u8* data = static_cast<const u8*>(key);
    const int nblocks = len / 4;

    u32 h1 = 0xC70F6907;

    const u32 c1 = 0xcc9e2d51;
    const u32 c2 = 0x1b873593;

    const u32* blocks = reinterpret_cast<const u32*>(data + (nblocks * 4));
    for (int i = -nblocks; i; i++) {
        u32 k1 = blocks[i];

        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotl32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    const u8* tail = data + (nblocks * 4);
    u32 k1 = 0;

    switch (len & 3) {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;
        h1 ^= k1;
    };

    h1 ^= len;
    return fmix32(h1);
}

} // namespace kiwi
