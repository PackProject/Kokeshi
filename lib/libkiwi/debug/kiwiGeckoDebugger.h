#ifndef LIBKIWI_DEBUG_GECKO_DEBUGGER_H
#define LIBKIWI_DEBUG_GECKO_DEBUGGER_H
#include <libkiwi/debug/kiwiIDebugger.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>
#include <libkiwi/util/kiwiGlobalInstance.h>
#include <revolution/EXI.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_debug
//! @{

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
     * @brief Attaches the debugger (USB Gecko)
     *
     * @return Success
     */
    virtual bool Attach();

private:
    /**
     * @brief EXI input pending callback
     *
     * @param chan EXI channel
     * @param pCtx Interrupt context
     */
    static void ExiCallback(EXIChannel chan, OSContext* pCtx);

    /**
     * @brief Reads data sent to the debugger
     *
     * @param pDst Destination buffer
     * @param size Read length
     * @return Number of bytes read
     */
    virtual Optional<u32> Read(void* pDst, u32 size);

    /**
     * @brief Writes data over the debugger
     *
     * @param pSrc Source buffer
     * @param size Write length
     * @return Number of bytes read
     */
    virtual Optional<u32> Write(const void* pSrc, u32 size);
};

//! @}
} // namespace kiwi

#endif
