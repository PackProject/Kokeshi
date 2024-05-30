#ifndef LIBKIWI_CORE_SPR_H
#define LIBKIWI_CORE_SPR_H
#include <libkiwi/k_types.h>

/**
 * Special-purpose register functions
 */

#define REG_R(reg)                                                                   \
    /* clang-format off */                                                        \
    inline u32 Mf##reg() {                                                        \
        register u32 x;                                                           \
        asm volatile {                                                            \
            mf##reg x                                                             \
        }                                                                         \
        return x;                                                                 \
    } \
    /* clang-format on */

#define REG_W(reg)                                                                   \
    /* clang-format off */                                                        \
    inline void Mt##reg(register u32 x) {                                         \
        asm volatile {                                                            \
            mt##reg x                                                             \
        }                                                                         \
    } \
    /* clang-format on */

#define REG_RW(reg) REG_R(reg) REG_W(reg)

REG_RW(cr)

REG_RW(ctr)

/**
 * DABR
 * Data Address Breakpoint Register
 */
REG_RW(dabr)
// clang-format off
#define DABR_DR (1 << (31 - 31)) // Data read enable
#define DABR_DW (1 << (31 - 30)) // Data write enable
#define DABR_BT (1 << (31 - 29)) // Breakpoint translation enable
// clang-format on

REG_RW(dar)

REG_RW(dbat0l)

REG_RW(dbat0u)

REG_RW(dbat1l)

REG_RW(dbat1u)

REG_RW(dbat2l)

REG_RW(dbat2u)

REG_RW(dbat3l)

REG_RW(dbat3u)

REG_RW(dec)

/**
 * DSISR
 * DSI Exception Status Register
 */
REG_RW(dsisr)
// clang-format off
#define DSISR_DIRECT (1 << (31 - 0))  // Direct-store error exception
#define DSISR_HTEG   (1 << (31 - 1))  // Translation not found in the primary hash table entry group
#define DSISR_ACCESS (1 << (31 - 4))  // Memory access
#define DSISR_DABR   (1 << (31 - 10)) // DABR match occurred
// clang-format on

REG_RW(ear)

/**
 * FPSCR
 * Floating-Point Status and Control Register
 */
inline u32 Mffpscr() {
    register u64 fpscr;

    // clang-format off
    asm {
        mffs f31
        stfd f31, fpscr
    }
    // clang-format on

    return fpscr;
}

inline void Mtfpscr(register u32 val) {
    register struct {
        f32 pad;
        f32 data;
    } fpscr;

    // clang-format off
    asm {
        li r4, 0
        stw val, fpscr.data
        stw r4, fpscr.pad
        lfd f31, fpscr.pad
        mtfs f31
    }
    // clang-format on
}
// clang-format off
#define FPSCR_FX       (1       << (31 - 0))  // Floating-point exception summary
#define FPSCR_FEX      (1       << (31 - 1))  // Floating-point enabled exception summary
#define FPSCR_VX       (1       << (31 - 2))  // Floating-point invalid operation exception summary
#define FPSCR_OX       (1       << (31 - 3))  // Floating-point overflow exception
#define FPSCR_UX       (1       << (31 - 4))  // Floating-point underflow exception
#define FPSCR_ZX       (1       << (31 - 5))  // Floating-point zero divide exception
#define FPSCR_XX       (1       << (31 - 6))  // Floating-point inexact exception
#define FPSCR_VXSNAN   (1       << (31 - 7))  // Floating-point invalid operation exception for SNaN
#define FPSCR_VXISI    (1       << (31 - 8))  // Floating-point invalid operation exception for (inf - inf)
#define FPSCR_VXIDI    (1       << (31 - 9))  // Floating-point invalid operation exception for (inf / inf)
#define FPSCR_VXZDZ    (1       << (31 - 10)) // Floating-point invalid operation exception for (0 / 0)
#define FPSCR_VXIMZ    (1       << (31 - 11)) // Floating-point invalid operation exception for (inf * 0)
#define FPSCR_VXVC     (1       << (31 - 12)) // Floating-point invalid operation exception for invalid compare
#define FPSCR_FR       (1       << (31 - 13)) // Floating-point fraction rounded
#define FPSCR_FI       (1       << (31 - 14)) // Floating-point fraction inexact
#define FPSCR_FPRF     (0b11111 << (31 - 19)) // Floating-point result flags
#define FPSCR_RESERVED (1       << (31 - 20)) // Reserved bit
#define FPSCR_VXSOFT   (1       << (31 - 21)) // Floating-point invalid operation exception for software request
#define FPSCR_VXSQRT   (1       << (31 - 22)) // Floating-point invalid operation exception for invalid square root
#define FPSCR_VXCVI    (1       << (31 - 23)) // Floating-point invalid operation exception for invalid integer convert
#define FPSCR_VE       (1       << (31 - 24)) // Floating-point invalid operation exception enable
#define FPSCR_OE       (1       << (31 - 25)) // IEEE floating-point overflow exception enable
#define FPSCR_UE       (1       << (31 - 26)) // IEEE floating-point underflow exception enable
#define FPSCR_ZE       (1       << (31 - 27)) // IEEE floating-point zero divide exception enable
#define FPSCR_XE       (1       << (31 - 28)) // Floating-point inexact exception enable
#define FPSCR_NI       (1       << (31 - 29)) // Floating-point non-IEEE mode
#define FPSCR_RN       (0b11    << (31 - 31)) // Floating-point rounding control
// clang-format on

