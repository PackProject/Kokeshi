#ifndef LIBKIWI_NET_EMU_RICH_PRESENCE_CLIENT_H
#define LIBKIWI_NET_EMU_RICH_PRESENCE_CLIENT_H
#include <libkiwi/k_types.h>
#include <libkiwi/net/kiwiIRichPresenceClient.h>
#include <libkiwi/util/kiwiIosDevice.h>

namespace kiwi {
//! @addtogroup libkiwi_net
//! @{

/**
 * @brief Discord rich presence client for Dolphin Emulator
 */
class EmuRichPresenceClient : public IRichPresenceClient {
public:
    /**
     * @brief Constructor
     */
    explicit EmuRichPresenceClient() {
        // Dolphin provides an emulated device
        mDevDolphin.Open("/dev/dolphin");
    }

    /**
     * @brief Tests whether there is a connection established
     */
    virtual bool IsConnected() const {
        return mDevDolphin.IsOpen();
    }

    /**
     * @brief Retreives the current Unix epoch time (in seconds)
     */
    virtual u64 GetTimeNow() const;

    /**
     * @brief Updates Discord client/app ID
     */
    virtual void UpdateApp() const;

    /**
     * @brief Updates Discord presence status
     */
    virtual void UpdateActivity() const;

private:
    IosDevice mDevDolphin; // Handle to Dolphin device
};

//! @}
} // namespace kiwi

#endif
