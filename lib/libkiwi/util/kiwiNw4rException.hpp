#ifndef LIBKIWI_UTIL_NW4R_EXCEPTION_H
#define LIBKIWI_UTIL_NW4R_EXCEPTION_H
#include <libkiwi/util/kiwiDynamicSingleton.hpp>
#include <revolution/OS.h>
#include <types.h>

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
        // Exception type (see OSError)
        u8 error;
        // Last context before error
        OSContext* ctx;
        // Last DSISR value before error
        u32 dsisr;
        // Last DAR value before error
        u32 dar;
        // Last MSR value before error
        u32 msr;
        // Assertion info (if assertion failed)
        Assert assert;
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
    // Exception/assertion info
    Info mErrorInfo;

    // Exception user callback
    UserCallback mpUserCallback;
    // Exception user callback argument
    void* mpUserCallbackArg;

    // Exception thread
    OSThread mThread;
    // Exception thread stack
    u8 mThreadStack[0x4000];

    // Thread message queue
    OSMessageQueue mMessageQueue;
    // Thread message buffer
    OSMessage mMessageBuffer;

    // Current GX render mode
    const GXRenderModeObj* mpRenderMode;

    // OS exception type names
    static const char* scExceptionNames[OS_ERR_MAX];

    // Exception stack trace depth
    static const s32 scExceptionTraceDepth = 10;
    // Assertion stack trace depth
    static const s32 scAssertTraceDepth = 20;
};

} // namespace kiwi

#endif
