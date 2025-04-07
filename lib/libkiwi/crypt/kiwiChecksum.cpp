#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Add more data to the running checksum
 *
 * @param pData New data
 * @param size Size of data
 */
void Checksum::Process(const void* pData, u32 size) {
    K_ASSERT(pData != nullptr);

    const u8* p = static_cast<const u8*>(pData);

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

} // namespace kiwi
