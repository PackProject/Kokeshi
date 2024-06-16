#ifndef LIBKIWI_NET_RVL_RICH_PRESENCE_H
#define LIBKIWI_NET_RVL_RICH_PRESENCE_H
#include <libkiwi/k_types.h>
#include <libkiwi/net/kiwiIRichPresence.h>

namespace kiwi {
//! @addtogroup libkiwi_net
//! @{

/**
 * @brief Rich presence implementation for Wii console
 */
class RvlRichPresence : public IRichPresence {
public:
    /**
     * @brief Constructor
     *
     * @param rClient Client app ID
     */
    explicit RvlRichPresence(const String& rClient);

    /**
     * @brief Tests whether there is a connection established
     */
    virtual bool IsConnected() const;

    /**
     * @brief Retreives the current Unix epoch time (in seconds)
     */
    virtual u64 GetTimeNow() const;

    /**
     * @brief Updates Discord client/app ID
     */
    virtual void UpdateClient() const;

    /**
     * @brief Updates Discord presence status
     */
    virtual void UpdatePresence() const;
};

//! @}
} // namespace kiwi

#endif
