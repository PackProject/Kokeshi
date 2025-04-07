#ifndef LIBKIWI_UTIL_IOS_VECTOR_H
#define LIBKIWI_UTIL_IOS_VECTOR_H
#include <libkiwi/k_types.h>
#include <revolution/IPC.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief IOS I/O vector
 */
class IosVector : public IPCIOVector {
public:
    /**
     * @brief Constructor
     */
    IosVector() {
        Clear();
    }

    /**
     * @brief Access data pointer
     */
    void* Base() const {
        return base;
    }
    /**
     * @brief Access data length
     */
    virtual u32 Length() const {
        return length;
    }

    /**
     * @brief Set vector contents
     *
     * @param _base Memory base
     * @param _length Memory size
     */
    void Set(const void* _base, u32 _length) {
        K_ASSERT(_base == nullptr || OSIsMEM2Region(_base));
        base = const_cast<void*>(_base);
        length = _length;
    }

    /**
     * @brief Clear vector contents
     */
    void Clear() {
        Set(nullptr, 0);
    }

private:
    using IPCIOVector::base;
    using IPCIOVector::length;
};

//! @}
} // namespace kiwi

#endif
