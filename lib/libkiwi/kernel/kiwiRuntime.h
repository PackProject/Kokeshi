#ifndef LIBKIWI_KERNEL_RUNTIME_H
#define LIBKIWI_KERNEL_RUNTIME_H
#include <libkiwi/math/kiwiAlgorithm.h>
#include <revolution/OS.h>
#include <types.h>

namespace kiwi {

#ifdef __cplusplus
extern "C" {
#endif

extern funcptr_t __ctor_loc;
extern funcptr_t __ctor_end;

extern void* const _f_init;
extern void* const _e_init;

extern void* const _f_text;
extern void* const _e_text;

extern void* const _f_ctors;
extern void* const _e_ctors;

extern void* const _f_dtors;
extern void* const _e_dtors;

extern void* const _f_rodata;
extern void* const _e_rodata;

extern void* const _f_data;
extern void* const _e_data;

namespace {

const void* GetInitStart() {
    return &_f_init;
}
const void* GetInitEnd() {
    return &_e_init;
}
u32 GetInitSize() {
    return PtrDistance(GetInitStart(), GetInitEnd());
}

const void* GetTextStart() {
    return &_f_text;
}
const void* GetTextEnd() {
    return &_e_text;
}
u32 GetTextSize() {
    return PtrDistance(GetTextStart(), GetTextEnd());
}

const void* GetCtorsStart() {
    return &_f_ctors;
}
const void* GetCtorsEnd() {
    return &_e_ctors;
}
u32 GetCtorsSize() {
    return PtrDistance(GetCtorsStart(), GetCtorsEnd());
}

const void* GetDtorsStart() {
    return &_f_dtors;
}
const void* GetDtorsEnd() {
    return &_e_dtors;
}
u32 GetDtorsSize() {
    return PtrDistance(GetDtorsStart(), GetDtorsEnd());
}

const void* GetRodataStart() {
    return &_f_rodata;
}
const void* GetRodataEnd() {
    return &_e_rodata;
}
u32 GetRodataSize() {
    return PtrDistance(GetRodataStart(), GetRodataEnd());
}

const void* GetDataStart() {
    return &_f_data;
}
const void* GetDataEnd() {
    return &_e_data;
}
u32 GetDataSize() {
    return PtrDistance(GetDataStart(), GetDataEnd());
}

} // namespace

#ifdef __cplusplus
}
#endif

// Simulate a breakpoint for Dolphin debugging
#define K_DEBUG_BREAK()                                                        \
    {                                                                          \
        BOOL __enabled__ = OSDisableInterrupts();                              \
        K_LOG("************ BREAKPOINT! ************");                        \
        K_LOG_EX("Source: " __FILE__ "(%d)", __LINE__);                        \
        volatile int __x__ = 1;                                                \
        do {                                                                   \
            ;                                                                  \
        } while (__x__);                                                       \
        OSRestoreInterrupts(__enabled__);                                      \
    }

// Begin ASM block
#define K_ASM_BEGIN asm volatile {
// End ASM block
#define K_ASM_END }

// Copy GPR to variable
#define K_GET_GPR(r, var)                                                      \
    asm volatile {                                                             \
        mr var, r;                                                             \
    }
// Copy variable to GPR
#define K_SET_GPR(r, var)                                                      \
    asm volatile {                                                             \
        mr r, var;                                                             \
    }

// Backup GPRs to stack frame
#define K_SAVE_GPRS                                                            \
    asm volatile {                                                             \
        stwu r1, -0x90(r1);                                                    \
        stmw r3, 0xC(r1);                                                      \
        mflr r12;                                                              \
        stw r12, 0x8(r1);                                                      \
    }

// Restore GPRs from stack frame
#define K_REST_GPRS                                                            \
    asm volatile {                                                             \
        lwz r12, 0x8(r1);                                                      \
        mtlr r12;                                                              \
        lmw r3, 0xC(r1);                                                       \
        addi r1, r1, 0x90;                                                     \
    }

} // namespace kiwi

#endif
