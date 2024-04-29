#ifndef LIBKIWI_NET_EMU_RICH_PRESENCE_H
#define LIBKIWI_NET_EMU_RICH_PRESENCE_H
#include <libkiwi/net/kiwiIRichPresence.h>
#include <types.h>

namespace kiwi {

/**
 * @brief Rich presence implementation for Dolphin Emulator
 */
class EmuRichPresence : public IRichPresence {
public:
    EmuRichPresence(const String& client);
    virtual ~EmuRichPresence();

    virtual bool IsConnected() const;
    virtual u64 GetTimeNow() const;

    virtual void UpdateClient() const;
    virtual void UpdatePresence() const;

private:
    // Handle to Dolphin device
    s32 mHandle;
};

} // namespace kiwi

#endif
