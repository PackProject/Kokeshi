#include <Pack/RPAudio.h>
#include <Pack/RPGraphics.h>
#include <Pack/RPKernel.h>
#include <Pack/RPSports.h>
#include <Pack/RPSystem.h>
#include <Pack/RPUtility.h>
#include <kokeshi.hpp>
#include <libkiwi.h>
#include <types.h>

/**
 * Mod entrypoint
 */
void KokeshiMain() {
    // Setup libkiwi debugging utilities
#ifndef NDEBUG
    kiwi::Nw4rException::CreateInstance();
    kiwi::MapFile::CreateInstance();
    kiwi::MapFile::GetInstance().Open(kokeshi::scMapfilePath,
                                      kiwi::MapFile::LinkType_Relocatable);
#endif
    // Your code goes here!

    // Enter game loop
    RPSysSystem::getInstance()->mainLoop();
    // Main function should never return
    ASSERT(false);
}
KOKESHI_BY_PACK(KM_BRANCH(0x80183f04, KokeshiMain), // Wii Sports
                KOKESHI_BY_PACK_NOOP,               // Wii Play
                KOKESHI_BY_PACK_NOOP);              // Wii Sports Resort
