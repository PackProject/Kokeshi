#include <kokeshi.hpp>
#include <libkiwi.h>
#include <types.h>

namespace kokeshi {

/**
 * Mod entrypoint
 */
void entry() {
    // Setup libkiwi debugging utilities
#ifndef NDEBUG
    kiwi::Nw4rException::CreateInstance();
    kiwi::MapFile::CreateInstance();
    kiwi::MapFile::GetInstance().Open(kokeshi::scMapfilePath,
                                      kiwi::MapFile::LinkType_Relocatable);
#endif

    // Setup everything else
    kiwi::SceneHookMgr::CreateInstance();

    // Enter game loop
    RPSysSceneCreator::getInstance()->changeSceneAfterFade(
        RPSysSceneCreator::RP_BOOT_SCENE, false);
    RPSysSystem::getInstance()->setCallBack();
    RPSysSystem::getInstance()->mainLoop();

    // Unreachable
    ASSERT(false);
}
KM_BRANCH(KOKESHI_BY_PACK(0x80183b6c, NULL, NULL), entry);

} // namespace kokeshi
