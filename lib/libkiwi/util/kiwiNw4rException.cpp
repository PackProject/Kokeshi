#include <cstdio>
#include <libkiwi.h>
#include <revolution/BASE.h>
#include <revolution/KPAD.h>
#include <revolution/VI.h>

namespace kiwi {

K_DYNAMIC_SINGLETON_IMPL(Nw4rException);

namespace {

/**
 * Fill in floating-point registers
 * @note Reimplementation of OSFillFPUContext (deadstripped)
 *
 * @param ctx OS context
 */
asm void ContextGetFP(register OSContext* ctx){
    // clang-format off
    mfmsr r5
    ori r5, r5, MSR_FP
    mtmsr r5
    isync

    stfd f0,  ctx->fprs[0]
    stfd f1,  ctx->fprs[1]
    stfd f2,  ctx->fprs[2]
    stfd f3,  ctx->fprs[3]
    stfd f4,  ctx->fprs[4]
    stfd f5,  ctx->fprs[5]
    stfd f6,  ctx->fprs[6]
    stfd f7,  ctx->fprs[7]
    stfd f8,  ctx->fprs[8]
    stfd f9,  ctx->fprs[9]
    stfd f10, ctx->fprs[10]
    stfd f11, ctx->fprs[11]
    stfd f12, ctx->fprs[12]
    stfd f13, ctx->fprs[13]
    stfd f14, ctx->fprs[14]
    stfd f15, ctx->fprs[15]
    stfd f16, ctx->fprs[16]
    stfd f17, ctx->fprs[17]
    stfd f18, ctx->fprs[18]
    stfd f19, ctx->fprs[19]
    stfd f20, ctx->fprs[20]
    stfd f21, ctx->fprs[21]
    stfd f22, ctx->fprs[22]
    stfd f23, ctx->fprs[23]
    stfd f24, ctx->fprs[24]
    stfd f25, ctx->fprs[25]
    stfd f26, ctx->fprs[26]
    stfd f27, ctx->fprs[27]
    stfd f28, ctx->fprs[28]
    stfd f29, ctx->fprs[29]
    stfd f30, ctx->fprs[30]
    stfd f31, ctx->fprs[31]

    mffs f0
    stfd f0, ctx->fpscr_pad
    lfd f0, ctx->fprs[0]
    mfspr r5, 0x398
    rlwinm. r5, r5, 3, 31, 31
    beq _exit

    psq_st f0,  OSContext.psfs[0](ctx),  0, 0
    psq_st f1,  OSContext.psfs[1](ctx),  0, 0
    psq_st f2,  OSContext.psfs[2](ctx),  0, 0
    psq_st f3,  OSContext.psfs[3](ctx),  0, 0
    psq_st f4,  OSContext.psfs[4](ctx),  0, 0
    psq_st f5,  OSContext.psfs[5](ctx),  0, 0
    psq_st f6,  OSContext.psfs[6](ctx),  0, 0
    psq_st f7,  OSContext.psfs[7](ctx),  0, 0
    psq_st f8,  OSContext.psfs[8](ctx),  0, 0
    psq_st f9,  OSContext.psfs[9](ctx),  0, 0
    psq_st f10, OSContext.psfs[10](ctx), 0, 0
    psq_st f11, OSContext.psfs[11](ctx), 0, 0
    psq_st f12, OSContext.psfs[12](ctx), 0, 0
    psq_st f13, OSContext.psfs[13](ctx), 0, 0
    psq_st f14, OSContext.psfs[14](ctx), 0, 0
    psq_st f15, OSContext.psfs[15](ctx), 0, 0
    psq_st f16, OSContext.psfs[16](ctx), 0, 0
    psq_st f17, OSContext.psfs[17](ctx), 0, 0
    psq_st f18, OSContext.psfs[18](ctx), 0, 0
    psq_st f19, OSContext.psfs[19](ctx), 0, 0
    psq_st f20, OSContext.psfs[20](ctx), 0, 0
    psq_st f21, OSContext.psfs[21](ctx), 0, 0
    psq_st f22, OSContext.psfs[22](ctx), 0, 0
    psq_st f23, OSContext.psfs[23](ctx), 0, 0
    psq_st f24, OSContext.psfs[24](ctx), 0, 0
    psq_st f25, OSContext.psfs[25](ctx), 0, 0
    psq_st f26, OSContext.psfs[26](ctx), 0, 0
    psq_st f27, OSContext.psfs[27](ctx), 0, 0
    psq_st f28, OSContext.psfs[28](ctx), 0, 0
    psq_st f29, OSContext.psfs[29](ctx), 0, 0
    psq_st f30, OSContext.psfs[30](ctx), 0, 0
    psq_st f31, OSContext.psfs[31](ctx), 0, 0

_exit:
    blr
    // clang-format on
}

/**
 * Gets value of the DSISR register
 */
u32 Mfdsisr() {
    register u32 _dsisr;
    // clang-format off
    asm {
        mfdsisr _dsisr
    }
    // clang-format on

    return _dsisr;
}

/**
 * Gets value of the DAR register
 */
u32 Mfdar() {
    register u32 _dar;
    // clang-format off
    asm {
        mfdar _dar
    }
    // clang-format on

    return _dar;
}

} // namespace

/**
 * Initializes exception handler
 */
Nw4rException::Nw4rException()
    : mpUserCallback(DefaultCallback),
      mpUserCallbackArg(NULL),
      mpRenderMode(NULL) {
    // Create debug console
    Nw4rConsole::CreateInstance();

    // Create exception thread
    OSCreateThread(&mThread, ThreadFunc, NULL,
                   mThreadStack + sizeof(mThreadStack), sizeof(mThreadStack),
                   OS_PRIORITY_MIN, OS_THREAD_DETACHED);
    OSInitMessageQueue(&mMessageQueue, &mMessageBuffer, 1);
    OSResumeThread(&mThread);

    // Hook error handler
    OSSetErrorHandler(OS_ERR_DSI, ErrorHandler);
    OSSetErrorHandler(OS_ERR_ISI, ErrorHandler);
    OSSetErrorHandler(OS_ERR_ALIGNMENT, ErrorHandler);
    OSSetErrorHandler(OS_ERR_PROTECTION, ErrorHandler);
    OSSetErrorHandler(OS_ERR_FP_EXCEPTION, NULL);

    // Auto-detect render mode
    mpRenderMode = LibGX::GetDefaultRenderMode();
    K_WARN(mpRenderMode == NULL, "No render mode!");
}

/**
 * Sets user exception callback
 *
 * @param callback Exception callback function
 * @param arg Exception callback argument
 */
void Nw4rException::SetUserCallback(UserCallback callback, void* arg) {
    mpUserCallback = callback != NULL ? callback : DefaultCallback;
    mpUserCallbackArg = arg;
}

/**
 * Writes text to the exception details
 * @note Written to Nw4rConsole and the debugger console
 *
 * @param fmt Format string
 * @param ... Format arguments
 */
void Nw4rException::Printf(const char* fmt, ...) {
    if (mpRenderMode == NULL) {
        return;
    }

    Nw4rDirectPrint::GetInstance().ChangeXfb(VIGetCurrentFrameBuffer(),
                                             mpRenderMode->fbWidth,
                                             mpRenderMode->xfbHeight);

    std::va_list list;
    va_start(list, fmt);
    Nw4rConsole::GetInstance().VPrintf(fmt, list);
    va_end(list);
}

/**
 * Triggers an assertion error
 *
 * @param file Name of source file where assertion occurred
 * @param line Line in source file where assertion occurred
 * @param msg Assertion message/expression
 */
void Nw4rException::FailAssert(const char* file, int line, const char* msg) {
    mErrorInfo.assert.file = file;
    mErrorInfo.assert.line = line;
    mErrorInfo.assert.msg = msg;

    // Preserve stack pointer at time of assertion
    mErrorInfo.assert.sp = OSGetStackPointer();

    // Invalid error type to signal assertion
    ErrorHandler(0xFF, OSGetCurrentContext(), Mfdsisr(), Mfdar());
}

/**
 * Exception thread main function
 */
void* Nw4rException::ThreadFunc(void* arg) {
#pragma unused(arg)
    PPCMtmsr(PPCMfmsr() & ~(MSR_FE0 | MSR_FE1));

    // Thread waits until error handler
    OSMessage msg;
    OSReceiveMessage(&GetInstance().mMessageQueue, &msg, OS_MSG_PERSISTENT);

    OSDisableInterrupts();
    VISetPreRetraceCallback(NULL);
    VISetPostRetraceCallback(NULL);
    VISetBlack(FALSE);
    VIFlush();

    GetInstance().DumpError();
    return NULL;
}

/**
 * Handles errors (exception/assertion)
 *
 * @param error Exception type
 * @param ctx Exception context
 * @param dsisr Last DSISR value
 * @param dar Last DAR value
 * @param ...
 */
void Nw4rException::ErrorHandler(u8 error, OSContext* ctx, u32 dsisr, u32 dar,
                                 ...) {
    Info& info = GetInstance().mErrorInfo;
    info.error = error;
    info.ctx = ctx;
    info.dsisr = dsisr;
    info.dar = dar;
    info.msr = PPCMfmsr();

    ContextGetFP(ctx);
    OSSetErrorHandler(error, NULL);

    // Allow thread to continue
    OSSendMessage(&GetInstance().mMessageQueue, 0, OS_MSG_PERSISTENT);

    if (OSGetCurrentThread() == NULL) {
        VISetPreRetraceCallback(NULL);
        VISetPostRetraceCallback(NULL);
        PPCMtmsr(PPCMfmsr() | MSR_RI);
        GetInstance().DumpError();
    }

    while (OSEnableScheduler() > 0) {
        ;
    }

    OSYieldThread();
    OSDisableScheduler();
}

/**
 * Allows controlling the console using the D-Pad
 *
 * @param info Exception info
 * @param arg Callback argument
 */
void Nw4rException::DefaultCallback(const Info& info, void* arg) {
#pragma unused(info)
#pragma unused(arg)
    Nw4rConsole::GetInstance().DrawDirect();

    while (true) {
        // Only re-draw when display is changed
        bool draw = false;

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            KPADStatus ks;
            memset(&ks, 0, sizeof(KPADStatus));
            KPADRead(i, &ks, 1);

            // Vertical scroll
            if (ks.hold & KPAD_BTN_DUP) {
                Nw4rConsole::GetInstance().ScrollUp();
            } else if (ks.hold & KPAD_BTN_DDOWN) {
                Nw4rConsole::GetInstance().ScrollDown();
            }

            // Horizontal scroll
            if (ks.hold & KPAD_BTN_DLEFT) {
                Nw4rConsole::GetInstance().ScrollLeft();
            } else if (ks.hold & KPAD_BTN_DRIGHT) {
                Nw4rConsole::GetInstance().ScrollRight();
            }

            // Determine whether we need to draw the display again
            draw |= (ks.hold & (KPAD_BTN_DUP | KPAD_BTN_DDOWN | KPAD_BTN_DLEFT |
                                KPAD_BTN_DRIGHT)) != 0;
        }

        if (draw) {
            Nw4rConsole::GetInstance().DrawDirect();
        }

        // Wait 100ms before polling again
        s64 start = OSGetTime();
        while (OSGetTime() - start < OS_MSEC_TO_TICKS(100)) {
            ;
        }
    }
}

