#include <cstring>
#include <libkiwi.h>

namespace kiwi {

K_DYNAMIC_SINGLETON_IMPL(GeckoDebugger);

/**
 * @brief Attaches the debugger (USB Gecko)
 *
 * @return Success
 */
bool GeckoDebugger::Attach() {
    // USB Gecko is a EXI device
    EXISetExiCallback(EXI_CHAN_0, ExiCallback);
    return true;
}

/**
 * @brief EXI input pending callback
 *
 * @param chan EXI channel
 * @param pCtx Interrupt context
 */
void GeckoDebugger::ExiCallback(EXIChannel chan, OSContext* pCtx) {
#pragma unused(chan)
#pragma unused(pCtx)

    GetInstance().Calculate();
}

/**
 * @brief Reads data sent to the debugger
 *
 * @param pDst Destination buffer
 * @param size Read length
 * @return Number of bytes read
 */
Optional<u32> GeckoDebugger::Read(void* pDst, u32 size) {
    K_ASSERT(pDst != nullptr);
    K_ASSERT(size > 0);

    // Lock this device while we use it
    if (!EXILock(EXI_CHAN_0, EXI_DEV_EXT, nullptr)) {
        return kiwi::nullopt;
    }

    // Setup channel parameters
    if (!EXISelect(EXI_CHAN_0, EXI_DEV_EXT, EXI_FREQ_32HZ)) {
        EXIUnlock(EXI_CHAN_0);
        return kiwi::nullopt;
    }

    bool success = true;

    // Prepare DMA
    u32 imm = 0xA0000000; // Read command
    success =
        success && EXIImm(EXI_CHAN_0, &imm, sizeof(u32), EXI_WRITE, nullptr);
    success = success && EXISync(EXI_CHAN_0);

    // Read data
    success = success && EXIImmEx(EXI_CHAN_0, pDst, size, EXI_READ);
    success = success && EXISync(EXI_CHAN_0);

    // Unlock this device when we are done
    success = success && EXIDeselect(EXI_CHAN_0);
    EXIUnlock(EXI_CHAN_0);

    return success ? MakeOptional(size) : kiwi::nullopt;
}

/**
 * @brief Writes data over the debugger
 *
 * @param pSrc Source buffer
 * @param size Write length
 * @return Number of bytes read
 */
Optional<u32> GeckoDebugger::Write(const void* pSrc, u32 size) {
    K_ASSERT(pSrc != nullptr);
    K_ASSERT(size > 0);

    // Lock this device while we use it
    if (!EXILock(EXI_CHAN_0, EXI_DEV_EXT, nullptr)) {
        return kiwi::nullopt;
    }

    // Setup channel parameters
    if (!EXISelect(EXI_CHAN_0, EXI_DEV_EXT, EXI_FREQ_32HZ)) {
        EXIUnlock(EXI_CHAN_0);
        return kiwi::nullopt;
    }

    bool success = true;

    // Prepare DMA
    u32 imm = 0xB0000000; // Write command
    success =
        success && EXIImm(EXI_CHAN_0, &imm, sizeof(u32), EXI_WRITE, nullptr);
    success = success && EXISync(EXI_CHAN_0);

    // Write data
    success = success &&
              EXIImmEx(EXI_CHAN_0, const_cast<void*>(pSrc), size, EXI_WRITE);
    success = success && EXISync(EXI_CHAN_0);

    // Unlock this device when we are done
    success = success && EXIDeselect(EXI_CHAN_0);
    EXIUnlock(EXI_CHAN_0);

    return success ? MakeOptional(size) : kiwi::nullopt;
}

} // namespace kiwi
