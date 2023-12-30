/*
 * Kamek
 * Wii game patching engine
 * (c) Treeki 2010-2018
 */

#ifndef KAMEK_HOOKS_H
#define KAMEK_HOOKS_H
#include <kamekTypes.hpp>

// allow Kamek hooks to be defined from C++ source files
#pragma section ".kamek"

// hook type IDs _must_ match what's in the Kamek source!
#define KCT_WRITE 1
#define KCT_CONDITIONAL_WRITE 2
#define KCT_INJECT_BRANCH 3
#define KCT_INJECT_CALL 4
#define KCT_PATCH_EXIT 5

// hook type IDs _must_ match what's in the Kamek source!
// clang-format off
#define KM_IDENTIFIER(key, counter) _k##key##counter
#define KM_HOOK_INT(counter) __declspec(section ".kamek") static const u32 KM_IDENTIFIER(Hook, counter)
// clang-format on

// general hook definition macros
// clang-format off
#define KM_HOOK_0(type)                                                        \
    KM_HOOK_INT(__COUNTER__)[2] = {0, (type)}
#define KM_HOOK_1(type, arg0)                                                  \
    KM_HOOK_INT(__COUNTER__)[3] = {1, (type), (u32)(arg0)}
#define KM_HOOK_2(type, arg0, arg1)                                            \
    KM_HOOK_INT(__COUNTER__)[4] = {2, (type), (u32)(arg0), (u32)(arg1)}
#define KM_HOOK_3(type, arg0, arg1, arg2)                                      \
    KM_HOOK_INT(__COUNTER__)[5] = {3, (type), (u32)(arg0), (u32)(arg1), (u32)(arg2)}
#define KM_HOOK_4(type, arg0, arg1, arg2, arg3)                                \
    KM_HOOK_INT(__COUNTER__)[6] = {4, (type), (u32)(arg0), (u32)(arg1), (u32)(arg2), (u32)(arg3)}
// clang-format on

// KM_COND_WRITE
//   Write value to address, conditionally
#define KM_COND_WRITE_POINTER(addr, original, value)                           \
    KM_HOOK_4(KCT_CONDITIONAL_WRITE, 1, (addr), (value), (original))
#define KM_COND_WRITE_32(addr, original, value)                                \
    KM_HOOK_4(KCT_CONDITIONAL_WRITE, 2, (addr), (value), (original))
#define KM_COND_WRITE_16(addr, original, value)                                \
    KM_HOOK_4(KCT_CONDITIONAL_WRITE, 3, (addr), (value), (original))
#define KM_COND_WRITE_8(addr, original, value)                                 \
    KM_HOOK_4(KCT_CONDITIONAL_WRITE, 4, (addr), (value), (original))

// KM_WRITE
//   Write value to address
// clang-format off
#define KM_WRITE_POINTER(addr, ptr) KM_HOOK_3(KCT_WRITE, 1, (addr), (ptr))
#define KM_WRITE_32(addr, value)    KM_HOOK_3(KCT_WRITE, 2, (addr), (value))
#define KM_WRITE_16(addr, value)    KM_HOOK_3(KCT_WRITE, 3, (addr), (value))
#define KM_WRITE_8(addr, value)     KM_HOOK_3(KCT_WRITE, 4, (addr), (value))
// clang-format on

// KM_PATCH_EXIT_POINT
//   Force the end of a Kamek function to always jump to a specific address
//   (if the address is 0, the end remains as-is (i.e. blr))
#define KM_PATCH_EXIT_POINT(funcStart, dest)                                   \
    KM_HOOK_2(KCT_PATCH_EXIT, (funcStart), (dest))

// KM_BRANCH, KM_CALL
//   Set up a branch from a specific instruction to a specific address
#define KM_BRANCH(addr, ptr) KM_HOOK_2(KCT_INJECT_BRANCH, (addr), (ptr))
#define KM_CALL(addr, ptr) KM_HOOK_2(KCT_INJECT_CALL, (addr), (ptr))

// KM_BRANCH_DEF_CPP, KM_BRANCH_DEF_ASM
//   Set up a branch (b) from a specific instruction to a function defined
//   directly underneath. If exitPoint is not NULL, the function will
//   branch to exitPoint when done; otherwise, it executes blr as normal
#define KM_BRANCHDefInt(counter, addr, exitPoint, returnType, ...)             \
    returnType KM_IDENTIFIER(UserFunc, counter)(__VA_ARGS__);                  \
    KM_BRANCH(addr, KM_IDENTIFIER(UserFunc, counter));                         \
    KM_PATCH_EXIT_POINT(KM_IDENTIFIER(UserFunc, counter), exitPoint);          \
    returnType KM_IDENTIFIER(UserFunc, counter)(__VA_ARGS__)

#define KM_BRANCH_DEF_CPP(addr, exitPoint, returnType, ...)                    \
    KM_BRANCHDefInt(__COUNTER__, addr, exitPoint, returnType, __VA_ARGS__)
#define KM_BRANCH_DEF_ASM(addr, exitPoint)                                     \
    KM_BRANCHDefInt(__COUNTER__, addr, exitPoint, asm void, )

// KM_CALL_DEF_CPP, KM_CALL_DEF_ASM
//   Set up a branch with link (bl) from a specific instruction to a function
//   defined directly underneath.
#define KM_CALL_DEF_INT(counter, addr, returnType, ...)                        \
    returnType KM_IDENTIFIER(UserFunc, counter)(__VA_ARGS__);                  \
    KM_CALL(addr, KM_IDENTIFIER(UserFunc, counter));                           \
    returnType KM_IDENTIFIER(UserFunc, counter)(__VA_ARGS__)

#define KM_CALL_DEF_CPP(addr, returnType, ...)                                 \
    KM_CALL_DEF_INT(__COUNTER__, addr, returnType, __VA_ARGS__)
#define KM_CALL_DEF_ASM(addr) KM_CALL_DEF_INT(__COUNTER__, addr, asm void, )

// KM_INJECT_MF
//   Create dummy function to call a member function.
#define _KM_INJECT_MF(cls, func)                                               \
    UNKWORD KM_TRAMP_##cls##_##func(void* arg, ...) {                          \
        typedef UNKWORD (cls::*cls##_fun_t)(...);                              \
        const cls##_fun_t mem_fun = (cls##_fun_t)(&cls::func);                 \
        cls* self = reinterpret_cast<cls*>(arg);                               \
        return (self->*mem_fun)();                                             \
    }

// KM_BRANCH_MF, KM_CALL_MF
//   Set up a branch from a specific instruction to a specific address (member
//   function).
#define KM_BRANCH_MF(addr, cls, func)                                          \
    _KM_INJECT_MF(cls, func);                                                  \
    KM_BRANCH(addr, KM_TRAMP_##cls##_##func);
#define KM_CALL_MF(addr, cls, func)                                            \
    _KM_INJECT_MF(cls, func);                                                  \
    KM_CALL(addr, KM_TRAMP_##cls##_##func);

#endif
