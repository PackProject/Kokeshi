#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Attempt to open this device
 *
 * @param path Virtual file path
 * @param timeOut Time-out period, in microseconds
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
    } while (OSGetTick() - start < OS_USEC_TO_TICKS(timeOut));

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
    IPCIOVector* vectors =
        new (32, EMemory_MEM2) IPCIOVector[in.Size() + out.Size()];
    K_ASSERT(vectors != nullptr);

    // Copy in user vectors
    int i = 0;
    for (int j = 0; j < in.Size(); j++, i++) {
        vectors[i].base = in[j].Base();
        vectors[i].length = in[j].Length();
    }
    for (int j = 0; j < out.Size(); j++, i++) {
        vectors[i].base = out[j].Base();
        vectors[i].length = out[j].Length();
    }

    s32 result = IOS_Ioctlv(mHandle, id, in.Size(), out.Size(), vectors);

    delete[] vectors;
    return result;
}

} // namespace kiwi
