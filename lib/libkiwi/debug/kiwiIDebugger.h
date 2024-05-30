#ifndef LIBKIWI_DEBUG_I_DEBUGGER_H
#define LIBKIWI_DEBUG_I_DEBUGGER_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiOptional.h>
#include <libkiwi/util/kiwiGlobalInstance.h>
#include <revolution/OS.h>

namespace kiwi {

/**
 * @brief Debugger (GeckoDotNet) interface
 */
class IDebugger {
    // For vectoring data breakpoints to BreakCallback
    friend class Nw4rException;

public:
    /**
     * @brief Constructor
     */
    IDebugger() : mExecState(EExecState_Paused) {
        // Reserve debugger-related exceptions
        OSSetErrorHandler(OS_ERR_TRACE, StepCallback);
        OSSetErrorHandler(OS_ERR_IABR, BreakCallback);

        // NOTE: DSI handler reserved for Nw4rException.
        // OSSetErrorHandler(OS_ERR_DSI, BreakCallback);
    }

    /**
     * @brief Destructor
     */
    virtual ~IDebugger() {}

    /**
     * @brief Attach the debugger
     * @note Register EventCallback to fit your communication type
     *
     * @return Success
     */
    virtual bool Attach() = 0;

protected:
    /**
     * @brief Program execution state
     */
    enum EExecState {
        EExecState_Dummy,
        EExecState_UnPaused,
        EExecState_Paused,
        EExecState_BreakPoint
    };

    /**
     * @brief Receive and process the next debugger command
     *
     * @note Call this function when there is input pending
     */
    void Calculate();

private:
    /**
     * @brief Breakpoint context (GeckoDotNet format)
     */
    struct BreakPoint {
        /* 0x00 */ u32 iabr;
        /* 0x04 */ u32 dabr;
        /* 0x08 */ u32 align;
        /* 0x0C */ u32 last;

        /**
         * @brief Constructor
         */
        BreakPoint() : iabr(0), dabr(0), align(0), last(0) {}
    };

    /**
     * @brief GeckoDotNet packet size
     */
    static const u32 UPLOAD_PACKET_SIZE = 0xF80;

    /**
     * @brief Breakpoint callback
     */
    static void BreakCallback(u8 error, OSContext* ctx, u32 _dsisr, u32 _dar,
                              ...);
    /**
     * @brief Step trace callback
     */
    static void StepCallback(u8 error, OSContext* ctx, u32 _dsisr, u32 _dar,
                             ...);

    /**
     * @brief Read data sent to the debugger
     *
     * @param dst Destination buffer
     * @param size Read length
     * @return Number of bytes read
     */
    virtual Optional<u32> Read(void* dst, u32 size) = 0;

    /**
     * @brief Write data over the debugger
     *
     * @param src Source buffer
     * @param size Write length
     * @return Number of bytes read
     */
    virtual Optional<u32> Write(const void* src, u32 size) = 0;

    /**
     * @brief Read data into an object
     *
     * @param t Destination object
     * @return Success
     */
    template <typename T> bool ReadObj(T& t) {
        return Read(&t, sizeof(T)).ValueOr(0) == sizeof(T);
    }

    /**
     * @brief Write data from an object
     *
     * @param t Source object
     * @return Success
     */
    template <typename T> bool WriteObj(const T& t) {
        return Write(&t, sizeof(T)).ValueOr(0) == sizeof(T);
    }

/**
 * @brief Debugger commands
 */
#define DEFINE_CMD(x, id)                                                      \
    static const u32 ECommand_##x = id;                                        \
    void OnEvent_##x();

    DEFINE_CMD(Write8, 0x01);
    DEFINE_CMD(Write16, 0x02);
    DEFINE_CMD(Write32, 0x03);
    DEFINE_CMD(ReadN, 0x04);
    DEFINE_CMD(Pause, 0x06);
    DEFINE_CMD(UnPause, 0x07);
    DEFINE_CMD(BreakPointData, 0x09);
    DEFINE_CMD(BreakPointCode, 0x10);
    DEFINE_CMD(ReadContext, 0x2F);
    DEFINE_CMD(WriteContext, 0x30);
    DEFINE_CMD(CancelBreakPoint, 0x38);
    DEFINE_CMD(WriteFile, 0x41);
    DEFINE_CMD(Step, 0x44);
    DEFINE_CMD(GetStatus, 0x50);
    DEFINE_CMD(BreakPointExact, 0x89);
    DEFINE_CMD(GetVersion, 0x99);
#undef DEFINE_CMD

private:
    // Program execution status
    EExecState mExecState;
    // Program execution context
    OSContext mExecContext;
    // Active breakpoint
    BreakPoint mBreakPoint;
};

} // namespace kiwi

#endif
