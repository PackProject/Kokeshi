#include <cstring>
#include <libkiwi.h>
#include <revolution/EXI.h>
#include <revolution/OS.h>

namespace kiwi {

K_GLOBAL_INSTANCE_IMPL(IDebugger);

/**
 * @brief Debugger version (GeckoDotNet)
 */
enum EVersion {
    EVersion_Rvl = 0x80,
    EVersion_Dol = 0x81,
};

/**
 * @brief Register context (GeckoDotNet format)
 */
struct GeckoContext {
    /* 0x00 */ u32 cr;
    /* 0x04 */ u32 xer;
    /* 0x08 */ u32 ctr;
    /* 0x0C */ u32 dsisr;
    /* 0x10 */ u32 dar;
    /* 0x14 */ u32 srr0;
    /* 0x18 */ u32 srr1;
    /* 0x1C */ u32 lr;
    /* 0x20 */ u32 gprs[32];
    /* 0xA0 */ u64 fprs[32];

    /**
     * @brief Constructor
     */
    GeckoContext();

    /**
     * @brief Constructor
     *
     * @param ctx OS context
     * @param _dsisr DSISR register value
     * @param _dar DAR register value
     */
    GeckoContext(const OSContext& ctx, u32 _dsisr, u32 _dar);

    /**
     * @brief Load OS context
     *
     * @param ctx OS context
     * @param _dsisr DSISR register value
     * @param _dar DAR register value
     */
    void Load(const OSContext& ctx, u32 _dsisr, u32 _dar);

