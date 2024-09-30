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
    kiwi::MapFile::GetInstance().Open(kokeshi::MAPFILE_PATH,
                                      kiwi::MapFile::ELinkType_Relocatable);

    kiwi::GeckoDebugger::CreateInstance();
    kiwi::GeckoDebugger::GetInstance().Attach();
#endif

    kiwi::LibSO::Initialize();

    // ====================================================
    // Your code goes here!
    kiwi::cout << "Hello world!" << kiwi::endl;
    // ====================================================

// Enter first scene
#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    kiwi::SceneCreator::GetInstance().ChangeSceneAfterFade(
        kiwi::ESceneID_RPSysBootScene);
#elif defined(PACK_RESORT)
    kiwi::SceneCreator::GetInstance().ChangeSceneAfterFade(
        kiwi::ESceneID_Sp2StrapScene);
#endif

    RP_GET_INSTANCE(RPSysSystem)->mainLoop(); // Enter game loop
    ASSERT(false); // Main function should never return
}
KOKESHI_BY_PACK(KM_CALL(0x80183b6c, KokeshiMain),  // Wii Sports
                KM_CALL(0x80183784, KokeshiMain),  // Wii Play
                KM_CALL(0x8022df10, KokeshiMain)); // Wii Sports Resort
