#include <Pack/RPGraphics.h>
#include <Pack/RPSystem.h>
#include <kokeshi.hpp>
#include <libkiwi.h>

namespace kiwi {
namespace {

/**
 * @brief Gets the currently active RP scene
 */
RPSysScene* GetCurrentScene() {
    RPSysSceneMgr* m = RPSysSceneMgr::getInstance();
    K_ASSERT(m != NULL);

    return static_cast<RPSysScene*>(m->getCurrentScene());
}

} // namespace

/**
 * @brief Enter callback
 */
void SceneHookMgr::OnSceneEnter() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    // RPSysScene::enter calls both Configure and Reset
    for (TList<IScnHook>::Iterator it = active->Begin(); it != active->End();) {
        it++->Configure(GetCurrentScene());
    }
    for (TList<IScnHook>::Iterator it = active->Begin(); it != active->End();) {
        it++->Reset(GetCurrentScene());
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x801853fc, SceneHookMgr::OnSceneEnter), // Wii Sports
                KM_BRANCH(0x80184df4, SceneHookMgr::OnSceneEnter), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                           // Wii Sports Resort
// clang-format on

/**
 * @brief Re-enter callback
 */
void SceneHookMgr::OnSceneReEnter() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    for (TList<IScnHook>::Iterator it = active->Begin(); it != active->End();) {
        it++->Reset(GetCurrentScene());
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80185300, SceneHookMgr::OnSceneReEnter), // Wii Sports
                KM_BRANCH(0x80184cf8, SceneHookMgr::OnSceneReEnter), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                             // Wii Sports Resort
// clang-format on

/**
 * @brief LoadResource callback
 */
void SceneHookMgr::OnSceneLoadResource() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    for (TList<IScnHook>::Iterator it = active->Begin(); it != active->End();) {
        it++->LoadResource(GetCurrentScene());
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x8018695c, SceneHookMgr::OnSceneLoadResource), // Wii Sports
                KM_BRANCH(0x801861fc, SceneHookMgr::OnSceneLoadResource), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                  // Wii Sports Resort
// clang-format on

/**
 * @brief Calculate callback
 */
void SceneHookMgr::OnSceneCalculate() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();

    // Hooks before scene logic
    if (active != NULL) {
        for (TList<IScnHook>::Iterator it = active->Begin();
             it != active->End();) {
            it++->BeforeCalculate(GetCurrentScene());
        }
    }

    // Run scene logic
    RPSysSceneMgr::getInstance()->updateState();
    RPSysSceneMgr::getInstance()->SceneManager::calcCurrentScene();

    // Hooks after scene logic
    if (active != NULL) {
        for (TList<IScnHook>::Iterator it = active->Begin();
             it != active->End();) {
            it++->AfterCalculate(GetCurrentScene());
        }
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80185868, SceneHookMgr::OnSceneCalculate), // Wii Sports
                KM_BRANCH(0x801851f0, SceneHookMgr::OnSceneCalculate), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                               // Wii Sports Resort
// clang-format on

/**
 * @brief Exit callback
 */
void SceneHookMgr::OnSceneExit() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    for (TList<IScnHook>::Iterator it = active->Begin(); it != active->End();) {
        it++->Exit(GetCurrentScene());
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80185000, SceneHookMgr::OnSceneExit), // Wii Sports
                KM_BRANCH(0x801849f8, SceneHookMgr::OnSceneExit), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                          // Wii Sports Resort
// clang-format on

/**
 * @brief Pause callback
 */
void SceneHookMgr::OnScenePause() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    for (TList<IScnHook>::Iterator it = active->Begin(); it != active->End();) {
        it++->Pause(GetCurrentScene(), true);
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x801b68ec, SceneHookMgr::OnScenePause), // Wii Sports
                KM_BRANCH(0x801b3174, SceneHookMgr::OnScenePause), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                           // Wii Sports Resort
// clang-format on

/**
 * @brief Unpause callback
 */
void SceneHookMgr::OnSceneUnPause() {
    TList<IScnHook>* active = GetInstance().GetActiveHooks();
    if (active == NULL) {
        return;
    }

    for (TList<IScnHook>::Iterator it = active->Begin(); it != active->End();) {
        it++->Pause(GetCurrentScene(), false);
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x801b6840, SceneHookMgr::OnSceneUnPause), // Wii Sports
                KM_BRANCH(0x801b30c8, SceneHookMgr::OnSceneUnPause), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                             // Wii Sports Resort
// clang-format on

} // namespace kiwi
