#include <cstdlib>
#include <cstring>
#include <libkiwi.h>
#include <revolution/IPC.h>

namespace kiwi {
namespace {

/**
 * @brief Utility for initializing IOS vectors
 *
 * @param prim Primitive value
 * @param vec IOS I/O vector
 */
template <typename T>
inline void MakeIOVector(const T& prim, IPCIOVector& vec) {
    vec.base = (void*)&prim;
    vec.length = sizeof(T);
}

/**
 * @brief Utility for initializing IOS vectors (string version)
 *
 * @param str String
 * @param vec IOS I/O vector
 */
template <>
inline void MakeIOVector<String>(const String& str, IPCIOVector& vec) {
    vec.base = (void*)str.CStr();
    vec.length = str.Length();
}

/**
 * @brief Dolphin device IOCTL IDs
 */
enum {
    IOCTLV_RPC_SET_CLIENT = 0x07,
    IOCTLV_RPC_SET_PRESENCE = 0x08,
    IOCTLV_RPC_RESET = 0x09,
    IOCTLV_GET_SYSTEM_TIME = 0x0A,
};

} // namespace

/**
 * @brief Constructor
 *
 * @param client Client app ID
 */
EmuRichPresence::EmuRichPresence(const String& client)
    : IRichPresence(client), mHandle(-1) {
    // Dolphin emulated device
    mHandle = IOS_Open("/dev/dolphin", IPC_OPEN_NONE);

    // Set client app ID
    if (IsConnected()) {
        UpdateClient();
    }
}

/**
 * @brief Destructor
 */
EmuRichPresence::~EmuRichPresence() {
    if (IsConnected()) {
        IOS_Close(mHandle);
    }
}

/**
 * @brief Whether there is an ongoing connection the RPC host
 */
bool EmuRichPresence::IsConnected() const {
    return mHandle >= 0;
}

/**
 * @brief Retreive the current Unix epoch time (in seconds)
 */
u64 EmuRichPresence::GetTimeNow() const {
    K_ASSERT(IsConnected());

    u64 time = 0;
    IPCIOVector output[1];
    MakeIOVector(time, output[0]);

    s32 result = IOS_Ioctlv(mHandle, IOCTLV_GET_SYSTEM_TIME, 0,
                            LENGTHOF(output), output);

    // Convert milliseconds to seconds
    K_ASSERT_EX(result >= 0, "IOCTLV_GET_SYSTEM_TIME failed: %d", result);
    return time / 1000;
}

/**
 * @brief Update Discord client/app ID
 */
void EmuRichPresence::UpdateClient() const {
    K_ASSERT(IsConnected());

    IPCIOVector input[1];
    MakeIOVector(mClient, input[0]);

    s32 result =
        IOS_Ioctlv(mHandle, IOCTLV_RPC_SET_CLIENT, LENGTHOF(input), 0, input);
    K_ASSERT_EX(result >= 0, "IOCTLV_RPC_SET_CLIENT failed: %d", result);
}

/**
 * @brief Update Discord presence status
 */
void EmuRichPresence::UpdatePresence() const {
    K_ASSERT(IsConnected());

    IPCIOVector input[10];

    // Presence info
    MakeIOVector(mDetails, input[0]);
    MakeIOVector(mState, input[1]);
    // Large image
    MakeIOVector(mLargeImageKey, input[2]);
    MakeIOVector(mLargeImageText, input[3]);
    // Small image
    MakeIOVector(mSmallImageKey, input[4]);
    MakeIOVector(mSmallImageText, input[5]);
    // Gameplay timestamps
    MakeIOVector(mStartTime, input[6]);
    MakeIOVector(mEndTime, input[7]);
    // Party size
    MakeIOVector(mPartyNum, input[8]);
    MakeIOVector(mPartyMax, input[9]);

    s32 result =
        IOS_Ioctlv(mHandle, IOCTLV_RPC_SET_PRESENCE, LENGTHOF(input), 0, input);
    K_ASSERT_EX(result >= 0, "IOCTLV_RPC_SET_PRESENCE failed: %d", result);
}

} // namespace kiwi
