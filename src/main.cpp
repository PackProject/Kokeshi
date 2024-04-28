#include <Pack/RPSystem.h>
#include <kokeshi.hpp>
#include <libkiwi.h>

/**
 * Mod entrypoint
 */
void KokeshiMain() {
#ifndef NDEBUG
    // Setup libkiwi debugging utilities
    kiwi::Nw4rException::CreateInstance();
    kiwi::MapFile::CreateInstance();
    kiwi::MapFile::GetInstance().Open(kokeshi::scMapfilePath,
                                      kiwi::MapFile::ELinkType_Relocatable);
#endif

    kiwi::LibSO::Initialize();            // Initialize socket library
    kiwi::ImGuiImplRvl::CreateInstance(); // Initialize ImGui

    // ====================================================
    // Your code goes here!
    kiwi::cout << "Hello world!" << kiwi::endl;
    // ====================================================

    RPSysSystem::getInstance()->mainLoop(); // Enter game loop
    ASSERT(false);                          // Main function should never return
}
KOKESHI_BY_PACK(KM_BRANCH(0x80183f04, KokeshiMain), // Wii Sports
                KM_BRANCH(0x80183b3c, KokeshiMain), // Wii Play
                KOKESHI_NOTIMPLEMENTED);            // Wii Sports Resort
