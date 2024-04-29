#ifndef LIBKIWI_NET_I_RICH_PRESENCE_H
#define LIBKIWI_NET_I_RICH_PRESENCE_H
#include <libkiwi/prim/kiwiString.h>
#include <revolution/OS.h>
#include <types.h>

namespace kiwi {

/**
 * @brief Rich presence interface (common functionality between console/emu)
 */
class IRichPresence {
public:
    /**
     * @brief Constructor
     *
     * @param client Client app ID
     */
    IRichPresence(const String& client)
        : mClient(client),
          mStartTime(0),
          mEndTime(0),
          mPartyNum(0),
          mPartyMax(0) {}

    /**
     * @brief Destructor
     */
    virtual ~IRichPresence() {}

    /**
     * @brief Whether there is an ongoing connection the RPC host
     */
    virtual bool IsConnected() const = 0;
    /**
     * @brief Retreive the current Unix epoch time (in seconds)
     */
    virtual u64 GetTimeNow() const = 0;

    /**
     * @brief Update Discord client/app ID
     */
    virtual void UpdateClient() const = 0;
    /**
     * @brief Update Discord presence status
     */
    virtual void UpdatePresence() const = 0;

    void SetDetails(const String& details) {
        mDetails = details;
    }
    void SetState(const String& state) {
        mState = state;
    }

    void SetLargeImageKey(const String& key) {
        mLargeImageKey = key;
    }
    void SetLargeImageText(const String& text) {
        mLargeImageText = text;
    }

    void SetSmallImageKey(const String& key) {
        mSmallImageKey = key;
    }
    void SetSmallImageText(const String& text) {
        mSmallImageText = text;
    }

    void SetStartTime(u64 start) {
        mStartTime = start;
    }
    void SetStartTimeNow() {
        mStartTime = GetTimeNow();
    }

    void SetPartyNum(s32 num) {
        mPartyNum = num;
    }
    void SetPartyMax(s32 max) {
        mPartyNum = max;
    }

protected:
    // Client ID
    String mClient;

    // Presence info
    String mDetails;
    String mState;

    // Large image
    String mLargeImageKey;
    String mLargeImageText;

    // Small image
    String mSmallImageKey;
    String mSmallImageText;

    // Gameplay timestamps
    u64 mStartTime;
    u64 mEndTime;

    // Party size
    s32 mPartyNum;
    s32 mPartyMax;
};

} // namespace kiwi

#endif
