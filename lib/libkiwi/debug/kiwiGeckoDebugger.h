#ifndef LIBKIWI_DEBUG_GECKO_DEBUGGER_H
#define LIBKIWI_DEBUG_GECKO_DEBUGGER_H
#include <libkiwi/debug/kiwiIDebugger.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>
#include <libkiwi/util/kiwiGlobalInstance.h>
#include <revolution/EXI.h>
#include <revolution/OS.h>

namespace kiwi {

/**
 * @brief USB Gecko debugger support
 */
class GeckoDebugger : public IDebugger,
                      private GlobalInstance<IDebugger>,
                      public DynamicSingleton<GeckoDebugger> {
    friend class GlobalInstance<IDebugger>;
    friend class DynamicSingleton<GeckoDebugger>;

public:
    /**
     * @brief Attach the debugger (USB Gecko)
     *
     * @return Success
     */
    virtual bool Attach();

private:
    /**
     * @brief EXI input pending callback
     *
     * @param chan EXI channel
     * @param ctx Interrupt context
     */
    static void ExiCallback(EXIChannel chan, OSContext* ctx);

    /**
     * @brief Read data sent to the debugger
     *
     * @param dst Destination buffer
     * @param size Read length
     * @return Number of bytes read
     */
    virtual Optional<u32> Read(void* dst, u32 size);

    /**
     * @brief Write data over the debugger
     *
     * @param src Source buffer
     * @param size Write length
     * @return Number of bytes read
     */
    virtual Optional<u32> Write(const void* src, u32 size);
};

} // namespace kiwi

#endif
