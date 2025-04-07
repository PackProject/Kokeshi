/**
 * SHA-1 in C
 * By Steve Reid <steve@edmweb.com>
 * 100% Public Domain
 *
 * Modified for libkiwi
 */
#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Finalizes the hash and returns the digest
 */
String SHA1::Finalize() {
    u8 buffer[20];
    detail::SHA1Final(buffer, &mContext);

    // Space for 20 bytes printed as hex
    String digest;
    digest.Reserve(20 * 2);

    // Convert nibbles to characters
    static const char sHexDigits[] = "0123456789ABCDEF";
    for (int i = 0; i < LENGTHOF(buffer); i++) {
        digest[i * 2 + 0] = sHexDigits[(buffer[i] & 0xF0) >> 8];
        digest[i * 2 + 1] = sHexDigits[(buffer[i] & 0x0F) >> 0];
    }
}

namespace detail {

// clang-format off
#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
#define blk0(i) block->l[i]
#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15]^block->l[(i+2)&15]^block->l[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);
// clang-format on

/**
 * @brief Hashes a single 512-bit block
 *
 * @param pState Hash state
 * @param pBuffer Block buffer
 */
void SHA1Transform(u32 pState[5], const u8 pBuffer[64]) {
    u32 a, b, c, d, e;

    typedef union {
        u8 c[64];
        u32 l[16];
    } CHAR64LONG16;

    CHAR64LONG16 block[1]; /* use array to appear as a pointer */
    memcpy(block, pBuffer, 64);

    /* Copy context->state[] to working vars */
    a = pState[0];
    b = pState[1];
    c = pState[2];
    d = pState[3];
    e = pState[4];

    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a, b, c, d, e, 0);
    R0(e, a, b, c, d, 1);
    R0(d, e, a, b, c, 2);
    R0(c, d, e, a, b, 3);
    R0(b, c, d, e, a, 4);
    R0(a, b, c, d, e, 5);
    R0(e, a, b, c, d, 6);
    R0(d, e, a, b, c, 7);
    R0(c, d, e, a, b, 8);
    R0(b, c, d, e, a, 9);
    R0(a, b, c, d, e, 10);
    R0(e, a, b, c, d, 11);
    R0(d, e, a, b, c, 12);
    R0(c, d, e, a, b, 13);
    R0(b, c, d, e, a, 14);
    R0(a, b, c, d, e, 15);
    R1(e, a, b, c, d, 16);
    R1(d, e, a, b, c, 17);
    R1(c, d, e, a, b, 18);
    R1(b, c, d, e, a, 19);
    R2(a, b, c, d, e, 20);
    R2(e, a, b, c, d, 21);
    R2(d, e, a, b, c, 22);
    R2(c, d, e, a, b, 23);
    R2(b, c, d, e, a, 24);
    R2(a, b, c, d, e, 25);
    R2(e, a, b, c, d, 26);
    R2(d, e, a, b, c, 27);
    R2(c, d, e, a, b, 28);
    R2(b, c, d, e, a, 29);
    R2(a, b, c, d, e, 30);
    R2(e, a, b, c, d, 31);
    R2(d, e, a, b, c, 32);
    R2(c, d, e, a, b, 33);
    R2(b, c, d, e, a, 34);
    R2(a, b, c, d, e, 35);
    R2(e, a, b, c, d, 36);
    R2(d, e, a, b, c, 37);
    R2(c, d, e, a, b, 38);
    R2(b, c, d, e, a, 39);
    R3(a, b, c, d, e, 40);
    R3(e, a, b, c, d, 41);
    R3(d, e, a, b, c, 42);
    R3(c, d, e, a, b, 43);
    R3(b, c, d, e, a, 44);
    R3(a, b, c, d, e, 45);
    R3(e, a, b, c, d, 46);
    R3(d, e, a, b, c, 47);
    R3(c, d, e, a, b, 48);
    R3(b, c, d, e, a, 49);
    R3(a, b, c, d, e, 50);
    R3(e, a, b, c, d, 51);
    R3(d, e, a, b, c, 52);
    R3(c, d, e, a, b, 53);
    R3(b, c, d, e, a, 54);
    R3(a, b, c, d, e, 55);
    R3(e, a, b, c, d, 56);
    R3(d, e, a, b, c, 57);
    R3(c, d, e, a, b, 58);
    R3(b, c, d, e, a, 59);
    R4(a, b, c, d, e, 60);
    R4(e, a, b, c, d, 61);
    R4(d, e, a, b, c, 62);
    R4(c, d, e, a, b, 63);
    R4(b, c, d, e, a, 64);
    R4(a, b, c, d, e, 65);
    R4(e, a, b, c, d, 66);
    R4(d, e, a, b, c, 67);
    R4(c, d, e, a, b, 68);
    R4(b, c, d, e, a, 69);
    R4(a, b, c, d, e, 70);
    R4(e, a, b, c, d, 71);
    R4(d, e, a, b, c, 72);
    R4(c, d, e, a, b, 73);
    R4(b, c, d, e, a, 74);
    R4(a, b, c, d, e, 75);
    R4(e, a, b, c, d, 76);
    R4(d, e, a, b, c, 77);
    R4(c, d, e, a, b, 78);
    R4(b, c, d, e, a, 79);

