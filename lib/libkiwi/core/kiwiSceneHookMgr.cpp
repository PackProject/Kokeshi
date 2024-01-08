#include <Pack/RPGraphics.h>
#include <Pack/RPSystem.h>
#include <libkiwi.h>

namespace kiwi {

K_DYNAMIC_SINGLETON_IMPL(SceneHookMgr);

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
KM_BRANCH(0x800a5190, SceneHookMgr::DoConfigure);

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
KM_BRANCH(0x8018695c, SceneHookMgr::DoLoadResource);

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
KM_BRANCH(0x80185160, SceneHookMgr::DoCalculate);

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
KM_BRANCH(0x80185090, SceneHookMgr::DoUserDraw);

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
KM_BRANCH(0x80185000, SceneHookMgr::DoExit);

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
KM_BRANCH(0x801b68ec, SceneHookMgr::DoPause);

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
KM_BRANCH(0x801b6840, SceneHookMgr::DoUnpause);

} // namespace kiwi
