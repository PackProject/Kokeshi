#ifndef LIBKIWI_NET_EMU_RICH_PRESENCE_H
#define LIBKIWI_NET_EMU_RICH_PRESENCE_H
#include <libkiwi/k_types.h>
#include <libkiwi/net/kiwiIRichPresence.h>

namespace kiwi {

/**
 * @brief Rich presence implementation for Dolphin Emulator
 */
class EmuRichPresence : public IRichPresence {
public:
    explicit EmuRichPresence(const String& client);
    virtual ~EmuRichPresence();

    virtual bool IsConnected() const;
    virtual u64 GetTimeNow() const;

    virtual void UpdateClient() const;
    virtual void UpdatePresence() const;

private:
    s32 mHandle; // Handle to Dolphin device
};

} // namespace kiwi

#endif
