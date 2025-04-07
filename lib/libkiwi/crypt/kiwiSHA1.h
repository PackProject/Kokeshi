#ifndef LIBKIWI_CRYPT_SHA1_H
#define LIBKIWI_CRYPT_SHA1_H
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_crypt
//! @{

namespace detail {
//! @addtogroup libkiwi_crypt
//! @{

/**
 * SHA-1 in C
 * By Steve Reid <steve@edmweb.com>
 * 100% Public Domain
 *
 * Modified for libkiwi
 */

/**
 * @brief SHA1 context
 */
struct SHA1_CTX {
    u32 state[5];
    u32 count[2];
    u8 buffer[64];
};

/**
 * @brief Initializes new context
 *
 * @param pContext SHA1 context
 */
void SHA1Init(SHA1_CTX* pContext);

/**
 * @brief Updates the hash value by processing the input data
 *
 * @param pContext SHA1 context
 * @param pData Input data
 * @param len Data length
 */
void SHA1Update(SHA1_CTX* pContext, const u8* pData, u32 len);

/**
 * @brief Writes the message digest to an output buffer and finalizes the hash
 * context
 *
 * @param[out] pDigest Digest buffer
 * @param pContext SHA1 context
 */
void SHA1Final(u8 pDigest[20], SHA1_CTX* pContext);

//! @}
} // namespace detail

/**
 * @brief SHA-1 hash algorithm
 */
class SHA1 {
public:
    /**
     * @brief Constructor
     */
    SHA1() {
        detail::SHA1Init(&mContext);
    }

    /**
     * @brief Updates the hash value by processing the input data
     *
     * @param pData New data
     * @param size Size of data
     */
    void Process(const void* pData, u32 size) {
        detail::SHA1Update(&mContext, static_cast<const u8*>(pData), size);
    }

    /**
     * @brief Finalizes the hash and returns the digest
     */
    String Finalize();

private:
    detail::SHA1_CTX mContext; //!< Hash context
};

/**
 * @brief Updates the hash value by processing the input data
 *
 * @param pData New data
 * @param size Size of data
 */
K_INLINE String SHA1Hash(const void* pData, u32 size) {
    SHA1 sha;
    sha.Process(pData, size);
    return sha.Finalize();
}

//! @}
} // namespace kiwi

#endif