/**
 * Dumps error information to the console
 */
void Nw4rException::DumpError() {
    VISetBlack(FALSE);
    VIFlush();
    OSDisableInterrupts();
    OSDisableScheduler();
    OSEnableInterrupts();

    // Acquire framebuffer
    Nw4rDirectPrint::GetInstance().SetupXfb();
    // Show console
    Nw4rConsole::GetInstance().SetVisible(true);

    // Dump information
    if (mErrorInfo.error == 0xFF) {
        DumpAssert();
    } else {
        DumpException();
    }

    // Display information
    while (true) {
        Nw4rConsole::GetInstance().DrawDirect();

        if (mpUserCallback != NULL) {
            mpUserCallback(mErrorInfo, mpUserCallbackArg);
        }
    }

    PPCHalt();
}

/**
 * Dumps exception information to the console
 */
void Nw4rException::DumpException() {
    // Basic information
    Printf("******** EXCEPTION OCCURRED! ********\n");
    Printf("Exception Type: %s\n", scExceptionNames[mErrorInfo.error]);
    Printf("SRR0: ");
    PrintSymbol(reinterpret_cast<void*>(mErrorInfo.ctx->srr0));
    Printf("\n");
    Printf("\n");

    // Memory status
    PrintHeapInfo();

    // Build region/target
    PrintBuildInfo();

    // Context
    PrintGPR();

    // Stack trace
    PrintStack(scExceptionTraceDepth);

    PrintThankYouMsg();
}

