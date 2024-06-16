// Includes libkiwi but also Kamek and Kokeshi macros,
// which are required for code hooks.
#include <libkiwi.h>

//=============================================================================
// Hook Types
//=============================================================================
//
// There are different macros to use, depending on how *exactly* you want to
// perform the code hook.
//
// The two most common types are the branch hook (KM_BRANCH), and the call hook
// (KM_CALL).
//
// KM_BRANCH redirects execution to your code via a branch (b) instruction,
// while the KM_CALL hook uses a branch-and-link (bl) instruction.
//
// You provide the address of the instruction that you wish to overwrite,
// and then the function you would like to jump to:
//

int dummy() {
    return 0;
}

KM_BRANCH(0x80004000, dummy);
KM_CALL(0x80004000, dummy);

//
// You can generally use either of these hooks, but if you are overwriting any
// type of branch instruction, you should use the matching hook type.
//
// Overwriting a 'b' instruction? Use KM_BRANCH.
// Overwriting a 'bl' instruction? Use KM_CALL.
//

//=============================================================================
// Member Functions
//=============================================================================
//
// In C++, member functions cannot be resolved to a void*, and thus cannot be
// used with KM_BRANCH/KM_CALL.
//
// Kokeshi adds two new macros to allow for member function overrides:
// KM_BRANCH_MF, and KM_CALL_MF.
//
// When invoking the macro, you provide the address of the instruction that you
// wish to override, the name of your class, and the name of your class's
// function that you would like to jump to:
//

class Dummy {
public:
    void doSomething() const;
};

KM_BRANCH_MF(0x80004000, Dummy, doSomething);
KM_CALL_MF(0x80004000, Dummy, doSomething);

//
// NOTE: These member function hooks can only apply to functions marked as
// "public" access.
//

//=============================================================================
// KM_WRITE
//=============================================================================
//
// If you just want to overwrite an instruction or two, and don't want to write
// a C/C++ function, Kamek provides the KM_WRITE family of macros:
//
// KM_WRITE_POINTER(addr, ptr) - Write the pointer 'addr' to the address 'ptr'
// KM_WRITE_32(addr, value) - Write the 32-bit value 'addr' to the address 'ptr'
// KM_WRITE_16(addr, value) - Write the 16-bit value 'addr' to the address 'ptr'
// KM_WRITE_8(addr, value) - Write the 8-bit value 'addr' to the address 'ptr'
//

// Writes a NOP instruction to the address 0x80004000.
KM_WRITE_32(0x80004000, 0x60000000);

//=============================================================================
// Kokeshi Hooks
//=============================================================================
//
// Need to use a different address for each game? No problem!
// Kokeshi offers two ways to accomplish this.
//
// 1. The KOKESHI_BY_PACK macro allows you to specify C/C++
// expressions/statements to compile based on the game your mod is being
// compiled for.
//
KOKESHI_BY_PACK(KM_WRITE_32(0x81114000, 0x60000000), /* Wii Sports */
                KM_WRITE_32(0x82224000, 0x60000000), /* Wii Play */
                KM_WRITE_32(0x83334000, 0x60000000)) /* Wii Sports Resort */
                                                     //
// 2. For larger changes, you can check for the following preprocessor
// directives:
//
// - PACK_SPORTS when compiling for Wii Sports
// - PACK_PLAY when compiling for Wii Play
// - PACK_RESORT when compiling for Wii Sports Resort
//
#ifdef PACK_SPORTS
// your code here . . .
#elif PACK_PLAY
// your code here . . .
#elif PACK_RESORT
// your code here . . .
#endif

//=============================================================================
// More Information
//=============================================================================
//
// For more information, please see loader/kamek/kamekHooks.hpp.
//
// The important hook types are covered in this example, but Kamek offers
// much more functionality!
//
