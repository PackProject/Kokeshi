#include <Pack/RPGraphics.h>
#include <Pack/RPSystem.h>
#include <kokeshi.hpp>
#include <libkiwi.h>

namespace kiwi {
namespace {

/**
 * @brief Gets the currently active scene (as the RP type)
 */
RPSysScene* GetCurrentScene() {
    RPSysSceneMgr* m = RPSysSceneMgr::getInstance();
    K_ASSERT(m != NULL);

    return static_cast<RPSysScene*>(m->getCurrentScene());
}

} // namespace

/**
 * Dispatches scene configure hook
 */
void SceneHookMgr::DoConfigure() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    TList<IScnHook>::Iterator it = active->Begin();
    while (it != active->End()) {
        it++->Configure(GetCurrentScene());
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x800a5190, SceneHookMgr::DoConfigure), // Wii Sports
                KM_BRANCH(0x800a5944, SceneHookMgr::DoConfigure), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                          // Wii Sports Resort
// clang-format on

/**
 * Dispatches scene load resource hook
 */
void SceneHookMgr::DoLoadResource() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    TList<IScnHook>::Iterator it = active->Begin();
    while (it != active->End()) {
        it++->LoadResource(GetCurrentScene());
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x8018695c, SceneHookMgr::DoLoadResource), // Wii Sports
                KM_BRANCH(0x801861fc, SceneHookMgr::DoLoadResource), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                             // Wii Sports Resort
// clang-format on

/**
 * Dispatches scene calculate hook
 */
void SceneHookMgr::DoCalculate() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    TList<IScnHook>::Iterator it = active->Begin();
    while (it != active->End()) {
        it++->Calculate(GetCurrentScene());
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80185160, SceneHookMgr::DoCalculate), // Wii Sports
                KM_BRANCH(0x80184b58, SceneHookMgr::DoCalculate), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                          // Wii Sports Resort
// clang-format on

/**
 * Dispatches scene exit callback
 */
void SceneHookMgr::DoExit() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    TList<IScnHook>::Iterator it = active->Begin();
    while (it != active->End()) {
        it++->Exit(GetCurrentScene());
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80185000, SceneHookMgr::DoExit), // Wii Sports
                KM_BRANCH(0x801849f8, SceneHookMgr::DoExit), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                     // Wii Sports Resort
// clang-format on

/**
 * Dispatches scene pause callback
 */
void SceneHookMgr::DoPause() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    TList<IScnHook>::Iterator it = active->Begin();
    while (it != active->End()) {
        it++->Pause(GetCurrentScene(), true);
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x801b68ec, SceneHookMgr::DoPause), // Wii Sports
                KM_BRANCH(0x801b3174, SceneHookMgr::DoPause), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                      // Wii Sports Resort
// clang-format on

/**
 * Dispatches scene unpause callback
 */
void SceneHookMgr::DoUnPause() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    TList<IScnHook>::Iterator it = active->Begin();
    while (it != active->End()) {
        it++->Pause(GetCurrentScene(), false);
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x801b6840, SceneHookMgr::DoUnPause), // Wii Sports
                KM_BRANCH(0x801b30c8, SceneHookMgr::DoUnPause), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                        // Wii Sports Resort
// clang-format on

} // namespace kiwi
