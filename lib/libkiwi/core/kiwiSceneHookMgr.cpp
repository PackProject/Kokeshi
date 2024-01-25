#include <Pack/RPGraphics.h>
#include <Pack/RPSystem.h>
#include <kokeshi.hpp>
#include <libkiwi.h>

namespace kiwi {

namespace {

/**
 * Gets the current scene
 */
RPSysScene* GetCurrentScene() {
    return static_cast<RPSysScene*>(
        RPSysSceneMgr::getInstance()->getCurrentScene());
}

/**
 * Gets the current scene's ID
 */
RPSysSceneCreator::ESceneID GetCurrentSceneID() {
    return static_cast<RPSysSceneCreator::ESceneID>(
        GetCurrentScene()->getSceneID());
}

/**
 * Tests whether the scene is an RP scene (non-RP scenes don't have
 * hooks)
 */
bool IsCurrentSceneRP() {
    return GetCurrentSceneID() < RPSysSceneCreator::RP_SCENE_MAX;
}

} // namespace

/**
 * Dispatches scene configure hook
 */
void SceneHookMgr::DoConfigure() {
    // Non-RP scenes don't have hooks
    if (!IsCurrentSceneRP()) {
        return;
    }

    Hook hook = GetInstance().mHooks[GetCurrentSceneID()];
    if (hook.mConfigureFn != NULL) {
        hook.mConfigureFn(GetCurrentScene());
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
    // Non-RP scenes don't have hooks
    if (!IsCurrentSceneRP()) {
        return;
    }

    Hook hook = GetInstance().mHooks[GetCurrentSceneID()];
    if (hook.mLoadResourceFn != NULL) {
        hook.mLoadResourceFn(GetCurrentScene());
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
    // Non-RP scenes don't have hooks
    if (!IsCurrentSceneRP()) {
        return;
    }

    Hook hook = GetInstance().mHooks[GetCurrentSceneID()];
    if (hook.mCalculateFn != NULL) {
        hook.mCalculateFn(GetCurrentScene());
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80185160, SceneHookMgr::DoCalculate), // Wii Sports
                KM_BRANCH(0x80184b58, SceneHookMgr::DoConfigure), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                          // Wii Sports Resort
// clang-format on

/**
 * Dispatches scene user draw callback
 */
void SceneHookMgr::DoUserDraw() {
    // Non-RP scenes don't have hooks
    if (!IsCurrentSceneRP()) {
        return;
    }

    Hook hook = GetInstance().mHooks[GetCurrentSceneID()];
    if (hook.mUserDrawFn != NULL) {
        RPGrpRenderer::Begin();
        hook.mUserDrawFn(GetCurrentScene());
        RPGrpRenderer::End();
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80185090, SceneHookMgr::DoUserDraw), // Wii Sports
                KM_BRANCH(0x80184a88, SceneHookMgr::DoUserDraw), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                         // Wii Sports Resort
// clang-format on

/**
 * Dispatches scene exit callback
 */
void SceneHookMgr::DoExit() {
    // Non-RP scenes don't have hooks
    if (!IsCurrentSceneRP()) {
        return;
    }

    Hook hook = GetInstance().mHooks[GetCurrentSceneID()];
    if (hook.mExitFn != NULL) {
        hook.mExitFn(GetCurrentScene());
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
    // Non-RP scenes don't have hooks
    if (!IsCurrentSceneRP()) {
        return;
    }

    Hook hook = GetInstance().mHooks[GetCurrentSceneID()];
    if (hook.mPauseFn != NULL) {
        hook.mPauseFn(GetCurrentScene(), true);
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
void SceneHookMgr::DoUnpause() {
    // Non-RP scenes don't have hooks
    if (!IsCurrentSceneRP()) {
        return;
    }

    Hook hook = GetInstance().mHooks[GetCurrentSceneID()];
    if (hook.mPauseFn != NULL) {
        hook.mPauseFn(GetCurrentScene(), false);
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x801b6840, SceneHookMgr::DoUnpause), // Wii Sports
                KM_BRANCH(0x801b30c8, SceneHookMgr::DoUnpause), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                        // Wii Sports Resort
// clang-format on

} // namespace kiwi
