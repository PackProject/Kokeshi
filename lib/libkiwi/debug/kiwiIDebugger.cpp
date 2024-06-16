#include <cstring>
#include <libkiwi.h>

namespace kiwi {

K_GLOBAL_INSTANCE_IMPL(IDebugger);

namespace {

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
     * @param rCtx OS context
     * @param _dsisr DSISR register value
     * @param _dar DAR register value
     */
    GeckoContext(const OSContext& rCtx, u32 _dsisr, u32 _dar);

    /**
     * @brief Loads OS context
     *
     * @param rCtx OS context
     * @param _dsisr DSISR register value
     * @param _dar DAR register value
     */
    void Load(const OSContext& rCtx, u32 _dsisr, u32 _dar);

    /**
     * @brief Saves to OS context
     *
     * @param rCtx OS context
     */
    void Save(OSContext& rCtx) const;
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
 * @param rCtx OS context
 * @param _dsisr DSISR register value
 * @param _dar DAR register value
 */
GeckoContext::GeckoContext(const OSContext& rCtx, u32 _dsisr, u32 _dar) {
    Load(rCtx, _dsisr, _dar);
}

/**
 * @brief Applies OS context
 *
 * @param rCtx OS context
 * @param _dsisr DSISR register value
 * @param _dar DAR register value
 */
void GeckoContext::Load(const OSContext& rCtx, u32 _dsisr, u32 _dar) {
    cr = rCtx.cr;
    xer = rCtx.xer;
    ctr = rCtx.ctr;
    srr0 = rCtx.srr0;
    srr1 = rCtx.srr1;
    lr = rCtx.lr;

    dsisr = _dsisr;
    dar = _dar;

    std::memcpy(gprs, rCtx.gprs, sizeof(gprs));
    std::memcpy(fprs, rCtx.fprs, sizeof(fprs));
}

/**
 * @brief Saves to OS context
 *
 * @param rCtx OS context
 */
void GeckoContext::Save(OSContext& rCtx) const {
    OSInitContext(&rCtx, reinterpret_cast<void*>(srr0),
                  reinterpret_cast<void*>(gprs[1]));

    rCtx.cr = cr;
    rCtx.lr = lr;
    rCtx.ctr = ctr;
    rCtx.xer = xer;
    rCtx.fpscr = Mffpscr();
    rCtx.srr1 = srr1;

    std::memcpy(rCtx.gprs, gprs, sizeof(rCtx.gprs));
    std::memcpy(rCtx.fprs, fprs, sizeof(rCtx.fprs));
}

} // namespace

/**
 * @brief Breakpoint callback
 *
 * @param error Error type
 * @param pCtx Exception context
 * @param _dsisr DSISR value
 * @param _dar DAR value
 */
void IDebugger::BreakCallback(u8 error, OSContext* pCtx, u32 _dsisr, u32 _dar,
                              ...) {
    K_ASSERT(pCtx != nullptr);
    K_ASSERT(error == OS_ERR_IABR ||
             (error == OS_ERR_DSI && (_dsisr & DSISR_DABR)));

    K_ASSERT_EX(false, "Not yet implemented");
}

/**
 * @brief Step trace callback
 *
 * @param error Error type
 * @param pCtx Exception context
 * @param _dsisr DSISR value
 * @param _dar DAR value
 */
void IDebugger::StepCallback(u8 error, OSContext* pCtx, u32 _dsisr, u32 _dar,
                             ...) {
    K_ASSERT(pCtx != nullptr);
    K_ASSERT(error == OS_ERR_TRACE);

    K_ASSERT_EX(false, "Not yet implemented");
}

/**
 * @brief Receives and processes the next debugger command
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
 * @brief Writes a 8-bit value to memory
 */
void IDebugger::OnEvent_Write8() {
    u8* pDst = nullptr;
    u32 value = 0;

    if (!ReadObj(pDst) || !ReadObj(value)) {
        return;
    }

    K_ASSERT(pDst != nullptr);
    *pDst = static_cast<u8>(value);
}

/**
 * @brief Writes a 16-bit value to memory
 */
void IDebugger::OnEvent_Write16() {
    u16* pDst = nullptr;
    u32 value = 0;

    if (!ReadObj(pDst) || !ReadObj(value)) {
        return;
    }

    K_ASSERT(pDst != nullptr);
    *pDst = static_cast<u16>(value);
}

/**
 * @brief Writes a 32-bit value to memory
 */
void IDebugger::OnEvent_Write32() {
    u32* pDst = nullptr;
    u32 value = 0;

    if (!ReadObj(pDst) || !ReadObj(value)) {
        return;
    }

    K_ASSERT(pDst != nullptr);
    *pDst = value;
}

/**
 * @brief Dumps a range of memory
 */
void IDebugger::OnEvent_ReadN() {
    const void* pDst = nullptr;
    const void* pEnd = nullptr;

    if (!ReadObj(pEnd) || !ReadObj(pDst)) {
        return;
    }

    K_ASSERT(pEnd > pDst);
    Write(pDst, PtrDistance(pDst, pEnd));
}

/**
 * @brief Pauses the program
 */
void IDebugger::OnEvent_Pause() {
    mExecState = EExecState_Paused;
}

/**
 * @brief Un-pauses the program
 */
void IDebugger::OnEvent_UnPause() {
    mExecState = EExecState_UnPaused;
}

/**
 * @brief Sets a PowerPC data breakpoint
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
 * @brief Sets a PowerPC instruction breakpoint
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
 * @brief Reads the breakpoint context
 */
void IDebugger::OnEvent_ReadContext() {
    GeckoContext ctx;
    ctx.Load(mExecContext, Mfdsisr(), Mfdar());
    WriteObj(ctx);
}

/**
 * @brief Writes the breakpoint context
 */
void IDebugger::OnEvent_WriteContext() {
    GeckoContext ctx;
    ReadObj(ctx);
    ctx.Save(mExecContext);
}

/**
 * @brief Cancels the active breakpoint
 */
void IDebugger::OnEvent_CancelBreakPoint() {
    Mtdabr(0);
    Mtiabr(0);
}

/**
 * @brief Writes file contents to memory
 */
void IDebugger::OnEvent_WriteFile() {
    K_ASSERT_EX(false, "Not yet implemented");
}

/**
 * @brief Steps forward one instruction
 */
void IDebugger::OnEvent_Step() {
    // Disable any further step traces
    mExecContext.srr1 &= ~MSR_SE;
}

/**
 * @brief Gets the program execution status
 */
void IDebugger::OnEvent_GetStatus() {
    WriteObj<u8>(mExecState);
}

/**
 * @brief Sets a Gecko breakpoint
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
 * @brief Gets the debugger version
 */
void IDebugger::OnEvent_GetVersion() {
    WriteObj<u8>(EVersion_Rvl);
}

} // namespace kiwi
