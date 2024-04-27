#ifndef LIBKIWI_UTIL_CHECKSUM_H
#define LIBKIWI_UTIL_CHECKSUM_H
#include <libkiwi/kernel/kiwiAssert.h>
#include <types.h>

namespace kiwi {

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
     * @param data New data
     * @param size Size of data
     */
    void Process(const void* data, u32 size) {
        K_ASSERT(data != NULL);

        const u8* p = static_cast<const u8*>(data);

        // Two bytes at a time when possible
        while (size > sizeof(u16)) {
            mSum += *reinterpret_cast<const u16*>(p);
            mSumInv += ~*reinterpret_cast<const u16*>(p);

            p += sizeof(u16);
            size -= sizeof(u16);
        }

        // Get the rest
        while (size > 0) {
            mSum += *p;
            mSumInv += ~*p;

            p += sizeof(u8);
            size -= sizeof(u8);
        }
    }

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
    u32 mSum;
    u32 mSumInv;
};

} // namespace kiwi

#endif
