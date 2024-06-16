#ifndef LIBKIWI_DEBUG_NW4R_EXCEPTION_H
#define LIBKIWI_DEBUG_NW4R_EXCEPTION_H
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_debug
//! @{

/**
 * @brief Reimplementation of NW4R's exception
 */
class Nw4rException : public DynamicSingleton<Nw4rException> {
    friend class DynamicSingleton<Nw4rException>;

public:
    /**
     * @brief Error type
     */
    enum EError {
        EError_SystemReset, // Hard/Soft reset pins were asserted

        EError_MachineCheck, // Machine attempted to access an address which
                             // doesn't exist, or a data error was detected

        EError_DSI, // Failed to execute a load/store memory instruction

        EError_ISI, // Failed to fetch the next instruction from the PC

        EError_ExtInterrupt, // An external interrupt was triggered

        EError_Alignment, // Failed to execute a memory-related instruction due
                          // to alignment/cache restrictions

        EError_Program, // Attempted to decode an illegal instruction

        EError_FPUnavailable, // Attempted to execute a floating-point
                              // instruction while floating-point features are
                              // disabled in the MSR

        EError_Decrementer, // The decrementer register has completed
                            // decrementing (== 0)

        EError_SystemCall, // Executed a system call ('sc') instruction

        EError_Trace, // A branch instruction is executed while
                      // single-step/branch trace is enabled in the MSR

        EError_PerfMonitor, // A performance monitor interrupt was triggered

        EError_IABR, // The next instruction matches the Instruction Address
                     // Breakpoint Register

        EError_SMI, // A System Management Interrupt was triggered

        EError_Thermal, // The junction temperature crosses the threshold in
                        // THRM1/THRM2

        EError_Protection, // Failed to access OS-protected memory

        EError_FPException, //

        EError_AssertFail, // An assertion failed

        EError_None,
        EError_Max
    };

    /**
     * @brief Assertion info
     */
    struct Assert {
        const char* pFile; // Name of source file where assertion occurred
        int line;          // Line in source file where assertion occurred
        const char* pMsg;  // Assertion message/expression
        const void* pSP;   // Stack pointer value when assertion occurred

        /**
         * @brief Constructor
         */
        Assert() : pFile(""), line(0), pMsg(""), pSP(nullptr) {}
    };

    /**
     * @brief Error info
     * @see EError, Assert
     */
    struct Info {
        EError error;    // Exception type
        OSContext* pCtx; // Last context before error
        u32 dsisr;       // Last DSISR value before error
        u32 dar;         // Last DAR value before error
        u32 msr;         // Last MSR value before error
        Assert assert;   // Assertion info (if assertion failed)

        /**
         * @brief Constructor
         */
        Info() : error(EError_None), pCtx(nullptr), dsisr(0), dar(0), msr(0) {}
    };

    /**
     * @brief Exception user callback
     *
     * @param rInfo Error info
     * @param pArg User callback argument
     */
    typedef void (*UserCallback)(const Info& rInfo, void* pArg);

public:
    /**
     * @brief Sets user exception callback
     *
     * @param pCallback Exception callback function
     * @param pArg Exception callback argument
     */
    void SetUserCallback(UserCallback pCallback, void* pArg = nullptr);

    /**
     * @brief Writes text to the exception details
     *
     * @param pMsg Format string
     * @param ... Format arguments
     */
    void Printf(const char* pMsg, ...);

    /**
     * @brief Triggers an assertion error
     *
     * @param pFile Name of source file where assertion occurred
     * @param line Line in source file where assertion occurred
     * @param pMsg Assertion message/expression
     */
    void FailAssert(const char* pFile, int line, const char* pMsg);

private:
    /**
     * @brief Constructor
     */
    Nw4rException();

    /**
     * @brief Exception thread main function
     *
     * @param pArg Thread function argument
     */
    static void* ThreadFunc(void* pArg);

    /**
     * @brief Handles errors (exception/assertion)
     *
     * @param error Error type
     * @param pCtx Exception context
     * @param _dsisr DSISR value
     * @param _dar DAR value
     */
    static void ErrorHandler(u8 error, OSContext* ctx, u32 _dsisr, u32 _dar,
                             ...);

    /**
     * @brief Allows controlling the console using the D-Pad
     *
     * @param rInfo Exception info
     * @param pArg Callback argument
     */
    static void DefaultCallback(const Info& rInfo, void* pArg);

    /**
     * @brief Dumps error information to the console
     */
    void DumpError();

    /**
     * @brief Dumps exception information to the console
     */
    void DumpException();
    /**
     * @brief Dumps assertion information to the console
     */
    void DumpAssert();

    /**
     * @brief Prints heap information to the screen
     */
    void PrintHeapInfo();
    /**
     * @brief Prints build information to the screen
     */
    void PrintBuildInfo();
    /**
     * @brief Prints GPR information to the screen
     */
    void PrintGPR();
    /**
     * @brief Prints stack trace information to the screen
     *
     * @param depth Stack trace max depth
     */
    void PrintStack(u32 depth);
    /**
     * @brief Prints "thank you" message
     */
    void PrintThankYouMsg();
    /**
     * @brief Prints symbol information using the map file
     *
     * @param pAddr Symbol address
     */
    void PrintSymbol(const void* pAddr);

private:
    Info mErrorInfo; // Exception/assertion info

    UserCallback mpUserCallback; // User callback
    void* mpUserCallbackArg;     // User callback argument

    OSThread mThread;        // Exception thread
    u8 mThreadStack[0x4000]; // Thread stack

    OSMessageQueue mMessageQueue; // Thread message queue
    OSMessage mMessageBuffer;     // Thread message buffer

    const GXRenderModeObj* mpRenderMode; // Current GX render mode

    static const char* scExceptionNames[OS_ERR_MAX]; // OS exception names

    static const s32 scExceptionTraceDepth = 10; // Exception stack trace depth
    static const s32 scAssertTraceDepth = 20;    // Assertion stack trace depth
};

//! @}
} // namespace kiwi

#endif
