#include <Pack/RPSystem.h>

#include <libkiwi.h>

#include <kokeshi.hpp>

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
#endif

    // Initialize network socket system
    kiwi::LibSO::Initialize();

    // ====================================================
    // Your code goes here!
    kiwi::cout << "Hello world!" << kiwi::endl;
    // ====================================================

    // Enter first scene
    kiwi::SceneCreator::GetInstance().ChangeBootScene();

    // Enter game loop
    RP_GET_INSTANCE(RPSysSystem)->mainLoop();

    // Main function should never return
    ASSERT(false);
}
KOKESHI_BY_PACK(KM_CALL(0x80183b6c, KokeshiMain),  // Wii Sports
                KM_CALL(0x80183784, KokeshiMain),  // Wii Play
                KM_CALL(0x8022df10, KokeshiMain)); // Wii Sports Resort