/**
 * Dumps assertion information to the console
 */
void Nw4rException::DumpAssert() {
    // Basic information
    Printf("******** ASSERTION FAILED! ********\n");
    Printf("%s\n", mErrorInfo.assert.msg);
    Printf("Source: %s(%d)\n", mErrorInfo.assert.file, mErrorInfo.assert.line);
    Printf("\n");

    // Memory status
    PrintHeapInfo();

    // Build region/target
    PrintBuildInfo();

    // Stack trace
    PrintStack(scAssertTraceDepth);

    PrintThankYouMsg();
}

/**
 * Prints heap information to the screen
 */
void Nw4rException::PrintHeapInfo() {
    Printf("---Heap Info---\n");
    Printf("Module: %.2f KB free\n",
           OS_MEM_B_TO_KB(
               static_cast<f32>(MemoryMgr::GetInstance().GetFreeSize())));
    Printf("\n");
}

/**
 * Prints build information to the screen
 */
void Nw4rException::PrintBuildInfo() {
    Printf("---Build Info---\n");
    Printf("%s\n", GetBuildDate().CStr());
    Printf("%s (%s)\n", GetBuildPack().CStr(), GetBuildTarget().CStr());
    Printf("\n");
}

/**
 * Prints GPR information to the screen
 */
void Nw4rException::PrintGPR() {
    Printf("---GPR Map---\n");
    Printf("-----------------------------------\n");

    for (int reg = 0; reg < 10; reg++) {
        Printf("R%02d:%08XH  R%02d:%08XH  R%02d:%08XH\n", reg,
               mErrorInfo.ctx->gprs[reg], reg + 11,
               mErrorInfo.ctx->gprs[reg + 11], reg + 22,
               mErrorInfo.ctx->gprs[reg + 22]);
    }

    Printf("R10:%08XH  R21:%08XH\n", mErrorInfo.ctx->gprs[10],
           mErrorInfo.ctx->gprs[21]);

    Printf("\n");
}