REG_RW(gqr0)

REG_RW(gqr1)

REG_RW(gqr2)

REG_RW(gqr3)

REG_RW(gqr4)

REG_RW(gqr5)

REG_RW(gqr6)

REG_RW(gqr7)

/**
 * HID0
 * Hardware Implementation-Dependent Register 0
 */
REG_RW(hid0)
// clang-format off
#define HID0_EMCP   (1 << (31 - 0))  // Enable MCP
#define HID0_DBP    (1 << (31 - 1))  // Enable/disable 60x bus address and data parity generation
#define HID0_EBA    (1 << (31 - 2))  // Enable/disable 60x bus address parity checking
#define HID0_EBD    (1 << (31 - 3))  // Enable 60x bus data parity checking
#define HID0_BCLK   (1 << (31 - 4))  // CLK_OUT enable
#define HID0_ECLK   (1 << (31 - 6))  // CLK_OUT enable
#define HID0_PAR    (1 << (31 - 7))  // Disable precharge of ARTRY
#define HID0_DOZE   (1 << (31 - 8))  // Doze mode enable
#define HID0_NAP    (1 << (31 - 9))  // Nap mode enable
#define HID0_SLEEP  (1 << (31 - 10)) // Sleep mode enable
#define HID0_DPM    (1 << (31 - 11)) // Dynamic power management enable
#define HID0_NHR    (1 << (31 - 15)) // Not hard reset (software-use only)
#define HID0_ICE    (1 << (31 - 16)) // Instruction cache enable
#define HID0_DCE    (1 << (31 - 17)) // Data cache enable
#define HID0_ILOCK  (1 << (31 - 18)) // Instruction cache lock
#define HID0_DLOCK  (1 << (31 - 19)) // Data cache lock
#define HID0_ICFI   (1 << (31 - 20)) // Instruction cache flash invalidate
#define HID0_DCFI   (1 << (31 - 21)) // Data cache flash invalidate
#define HID0_SPD    (1 << (31 - 22)) // Speculative cache access disable
#define HID0_IFEM   (1 << (31 - 23)) // Enable M bit on bus for instruction fetches
#define HID0_SGE    (1 << (31 - 24)) // Store gathering enable
#define HID0_DCFA   (1 << (31 - 25)) // Data cache flush assist
#define HID0_BTIC   (1 << (31 - 26)) // Branch Target Instruction Cache enable
#define HID0_ABE    (1 << (31 - 28)) // Address broadcast enable
#define HID0_BHT    (1 << (31 - 29)) // Branch history table enable
#define HID0_NOOPTI (1 << (31 - 31)) // No-op the data cache touch instructions
// clang-format on

/**
 * HID1
 * Hardware Implementation-Dependent Register 1
 */
REG_RW(hid1)
// clang-format off
#define HID1_PC0 (1 << (31 - 0)) // PLL configuration bits (read-only)
#define HID1_PC1 (1 << (31 - 1)) // PLL configuration bits (read-only)
#define HID1_PC2 (1 << (31 - 2)) // PLL configuration bits (read-only)
#define HID1_PC3 (1 << (31 - 3)) // PLL configuration bits (read-only)
#define HID1_PC4 (1 << (31 - 4)) // PLL configuration bits (read-only)
// clang-format on

/**
 * HID2
 * Hardware Implementation-Dependent Register 2
 */
REG_RW(hid2)
// clang-format off
#define HID2_WPE    (1      << (31 - 1))  // Write pipe enable
#define HID2_PSE    (1      << (31 - 2))  // Paired-single enable
#define HID2_LCE    (1      << (31 - 3))  // Locked cache enable
#define HID2_DMAQL  (0b1111 << (31 - 7))  // DMA queue length (read only)
#define HID2_DCHERR (1      << (31 - 8))  // dcbz_l cache hit error
#define HID2_DNCERR (1      << (31 - 9))  // DMA access to normal cache error
#define HID2_DCMERR (1      << (31 - 10)) // DMA cache miss error
#define HID2_DQOERR (1      << (31 - 11)) // DMA queue overflow error
#define HID2_DCHEE  (1      << (31 - 12)) // dcbz_l cache hit error enable
#define HID2_DNCEE  (1      << (31 - 13)) // DMA access to normal cache error enable
#define HID2_DCMEE  (1      << (31 - 14)) // DMA cache miss error enable
#define HID2_DQOEE  (1      << (31 - 15)) // DMA queue overflow error enable
// clang-format on

/**
 * HID4
 * Hardware Implementation-Dependent Register 4
 */
