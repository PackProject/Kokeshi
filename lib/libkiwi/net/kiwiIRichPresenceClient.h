#ifndef LIBKIWI_NET_I_RICH_PRESENCE_CLIENT_H
#define LIBKIWI_NET_I_RICH_PRESENCE_CLIENT_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>

namespace kiwi {
//! @addtogroup libkiwi_net
//! @{

/**
 * @brief Rich presence interface
 */
class IRichPresenceClient {
public:
    /**
     * @brief Constructor
     */
    explicit IRichPresenceClient()
        : mStartTime(0), mEndTime(0), mPartyNum(0), mPartyMax(0) {}

    /**
     * @brief Destructor
     */
    virtual ~IRichPresenceClient() {}

    /**
     * @brief Tests whether there is a connection established
     */
    virtual bool IsConnected() const = 0;

    /**
     * @brief Retreives the current Unix epoch time (in seconds)
     */
    virtual u64 GetTimeNow() const = 0;

    /**
     * @brief Updates Discord client/app ID
     */
    virtual void UpdateApp() const = 0;
    /**
     * @brief Updates Discord presence status
     */
    virtual void UpdateActivity() const = 0;

    /**
     * @brief Sets the activity's application ID
     *
     * @param rAppID
     */
    void SetAppID(const String& rAppID) {
        mAppID = rAppID;
    }
    /**
     * @brief Gets the activity's application ID
     */
    String GetAppID() const {
        return mAppID;
    }

    /**
     * @brief Sets the activity's details
     *
     * @param rDetails What the player is currently doing
     */
    void SetDetails(const String& rDetails) {
        mDetails = rDetails;
    }
    /**
     * @brief Sets the activity's state
     *
     * @param rState The user's current status
     */
    void SetState(const String& rState) {
        mState = rState;
    }

    /**
     * @brief Sets the key of the large profile image
     *
     * @param rKey Key of the uploaded large profile image
     */
    void SetLargeImageKey(const String& rKey) {
        mLargeImageKey = rKey;
    }
    /**
     * @brief Sets the tooltip for the large profile image
     *
     * @param rText Tooltip for the large image
     */
    void SetLargeImageText(const String& rText) {
        mLargeImageText = rText;
    }

    /**
     * @brief Sets the key of the small profile image
     *
     * @param rKey Key of the uploaded small profile image
     */
    void SetSmallImageKey(const String& rKey) {
        mSmallImageKey = rKey;
    }
    /**
     * @brief Sets the tooltip for the small profile image
     *
     * @param rText Tooltip for the small image
     */
    void SetSmallImageText(const String& rText) {
        mSmallImageText = rText;
    }

    /**
     * @brief Sets the gameplay start time
     * @note Will show time as "elapsed"
     *
     * @param start Epoch seconds for game start
     */
    void SetStartTime(u64 start) {
        mStartTime = start;
        mEndTime = 0;
    }
    /**
     * @brief Sets the gameplay start time to the current clock time
     */
    void SetStartTimeNow() {
        mStartTime = GetTimeNow();
    }

    /**
     * @brief Sets the gameplay end time
     * @note Will show time as "remaining"
     *
     * @param start Epoch seconds for game start
     */
    void SetEndTime(u64 end) {
        mEndTime = end;
        mStartTime = 0;
    }

    /**
     * @brief Sets the number of players in the party
     *
     * @param num Current party size
     */
    void SetPartyNum(s32 num) {
        mPartyNum = num;
    }
    /**
     * @brief Sets the maximum number of players in the party
     *
     * @param num Maximum party size
     */
    void SetPartyMax(s32 max) {
        mPartyNum = max;
    }

protected:
    String mAppID; // Application ID

    String mDetails; // Presence details
    String mState;   // Presence state

    String mLargeImageKey;  // Large image name
    String mLargeImageText; // Large image description

    String mSmallImageKey;  // Small image name
    String mSmallImageText; // Small image description

    u64 mStartTime; // Gameplay start epoch
    u64 mEndTime;   // Gameplay end epoch

    s32 mPartyNum; // Party size
    s32 mPartyMax; // Maximum party size
};

//! @}
} // namespace kiwi

#endif
