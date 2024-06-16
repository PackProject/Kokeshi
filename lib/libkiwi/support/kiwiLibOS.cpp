#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Saves floating-point registers to the context
 *
 * @param ctx OS context
 */
void LibOS::FillFPUContext(register OSContext* ctx) {
    K_ASSERT(ctx != nullptr);

    // clang-format off
    K_ASM_BEGIN {
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

        mfhid2 r5
        rlwinm. r5, r5, 3, 31, 31 // HID2_PSE
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
    }
    // clang-format on
}

} // namespace kiwi
