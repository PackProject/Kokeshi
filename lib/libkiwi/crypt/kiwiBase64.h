#ifndef LIBKIWI_CRYPT_BASE64_H
#define LIBKIWI_CRYPT_BASE64_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>

namespace kiwi {
//! @addtogroup libkiwi_crypt
//! @{

/**
 * @brief Encodes binary data into a string of Base64 characters
 *
 * @param pData Binary data
 * @param size Data size
 */
String B64Encode(const void* pData, u32 size);

/**
 * @brief Decodes a string of Base64 characters into binary data
 *
 * @param rData Base64 encoded string
 * @param pDst Buffer for decoded data
 * @param size Size of the destination buffer
 *
 * @return Whether the decoded data fit completely within the buffer
 */
bool B64Decode(const String& rData, void* pDst, u32 size);

/**
 * @brief Decodes a string of Base64 characters into binary data
 * @note Caller is responsible for freeing the returned buffer
 *
 * @param rData Base64 encoded string
 * @param[out] pSize Decoded data size
 *
 * @return Buffer containing decoded data (null if decoding failed)
 */
void* B64Decode(const String& rData, u32* pSize = nullptr);

/**
 * @brief Calculates the buffer size required to decode the Base64 string
 *
 * @param rData Base64 encoded string
 */
K_INLINE u32 B64GetDecodeSize(const String& rData) {
    return (rData.Length() * 3) / 4;
}

//! @}
} // namespace kiwi

#endif
