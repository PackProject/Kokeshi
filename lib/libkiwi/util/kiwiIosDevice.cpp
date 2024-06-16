#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Attempt to open this device
 *
 * @param path Virtual file path
 * @param timeOut Time-out period, in milliseconds
 * @return Success
 */
bool IosDevice::Open(const String& path, u32 timeOut) {
    // Try until we succeed or time-out
    s32 start = OSGetTick();
    do {
        mHandle = IOS_Open(path, IPC_OPEN_NONE);
        if (IsOpen()) {
            return true;
        }
    } while (OSGetTick() - start < OS_MSEC_TO_TICKS(timeOut));

    return false;
}

/**
 * @brief Attempt to close the device
 *
 * @return Success
 */
bool IosDevice::Close() {
    if (!IsOpen()) {
        return true;
    }

    s32 result = IOS_Close(mHandle);

    // May not always be allowed to close(?)
    if (result == IPC_RESULT_OK) {
        mHandle = -1;
        return true;
    }

    return false;
}

/**
 * @brief Perform I/O control (no parameters) on this device
 *
 * @param id Ioctl ID
 * @return IOS result code
 */
s32 IosDevice::Ioctl(s32 id) const {
    K_ASSERT_EX(IsOpen(), "Please open this device");

    return IOS_Ioctl(mHandle, id, nullptr, 0, nullptr, 0);
}

/**
 * @brief Perform I/O control (single vectors) on this device
 *
 * @param id Ioctl ID
 * @param in Input vector
 * @param out Output vector
 * @return IOS result code
 */
s32 IosDevice::Ioctl(s32 id, const IosVector& in, IosVector& out) const {
    K_ASSERT_EX(IsOpen(), "Please open this device");

    return IOS_Ioctl(mHandle, id, in.Base(), in.Length(), out.Base(),
                     out.Length());
}

/**
 * @brief Perform I/O control (multiple vectors) on this device
 *
 * @param id Ioctl ID
 * @param in Input vectors
 * @param out Output vectors
 * @return IOS result code
 */
s32 IosDevice::IoctlV(s32 id, const TVector<IosVector>& in,
                      const TVector<IosVector>& out) const {
    K_ASSERT_EX(IsOpen(), "Please open this device");

    // Vectors need to be contiguous and usually(?) in MEM2
    IosVector* vectors =
        new (32, EMemory_MEM2) IosVector[in.Size() + out.Size()];

    // Copy in user vectors
    int i = 0;
    for (int j = 0; j < in.Size();) {
        vectors[i++] = in[j++];
    }
    for (int j = 0; j < out.Size();) {
        vectors[i++] = out[j++];
    }

    s32 result = IOS_Ioctlv(mHandle, id, in.Size(), out.Size(), vectors);

    delete[] vectors;
    return result;
}

} // namespace kiwi
