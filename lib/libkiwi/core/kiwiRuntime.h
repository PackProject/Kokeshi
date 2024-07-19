#ifndef LIBKIWI_CORE_RUNTIME_H
#define LIBKIWI_CORE_RUNTIME_H
#include <libkiwi/k_types.h>
#include <libkiwi/math/kiwiAlgorithm.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Linker Generated Symbols (for your module)
 */
/**@{*/
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
/**@}*/

/**
 * @name Linker Generated Symbols (for the base DOL)
 */
/**@{*/
extern void* const _f_dol_init;
extern void* const _e_dol_init;

extern void* const _f_dol_text;
extern void* const _e_dol_text;

extern void* const _f_dol_ctors;
extern void* const _e_dol_ctors;

extern void* const _f_dol_dtors;
extern void* const _e_dol_dtors;

extern void* const _f_dol_rodata;
extern void* const _e_dol_rodata;

extern void* const _f_dol_data;
extern void* const _e_dol_data;
/**@}*/

namespace {

/**
 * @brief Gets the start address of the stack
 * @note Stack grows upwards
 */
const void* GetStackStart() {
    return _stack_addr;
}
/**
 * @brief Gets the end address of the stack
 * @note Stack grows upwards
 */
const void* GetStackEnd() {
    return _stack_end;
}

/**
 * @name Module section information
 */
/**@{*/
/**
 * @brief Gets the start address of the module ELF
 */
const void* GetModuleStart() {
    return kokeshi::CURRENT_MODULE.start;
}
/**
 * @brief Gets the size of the module ELF
 */
u32 GetModuleSize() {
    return kokeshi::CURRENT_MODULE.size;
}
/**
 * @brief Gets the end address of the module ELF
 */
const void* GetModuleEnd() {
    return AddToPtr(GetModuleStart(), GetModuleSize());
}

/**
 * @brief Gets the start address of the .init ELF section
 */
const void* GetModuleInitStart() {
    return &_f_init;
}
/**
 * @brief Gets the end address of the .init ELF section
 */
const void* GetModuleInitEnd() {
    return &_e_init;
}
/**
 * @brief Gets the size of the .init ELF section
 */
u32 GetModuleInitSize() {
    return PtrDistance(GetModuleInitStart(), GetModuleInitEnd());
}

/**
 * @brief Gets the start address of the .text ELF section
 */
const void* GetModuleTextStart() {
    return &_f_text;
}
/**
 * @brief Gets the end address of the .text ELF section
 */
const void* GetModuleTextEnd() {
    return &_e_text;
}
/**
 * @brief Gets the size of the .text ELF section
 */
u32 GetModuleTextSize() {
    return PtrDistance(GetModuleTextStart(), GetModuleTextEnd());
}

/**
 * @brief Gets the start address of the .ctors ELF section
 */
const void* GetModuleCtorsStart() {
    return &_f_ctors;
}
/**
 * @brief Gets the end address of the .ctors ELF section
 */
const void* GetModuleCtorsEnd() {
    return &_e_ctors;
}
/**
 * @brief Gets the size of the .ctors ELF section
 */
u32 GetModuleCtorsSize() {
    return PtrDistance(GetModuleCtorsStart(), GetModuleCtorsEnd());
}

/**
 * @brief Gets the start address of the .dtors ELF section
 */
const void* GetModuleDtorsStart() {
    return &_f_dtors;
}
/**
 * @brief Gets the end address of the .dtors ELF section
 */
const void* GetModuleDtorsEnd() {
    return &_e_dtors;
}
/**
 * @brief Gets the size of the .dtors ELF section
 */
u32 GetModuleDtorsSize() {
    return PtrDistance(GetModuleDtorsStart(), GetModuleDtorsEnd());
}

/**
 * @brief Gets the start address of the .rodata ELF section
 */
const void* GetModuleRodataStart() {
    return &_f_rodata;
}
/**
 * @brief Gets the end address of the .rodata ELF section
 */
const void* GetModuleRodataEnd() {
    return &_e_rodata;
}
/**
 * @brief Gets the size of the .rodata ELF section
 */
u32 GetModuleRodataSize() {
    return PtrDistance(GetModuleRodataStart(), GetModuleRodataEnd());
}

/**
 * @brief Gets the start address of the .data ELF section
 */
const void* GetModuleDataStart() {
    return &_f_data;
}
/**
 * @brief Gets the end address of the .data ELF section
 */
const void* GetModuleDataEnd() {
    return &_e_data;
}
/**
 * @brief Gets the size of the .data ELF section
 */
u32 GetModuleDataSize() {
    return PtrDistance(GetModuleDataStart(), GetModuleDataEnd());
}
/**@}*/

/**
 * @name DOL section information
 */
/**@{*/
/**
 * @brief Gets the start address of the .init DOL section
 */
const void* GetDolInitStart() {
    return &_f_dol_init;
}
/**
 * @brief Gets the end address of the .init DOL section
 */
const void* GetDolInitEnd() {
    return &_e_dol_init;
}
/**
 * @brief Gets the size of the .init DOL section
 */
u32 GetDolInitSize() {
    return PtrDistance(GetDolInitStart(), GetDolInitEnd());
}

/**
 * @brief Gets the start address of the .text DOL section
 */
const void* GetDolTextStart() {
    return &_f_dol_text;
}
/**
 * @brief Gets the end address of the .text DOL section
 */
const void* GetDolTextEnd() {
    return &_e_dol_text;
}
/**
 * @brief Gets the size of the .text DOL section
 */
u32 GetDolTextSize() {
    return PtrDistance(GetDolTextStart(), GetDolTextEnd());
}

/**
 * @brief Gets the start address of the .ctors DOL section
 */
const void* GetDolCtorsStart() {
    return &_f_dol_ctors;
}
/**
 * @brief Gets the end address of the .ctors DOL section
 */
const void* GetDolCtorsEnd() {
    return &_e_dol_ctors;
}
/**
 * @brief Gets the size of the .ctors DOL section
 */
u32 GetDolCtorsSize() {
    return PtrDistance(GetDolCtorsStart(), GetDolCtorsEnd());
}

/**
 * @brief Gets the start address of the .dtors DOL section
 */
const void* GetDolDtorsStart() {
    return &_f_dol_dtors;
}
/**
 * @brief Gets the end address of the .dtors DOL section
 */
const void* GetDolDtorsEnd() {
    return &_e_dol_dtors;
}
/**
 * @brief Gets the size of the .dtors DOL section
 */
u32 GetDolDtorsSize() {
    return PtrDistance(GetDolDtorsStart(), GetDolDtorsEnd());
}

/**
 * @brief Gets the start address of the .rodata DOL section
 */
const void* GetDolRodataStart() {
    return &_f_dol_rodata;
}
/**
 * @brief Gets the end address of the .rodata DOL section
 */
const void* GetDolRodataEnd() {
    return &_e_dol_rodata;
}
/**
 * @brief Gets the size of the .rodata DOL section
 */
u32 GetDolRodataSize() {
    return PtrDistance(GetDolRodataStart(), GetDolRodataEnd());
}

/**
 * @brief Gets the start address of the .data DOL section
 */
const void* GetDolDataStart() {
    return &_f_dol_data;
}
/**
 * @brief Gets the end address of the .data DOL section
 */
const void* GetDolDataEnd() {
    return &_e_dol_data;
}
/**
 * @brief Gets the size of the .data DOL section
 */
u32 GetDolDataSize() {
    return PtrDistance(GetDolDataStart(), GetDolDataEnd());
}
/**@}*/

} // namespace

