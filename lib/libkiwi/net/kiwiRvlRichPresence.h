#ifndef LIBKIWI_NET_RVL_RICH_PRESENCE_H
#define LIBKIWI_NET_RVL_RICH_PRESENCE_H
#include <libkiwi/k_types.h>
#include <libkiwi/net/kiwiIRichPresence.h>

namespace kiwi {

/**
 * @brief Rich presence implementation for Wii console
 */
class RvlRichPresence : public IRichPresence {
public:
    explicit RvlRichPresence(const String& client);
    virtual ~RvlRichPresence();

    virtual bool IsConnected() const;
    virtual u64 GetTimeNow() const;

    virtual void UpdateClient() const;
    virtual void UpdatePresence() const;
};

} // namespace kiwi

#endif