    /**
     * @brief Save to OS context
     *
     * @param ctx OS context
     */
    void Save(OSContext& ctx) const;
};

/**
 * @brief Constructor
 */
GeckoContext::GeckoContext()
    : cr(0), xer(0), ctr(0), dsisr(0), dar(0), srr0(0), srr1(0), lr(0) {
    std::memset(gprs, 0, sizeof(gprs));
    std::memset(fprs, 0, sizeof(fprs));
}

/**
 * @brief Constructor
 *
 * @param ctx OS context
 * @param _dsisr DSISR register value
 * @param _dar DAR register value
 */
GeckoContext::GeckoContext(const OSContext& ctx, u32 _dsisr, u32 _dar) {
    Load(ctx, _dsisr, _dar);
}

/**
 * @brief Apply OS context
 *
 * @param ctx OS context
 * @param _dsisr DSISR register value
 * @param _dar DAR register value
 */
void GeckoContext::Load(const OSContext& ctx, u32 _dsisr, u32 _dar) {
    cr = ctx.cr;
    xer = ctx.xer;
    ctr = ctx.ctr;
    srr0 = ctx.srr0;
    srr1 = ctx.srr1;
    lr = ctx.lr;

    dsisr = _dsisr;
    dar = _dar;

    std::memcpy(gprs, ctx.gprs, sizeof(gprs));
    std::memcpy(fprs, ctx.fprs, sizeof(fprs));
}

/**
 * @brief Save to OS context
 *
 * @param ctx OS context
 */
void GeckoContext::Save(OSContext& ctx) const {
    OSInitContext(&ctx, reinterpret_cast<void*>(srr0),
                  reinterpret_cast<void*>(gprs[1]));

    ctx.cr = cr;
    ctx.lr = lr;
    ctx.ctr = ctr;
    ctx.xer = xer;
    ctx.fpscr = Mffpscr();
    ctx.srr1 = srr1;

    std::memcpy(ctx.gprs, gprs, sizeof(ctx.gprs));
    std::memcpy(ctx.fprs, fprs, sizeof(ctx.fprs));
}

/**
 * @brief Breakpoint callback
 */
void IDebugger::BreakCallback(u8 error, OSContext* ctx, u32 _dsisr, u32 _dar,
                              ...) {
    K_ASSERT(ctx != NULL);
    K_ASSERT(error == OS_ERR_IABR ||
             (error == OS_ERR_DSI && (_dsisr & DSISR_DABR)));
}

/**
 * @brief Step trace callback
 */
void IDebugger::StepCallback(u8 error, OSContext* ctx, u32 _dsisr, u32 _dar,
                             ...) {
    K_ASSERT(ctx != NULL);
    K_ASSERT(error == OS_ERR_TRACE);
}

/**
 * @brief Receive and process the next debugger command
 *
 * @note Call this function when there is input pending
 */
void IDebugger::Calculate() {
#define HANDLE_CMD(x)                                                          \
    case ECommand_##x: OnEvent_##x(); break;

    u8 cmd = 0;
    if (!ReadObj(cmd) || cmd == 0) {
        return;
    }

    switch (cmd) {
        HANDLE_CMD(Write8);
        HANDLE_CMD(Write16);
        HANDLE_CMD(Write32);
        HANDLE_CMD(ReadN);
        HANDLE_CMD(Pause);
        HANDLE_CMD(UnPause);
        HANDLE_CMD(BreakPointData);
        HANDLE_CMD(BreakPointCode);
        HANDLE_CMD(ReadContext);
        HANDLE_CMD(WriteContext);
        HANDLE_CMD(CancelBreakPoint);
        HANDLE_CMD(Step);
        HANDLE_CMD(GetStatus);
        HANDLE_CMD(BreakPointExact);
        HANDLE_CMD(GetVersion);

    default: K_ASSERT_EX(false, "Gecko command %d not supported", cmd); break;
    }

#undef HANDLE_CMD
}

/**
 * @brief Write a 8-bit value to memory
 */
void IDebugger::OnEvent_Write8() {
    u8* dst = NULL;
    u32 value = 0;

    if (!ReadObj(dst) || !ReadObj(value)) {
        return;
    }

    K_ASSERT(dst != NULL);
    *dst = static_cast<u8>(value);
}

/**
 * @brief Write a 16-bit value to memory
 */
void IDebugger::OnEvent_Write16() {
    u16* dst = NULL;
    u32 value = 0;

    if (!ReadObj(dst) || !ReadObj(value)) {
        return;
    }

    K_ASSERT(dst != NULL);
    *dst = static_cast<u16>(value);
}

/**
 * @brief Write a 32-bit value to memory
 */
void IDebugger::OnEvent_Write32() {
    u32* dst = NULL;
    u32 value = 0;

    if (!ReadObj(dst) || !ReadObj(value)) {
        return;
    }

    K_ASSERT(dst != NULL);
    *dst = value;
}

/**
 * @brief Dump a range of memory
 */
void IDebugger::OnEvent_ReadN() {
    const void* start = NULL;
    const void* end = NULL;

    if (!ReadObj(end) || !ReadObj(start)) {
        return;
    }

    K_ASSERT(end > start);
    Write(start, PtrDistance(start, end));
}

/**
 * @brief Pause the program
 */
void IDebugger::OnEvent_Pause() {
    mExecState = EExecState_Paused;
}

/**
 * @brief Un-pause the program
 */
void IDebugger::OnEvent_UnPause() {
    mExecState = EExecState_UnPaused;
}

/**
 * @brief Set a PowerPC data breakpoint
 */
void IDebugger::OnEvent_BreakPointData() {
    // Load breakpoint info
    mBreakPoint.iabr = 0;
    ReadObj(mBreakPoint.dabr);
    mBreakPoint.align = 0;
    mBreakPoint.last = 0;

    // Set HW breakpoint
    Mtiabr(mBreakPoint.iabr);
    Mtdabr(mBreakPoint.dabr);
}

/**
 * @brief Set a PowerPC instruction breakpoint
 */
void IDebugger::OnEvent_BreakPointCode() {
    // Load breakpoint info
    ReadObj(mBreakPoint.iabr);
    mBreakPoint.dabr = 0;
    mBreakPoint.align = 0;
    mBreakPoint.last = 0;

    // Set HW breakpoint
    Mtiabr(mBreakPoint.iabr);
    Mtdabr(mBreakPoint.dabr);
}

/**
 * @brief Read the breakpoint context
 */
void IDebugger::OnEvent_ReadContext() {
    GeckoContext ctx;
    ctx.Load(mExecContext, Mfdsisr(), Mfdar());
    WriteObj(ctx);
}

/**
 * @brief Write the breakpoint context
 */
void IDebugger::OnEvent_WriteContext() {
    GeckoContext ctx;
    ReadObj(ctx);
    ctx.Save(mExecContext);
}

/**
 * @brief Cancel the active breakpoint
 */
void IDebugger::OnEvent_CancelBreakPoint() {
    Mtdabr(0);
    Mtiabr(0);
}

/**
 * @brief Write file contents to memory
 */
void IDebugger::OnEvent_WriteFile() {
    K_ASSERT_EX(false, "Not yet implemented");
}

void IDebugger::OnEvent_Step() {
    // Disable any further step traces
    mExecContext.srr1 &= ~MSR_SE;
}

/**
 * @brief Get the program execution status
 */
void IDebugger::OnEvent_GetStatus() {
    WriteObj<u8>(mExecState);
}

/**
 * @brief Set a Gecko breakpoint
 */
void IDebugger::OnEvent_BreakPointExact() {
    // Load breakpoint info
    mBreakPoint.iabr = 0;
    ReadObj(mBreakPoint.dabr);
    ReadObj(mBreakPoint.align);
    mBreakPoint.last = 0;

    // Set HW breakpoint
    Mtiabr(mBreakPoint.iabr);
    Mtdabr(mBreakPoint.dabr);
}

/**
 * @brief Get the debugger version
 */
void IDebugger::OnEvent_GetVersion() {
    WriteObj<u8>(EVersion_Rvl);
}

} // namespace kiwi