#ifdef __cplusplus
}
#endif

/**
 * @brief Simulate a breakpoint for Dolphin debugging
 */
#define K_DEBUG_BREAK()                                                        \
    {                                                                          \
        BOOL __enabled__ = OSDisableInterrupts();                              \
        K_LOG("************ BREAKPOINT! ************\n");                      \
        K_LOG_EX("Source: " __FILE__ "(%d)\n", __LINE__);                      \
        volatile int __x__ = 1;                                                \
        do {                                                                   \
            ;                                                                  \
        } while (__x__);                                                       \
        OSRestoreInterrupts(__enabled__);                                      \
    }

/**
 * @name Assembly utilities
 */
/**@{*/
/**
 * @brief Begin an inline assembly block
 */
#define K_ASM_BEGIN asm volatile

/**
 * @brief Copy the contents of a GPR to a variable
 * @note Compiler optimizations usually alias the variable to the specified GPR
 */
#define K_GET_GPR(r, var)                                                      \
    K_ASM_BEGIN {                                                              \
        mr var, r;                                                             \
    }
/**
 * @brief Copy the contents of a variable to a GPR
 */
#define K_SET_GPR(r, var)                                                      \
    K_ASM_BEGIN {                                                              \
        mr r, var;                                                             \
    }

/**
 * @brief Create a stack frame and save all GPRs
 */
#define K_SAVE_GPRS                                                            \
    K_ASM_BEGIN {                                                              \
        stwu r1, -0x90(r1);                                                    \
        stmw r3, 0xC(r1);                                                      \
        mflr r12;                                                              \
        stw r12, 0x8(r1);                                                      \
    }

/**
 * @brief Destroy the stack frame and restore all GPRs
 */
#define K_REST_GPRS                                                            \
    K_ASM_BEGIN {                                                              \
        lwz r12, 0x8(r1);                                                      \
        mtlr r12;                                                              \
        lmw r3, 0xC(r1);                                                       \
        addi r1, r1, 0x90;                                                     \
    }
/**@}*/

//! @}
} // namespace kiwi

#endif
