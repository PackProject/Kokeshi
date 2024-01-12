#ifndef LIBKIWI_KERNEL_RUNTIME_H
#define LIBKIWI_KERNEL_RUNTIME_H
#include <libkiwi/math/kiwiAlgorithm.hpp>
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

static inline const void* GetInitStart() {
    return &_f_init;
}
static inline const void* GetInitEnd() {
    return &_e_init;
}
static inline u32 GetInitSize() {
    return PtrDistance(GetInitStart(), GetInitEnd());
}

static inline const void* GetTextStart() {
    return &_f_text;
}
static inline const void* GetTextEnd() {
    return &_e_text;
}
static inline u32 GetTextSize() {
    return PtrDistance(GetTextStart(), GetTextEnd());
}

static inline const void* GetCtorsStart() {
    return &_f_ctors;
}
static inline const void* GetCtorsEnd() {
    return &_e_ctors;
}
static inline u32 GetCtorsSize() {
    return PtrDistance(GetCtorsStart(), GetCtorsEnd());
}

static inline const void* GetDtorsStart() {
    return &_f_dtors;
}
static inline const void* GetDtorsEnd() {
    return &_e_dtors;
}
static inline u32 GetDtorsSize() {
    return PtrDistance(GetDtorsStart(), GetDtorsEnd());
}

static inline const void* GetRodataStart() {
    return &_f_rodata;
}
static inline const void* GetRodataEnd() {
    return &_e_rodata;
}
static inline u32 GetRodataSize() {
    return PtrDistance(GetRodataStart(), GetRodataEnd());
}

static inline const void* GetDataStart() {
    return &_f_data;
}
static inline const void* GetDataEnd() {
    return &_e_data;
}
static inline u32 GetDataSize() {
    return PtrDistance(GetDataStart(), GetDataEnd());
}

#ifdef __cplusplus
}
#endif

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