// clang-format off
#define HID4_H4A      (1    << (31 - 0))  // Unknown
#define HID4_L2FM     (0b11 << (31 - 2))  // L2 fetch mode
#define HID4_BPD      (0b11 << (31 - 4))  // Bus pipeline depth
#define HID4_BCO      (1    << (31 - 5))  // L2 second castout buffer enable
#define HID4_SBE      (1    << (31 - 6))  // Secondary BAT enable
#define HID4_PS1_CTL  (1    << (31 - 7))  // Paired-singles control bit 1
#define HID4_DBP      (1    << (31 - 9))  // Data bus parking
#define HID4_L2MUM    (1    << (31 - 10)) // L2 MUM enable
#define HID4_L2_CCFI  (1    << (31 - 11)) // L2CFI - L2 complete castout prior to L2 flash invalidate
#define HID4_PSS2_CTL (1    << (31 - 12)) // Paired-singles control bit 2
// clang-format on

REG_RW(iabr)

REG_RW(ibat0l)

REG_RW(ibat0u)

REG_RW(ibat1l)

REG_RW(ibat1u)

REG_RW(ibat2l)

REG_RW(ibat2u)

REG_RW(ibat3l)

REG_RW(ibat3u)

REG_RW(ictc)

/**
 * L2CR
 * L2 Control Register
 */
REG_RW(l2cr)
// clang-format off
#define L2CR_L2E  (1 << (31 - 0))  // L2 enable
#define L2CR_L2CE (1 << (31 - 1))  // L2 Checkstop enable
#define L2CR_L2DO (1 << (31 - 9))  // L2 data-only
#define L2CR_L2I  (1 << (31 - 10)) // L2 global invalidate
#define L2CR_L2WT (1 << (31 - 12)) // L2 write-through
#define L2CR_L2TS (1 << (31 - 13)) // L2 test support
#define L2CR_L2IP (1 << (31 - 31)) // L2 global invalidate in progress (read only)
// clang-format on

REG_RW(lr)

REG_RW(mmcr0)

REG_RW(mmcr1)

/**
 * MSR
 * Machine Status Register
 */
REG_RW(msr)
// clang-format off
#define MSR_POW (1 << (31 - 13)) // Power management enable
#define MSR_ILE (1 << (31 - 15)) // Exception little-endian mode
#define MSR_EE  (1 << (31 - 16)) // External interrupt enable
#define MSR_PR  (1 << (31 - 17)) // Privilege level
#define MSR_FP  (1 << (31 - 18)) // Floating-point available
#define MSR_ME  (1 << (31 - 19)) // Machine check enable
#define MSR_FE0 (1 << (31 - 20)) // IEEE floating-point exception mode 0
#define MSR_SE  (1 << (31 - 21)) // Single-step trace enable
#define MSR_BE  (1 << (31 - 22)) // Branch trace enable
#define MSR_FE1 (1 << (31 - 23)) // IEEE floating-point exception mode 1
#define MSR_IP  (1 << (31 - 25)) // Exception prefix
#define MSR_IR  (1 << (31 - 26)) // Instruction address translation
#define MSR_DR  (1 << (31 - 27)) // Data address translation
#define MSR_PM  (1 << (31 - 29)) // Performance monitor marked mode
#define MSR_RI  (1 << (31 - 30)) // Indicates whether system reset or machine check exception is recoverable
#define MSR_LE  (1 << (31 - 31)) // Little-endian mode enable
// clang-format on

/**
 * PMC1-PMC4
 * Performance Monitor Counter Registers
 */
REG_RW(pmc1)
REG_RW(pmc2)
REG_RW(pmc3)
REG_RW(pmc4)
// clang-format off
#define PMC_OV (1 << (31 - 0)) // Overflow
// clang-format on

/**
 * PVR
 * Processor Version Register
 */
REG_R(pvr)
// clang-format off
#define PVR_VER (0xFFFF0000) // A 16-bit number that uniquely identifies a particular processor version
#define PVR_REV (0x0000FFFF) // A 16-bit number that distinguishes between various releases of a particular version (that is, an engineering change level)
// clang-format on

REG_RW(sdr1)

REG_RW(sia)

REG_RW(sprg0)

REG_RW(sprg1)

REG_RW(sprg2)

REG_RW(sprg3)

REG_RW(srr0)

REG_RW(srr1)

REG_R(tbl)

REG_R(tbu)

REG_RW(thrm1)

REG_RW(thrm2)

REG_RW(thrm3)

REG_R(ummcr0)

REG_R(ummcr1)

REG_R(upmc1)

REG_R(upmc2)

REG_R(upmc3)

REG_R(upmc4)

REG_R(usia)

/**
 * WPAR
 * Write Pipe Address Register
 */
REG_RW(wpar)
// clang-format off
#define WPAR_GB_ADDR (0x07FFFFFF << (31 - 26)) // High order address bits of the data to be gathered
#define WPAR_BNE     (1          << (31 - 31)) // Buffer not empty (read only)
// clang-format on

REG_RW(xer)

#undef REG_R
#undef REG_W
#undef REG_RW

#endif