    /* Add the working vars back into context.state[] */
    pState[0] += a;
    pState[1] += b;
    pState[2] += c;
    pState[3] += d;
    pState[4] += e;

    /* Wipe variables */
    a = b = c = d = e = 0;
    memset(block, '\0', sizeof(block));
}

/**
 * @brief Initializes new context
 *
 * @param pContext SHA1 context
 */
void SHA1Init(SHA1_CTX* pContext) {
    /* SHA1 initialization constants */
    pContext->state[0] = 0x67452301;
    pContext->state[1] = 0xEFCDAB89;
    pContext->state[2] = 0x98BADCFE;
    pContext->state[3] = 0x10325476;
    pContext->state[4] = 0xC3D2E1F0;
    pContext->count[0] = pContext->count[1] = 0;
}

/**
 * @brief Updates the hash value by processing the input data
 *
 * @param pContext SHA1 context
 * @param pData Input data
 * @param len Data length
 */
void SHA1Update(SHA1_CTX* pContext, const u8* pData, u32 len) {
    u32 i;
    u32 j;

    j = pContext->count[0];
    if ((pContext->count[0] += len << 3) < j) {
        pContext->count[1]++;
    }

    pContext->count[1] += (len >> 29);
    j = (j >> 3) & 63;

    if ((j + len) > 63) {
        memcpy(&pContext->buffer[j], pData, (i = 64 - j));
        SHA1Transform(pContext->state, pContext->buffer);
        for (; i + 63 < len; i += 64) {
            SHA1Transform(pContext->state, &pData[i]);
        }
        j = 0;
    } else {
        i = 0;
    }

    memcpy(&pContext->buffer[j], &pData[i], len - i);
}

/**
 * @brief Writes the message digest to an output buffer and finalizes the hash
 * context
 *
 * @param[out] pDigest Digest buffer
 * @param pContext SHA1 context
 */
void SHA1Final(u8 pDigest[20], SHA1_CTX* pContext) {
    unsigned i;
    u8 finalcount[8];
    u8 c;

    for (i = 0; i < 8; i++) {
        finalcount[i] = static_cast<u8>(
            (pContext->count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) &
            255); /* Endian independent */
    }

    c = 0200;
    SHA1Update(pContext, &c, 1);
    while ((pContext->count[0] & 504) != 448) {
        c = 0000;
        SHA1Update(pContext, &c, 1);
    }
    SHA1Update(pContext, finalcount, 8); /* Should cause a SHA1Transform() */

    for (i = 0; i < 20; i++) {
        pDigest[i] = static_cast<u8>(
            (pContext->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
    }

    /* Wipe variables */
    memset(pContext, '\0', sizeof(*pContext));
    memset(&finalcount, '\0', sizeof(finalcount));
}

} // namespace detail
} // namespace kiwi
