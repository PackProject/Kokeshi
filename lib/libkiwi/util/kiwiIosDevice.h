#ifndef LIBKIWI_UTIL_IOS_DEVICE_H
#define LIBKIWI_UTIL_IOS_DEVICE_H
#include <libkiwi/core/kiwiMemoryMgr.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiOptional.h>
#include <libkiwi/prim/kiwiString.h>
#include <libkiwi/prim/kiwiVector.h>
#include <libkiwi/util/kiwiIosObject.h>
#include <revolution/IPC.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief IOS device handle
 */
class IosDevice {
public:
    /**
     * @brief Constructor
     */
    IosDevice() : mName(""), mHandle(-1) {}

    /**
     * @brief Destructor
     */
    ~IosDevice() {
        Close();
    }

    /**
     * @brief Attempt to open this device
     *
     * @param path Virtual file path
     * @param timeOut Time-out period, in microseconds
     * @return Success
     */
    bool Open(const String& path, u32 timeOut = 500);

    /**
     * @brief Attempt to close this device
     *
     * @return Success
     */
    bool Close();

    /**
     * @brief Tests whether this device is still open
     */
    bool IsOpen() const {
        return mHandle >= 0;
    }

    /**
     * @brief Perform I/O control (no parameters) on this device
     *
     * @param id Ioctl ID
     * @return IOS result code
     */
    s32 Ioctl(s32 id) const;

    /**
     * @brief Perform I/O control (single vectors) on this device
     *
     * @param id Ioctl ID
     * @param in Input vector
     * @param out Output vector
     * @return IOS result code
     */
    s32 Ioctl(s32 id, const IosVector& in, IosVector& out) const;

    /**
     * @brief Perform I/O control (multiple vectors) on this device
     *
     * @param id Ioctl ID
     * @param in Input vectors
     * @param out Output vectors
     * @return IOS result code
     */
    s32 IoctlV(s32 id, const TVector<IosVector>& in,
               const TVector<IosVector>& out) const;

private:
    String mName; // Virtual file path
    s32 mHandle;  // Virtual file descriptor
};

//! @}
} // namespace kiwi

#endif