/**
 * Prints stack trace information to the screen
 *
 * @param depth Stack trace max depth
 */
void Nw4rException::PrintStack(u32 depth) {
    /**
     * Codewarrior stack frame
     */
    struct StackFrame {
        const StackFrame* next;
        const void* lr;
    };

    // Get stack pointer
    const StackFrame* frame = reinterpret_cast<const StackFrame*>(
        mErrorInfo.error == 0xFF
            ? mErrorInfo.assert.sp
            : reinterpret_cast<void*>(mErrorInfo.ctx->gprs[1]));

    Printf("---Stack Trace---\n");
    Printf("-----------------------------------\n");
    Printf("Address:   BackChain   LR save\n");

    for (int i = 0;
         i < depth && reinterpret_cast<std::uintptr_t>(frame) != 0xFFFFFFFF;
         i++, frame = frame->next) {

        // Print stack frame info
        Printf("%08X:  %08X    ", frame, frame->next);
        PrintSymbol(frame->lr);
        Printf("\n");
    }

    Printf("\n");
}

/**
 * Prints "thank you" message
 */
void Nw4rException::PrintThankYouMsg() {
    Printf("*************************************\n");
    Printf("\n");
    Printf("Thank you! You are a great debugger!\n");
}

/**
 * Prints symbol information using the map file
 */
void Nw4rException::PrintSymbol(const void* addr) {
    // Symbol's offset from the start of game code
    std::ptrdiff_t textOffset = PtrDistance(GetTextStart(), addr);

    // Symbol is from game (outside module)
    if (textOffset < 0 || textOffset >= GetTextSize()) {
        // Print raw address
        Printf("%08X (game)", addr);
        return;
    }

    // Non-game symbol, see if a map file has been loaded
    if (!MapFile::GetInstance().IsAvailable()) {
        /**
         * If the map file is not available, we are in one of two situations:
         * 1. The exception occurred before the map file could be read
         * 2. The map file does not exist
         *
         * Either way, we want to at least print the code's address relocatable
         * address (relative to the start of the module).
         */
        Printf("%08X (RELOC)", textOffset);
        return;
    }

    // Map file is available, let's use it
    const MapFile::Symbol* sym = MapFile::GetInstance().QueryTextSymbol(addr);

    /**
     * At this point we know the symbol is in module code, so the map file
     * should always return a result. However, to prevent the exception handler
     * from itself throwing an exception we do not assert this.
     */
    K_WARN_EX(sym == NULL, "Symbol missing(?) from module link map: reloc %08X",
              textOffset);

    // Symbol doesn't exist in map file
    if (sym == NULL) {
        // Print raw address
        Printf("%08X (MODULE)", addr);
        return;
    }

    // Offset into function where exception occurred
    u32 offset = sym->type == MapFile::LinkType_Relocatable
                     ? PtrDistance(AddToPtr(GetTextStart(), sym->offset), addr)
                     : PtrDistance(sym->addr, addr);

    // Print function name and instruction offset
    Printf("%s(+0x%04X)", sym->name, offset);
}

const char* Nw4rException::scExceptionNames[OS_ERR_MAX] = {
    "System Reset",
    "Machine Check",
    "DSI",
    "ISI",
    "External Interrupt",
    "Alignment",
    "Program",
    "Floating Point",
    "Decrementer",
    "System Call",
    "Trace",
    "Performance Monitor",
    "Break Point",
    "System Interrupt",
    "Thermal Interrupt",
    "Protection",
    "Floating Point"};

} // namespace kiwi
