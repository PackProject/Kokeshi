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

    kiwi::GeckoDebugger::CreateInstance();
    kiwi::GeckoDebugger::GetInstance().Attach();
#endif

    kiwi::LibSO::Initialize(); // Initialize socket library

    // ====================================================
    // Your code goes here!
    kiwi::cout << "Hello world!" << kiwi::endl;
    // ====================================================

    // Enter first scene
    kiwi::SceneCreator::GetInstance().ChangeSceneAfterFade(
        kiwi::ESceneID_RPSysBootScene);

    RP_GET_INSTANCE(RPSysSystem)->mainLoop(); // Enter game loop
    ASSERT(false); // Main function should never return
}
KOKESHI_BY_PACK(KM_CALL(0x80183b6c, KokeshiMain), // Wii Sports
                KM_CALL(0x80183784, KokeshiMain), // Wii Play
                KOKESHI_NOTIMPLEMENTED);          // Wii Sports Resort
