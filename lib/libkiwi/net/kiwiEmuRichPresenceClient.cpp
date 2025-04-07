#include <libkiwi.h>

#include <revolution/IPC.h>

#include <cstdlib>
#include <cstring>

namespace kiwi {

/**
 * @brief Dolphin device I/O control codes
 */
enum {
    // dev/dolphin
    IoctlV_DiscordSetClient = 7,
    IoctlV_DiscordSetPresence = 8,
    IoctlV_DiscordReset = 9,
    IoctlV_GetSystemTime = 10,
};

/**
 * @brief Retreives the current Unix epoch time (in seconds)
 */
u64 EmuRichPresenceClient::GetTimeNow() const {
    if (!IsConnected()) {
        return;
    }

    TVector<IosVector> input;
    TVector<IosVector> output;

    IosObject<u64> time;
    output.PushBack(time);

    s32 result = mDevDolphin.IoctlV(IoctlV_GetSystemTime, input, output);
    K_ASSERT_EX(result >= 0, "IoctlV_GetSystemTime failed: %d", result);

    // Convert milliseconds to seconds
    return *time / 1000;
}

/**
 * @brief Updates Discord client/app ID
 */
void EmuRichPresenceClient::UpdateApp() const {
    if (!IsConnected()) {
        return;
    }

    TVector<IosVector> input;
    TVector<IosVector> output;

    IosString<char> client(mAppID);
    input.PushBack(client);

    s32 result = mDevDolphin.IoctlV(IoctlV_DiscordSetClient, input, output);
    K_ASSERT_EX(result >= 0, "IoctlV_DiscordSetClient failed: %d", result);

    K_LOG_EX("IoctlV_DiscordSetClient returned: %d\n", result);
}

/**
 * @brief Updates Discord presence status
 */
void EmuRichPresenceClient::UpdateActivity() const {
    if (!IsConnected()) {
        return;
    }

    TVector<IosVector> input;
    TVector<IosVector> output;

    // Presence info
    IosString<char> details(mDetails);
    IosString<char> state(mState);
    input.PushBack(details);
    input.PushBack(state);

    // Large image
    IosString<char> largeKey(mLargeImageKey);
    IosString<char> largeText(mLargeImageText);
    input.PushBack(largeKey);
    input.PushBack(largeText);

    // Small image
    IosString<char> smallKey(mSmallImageKey);
    IosString<char> smallText(mSmallImageText);
    input.PushBack(smallKey);
    input.PushBack(smallText);

    // Gameplay timestamps
    IosObject<u64> startTime(mStartTime);
    IosObject<u64> endTime(mEndTime);
    input.PushBack(startTime);
    input.PushBack(endTime);

    // Party size
    IosObject<s32> partyNum(mPartyNum);
    IosObject<s32> partyMax(mPartyMax);
    input.PushBack(partyNum);
    input.PushBack(partyMax);

    s32 result = mDevDolphin.IoctlV(IoctlV_DiscordSetPresence, input, output);
    K_ASSERT_EX(result >= 0, "IoctlV_DiscordSetPresence failed: %d", result);

    K_LOG_EX("IoctlV_DiscordSetPresence returned: %d\n", result);
}

} // namespace kiwi
