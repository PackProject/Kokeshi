#ifndef LIBKIWI_CRYPT_CHECKSUM_H
#define LIBKIWI_CRYPT_CHECKSUM_H
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_crypt
//! @{

/**
 * @brief Running 32-bit checksum
 */
class Checksum {
public:
    /**
     * @brief Constructor
     */
    Checksum() : mSum(0), mSumInv(0) {}

    /**
     * @brief Add more data to the running checksum
     *
     * @param pData New data
     * @param size Size of data
     */
    void Process(const void* pData, u32 size);

    /**
     * @brief Get 32-bit representation
     */
    u32 Result() const {
        return mSum << 16 | mSumInv;
    }

    /**
     * @brief Conversion operator
     */
    operator u32() const {
        return Result();
    }

private:
    u32 mSum;    // Sum
    u32 mSumInv; // Inverse sum
};

//! @}
} // namespace kiwi

#endif
