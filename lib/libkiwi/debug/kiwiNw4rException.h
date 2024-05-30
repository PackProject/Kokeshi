#ifndef LIBKIWI_DEBUG_NW4R_EXCEPTION_H
#define LIBKIWI_DEBUG_NW4R_EXCEPTION_H
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>
#include <revolution/OS.h>

namespace kiwi {

/**
 * Reimplementation of NW4R's exception
 */
class Nw4rException : public DynamicSingleton<Nw4rException> {
    friend class DynamicSingleton<Nw4rException>;

public:
    /**
     * Assertion info
     */
    struct Assert {
        // Name of source file where assertion occurred
        const char* file;
        // Line in source file where assertion occurred
        int line;
        // Assertion message/expression
        const char* msg;
        // Stack pointer value when assertion occurred
        const void* sp;
    };

    /**
     * Error info
     */
    struct Info {
        u8 error;       // Exception type (see OSError)
        OSContext* ctx; // Last context before error
        u32 dsisr;      // Last DSISR value before error
        u32 dar;        // Last DAR value before error
        u32 msr;        // Last MSR value before error
        Assert assert;  // Assertion info (if assertion failed)
    };

    /**
     * Exception user callback
     * @param info Error info
     * @param arg User callback argument
     */
    typedef void (*UserCallback)(const Info& info, void* arg);

public:
    void SetUserCallback(UserCallback callback, void* arg);
    void Printf(const char* fmt, ...);
    void FailAssert(const char* file, int line, const char* msg);

private:
    Nw4rException();

    static void* ThreadFunc(void* arg);
    static void ErrorHandler(u8 error, OSContext* ctx, u32 dsisr, u32 dar, ...);
    static void DefaultCallback(const Info& info, void* arg);

    void DumpError();
    void DumpException();
    void DumpAssert();

    void PrintHeapInfo();
    void PrintBuildInfo();
    void PrintGPR();
    void PrintStack(u32 depth);
    void PrintThankYouMsg();
    void PrintSymbol(const void* addr);

private:
    Info mErrorInfo; // Exception/assertion info

    UserCallback mpUserCallback; // User callback
    void* mpUserCallbackArg;     // User callback argument

    OSThread mThread;        // Exception thread
    u8 mThreadStack[0x4000]; // Thread stack

    OSMessageQueue mMessageQueue; // Thread message queue
    OSMessage mMessageBuffer;     // Thread message buffer

    const GXRenderModeObj* mpRenderMode; // Current GX render mode

    static const char* scExceptionNames[OS_ERR_MAX]; // OS exception type names

    static const s32 scExceptionTraceDepth = 10; // Exception stack trace depth
    static const s32 scAssertTraceDepth = 20;    // Assertion stack trace depth
};

} // namespace kiwi

#endif
