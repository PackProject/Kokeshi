#define LIBKIWI_INTERNAL

#include <Pack/RPGraphics.h>
#include <Pack/RPSystem.h>
#include <libkiwi.h>

namespace kiwi {
namespace {

/**
 * @brief Tests whether the current scene is a Pack Project scene
 */
bool IsPackScene() {
    return RP_GET_INSTANCE(RPSysSceneMgr)->getCurrentSceneID() < ESceneID_Max;
}

/**
 * @brief Gets the currently active scene
 */
RPSysScene* GetCurrentScene() {
    return static_cast<RPSysScene*>(
        RP_GET_INSTANCE(RPSysSceneMgr)->getCurrentScene());
}

} // namespace

/**
 * @brief Gets list of hooks for the current scene
 */
TList<ISceneHook>& SceneHookMgr::GetActiveHooks() {
    K_ASSERT_EX(IsPackScene(), "Only game scenes have hooks");
    s32 id = RP_GET_INSTANCE(RPSysSceneMgr)->getCurrentSceneID();
    return mHookLists[id];
}

/**
 * @brief Registers new hook
 *
 * @param rHook Scene hook
 * @param id Scene ID (-1 for all scenes)
 */
void SceneHookMgr::AddHook(ISceneHook& rHook, s32 id) {
    if (id == -1) {
        mGlobalHooks.PushBack(&rHook);
    } else {
        K_ASSERT_EX(id < ESceneID_Max, "Only game scenes have hooks");
        mHookLists[id].PushBack(&rHook);
    }
}

/**
 * @brief Unregisters existing hook
 *
 * @param rHook Scene hook
 * @param id Scene ID (-1 for all scenes)
 */
void SceneHookMgr::RemoveHook(const ISceneHook& rHook, s32 id) {
    if (id == -1) {
        mGlobalHooks.Remove(&rHook);
    } else {
        K_ASSERT_EX(id < ESceneID_Max, "Only game scenes have hooks");
        mHookLists[id].Remove(&rHook);
    }
}

/**
 * @brief Enter state
 */
void SceneHookMgr::DoEnter() {
    GetCurrentScene()->Configure();

    // Global hooks
    K_LIST_FOREACH(GetInstance().mGlobalHooks, ISceneHook,
                   { it->Configure(GetCurrentScene()); });

    // Hooks for game scene
    if (IsPackScene()) {
        K_LIST_FOREACH(GetInstance().GetActiveHooks(), ISceneHook,
                       { it->Configure(GetCurrentScene()); });
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_CALL(0x8018532c, SceneHookMgr::DoEnter),  // Wii Sports
                KM_CALL(0x80184d24, SceneHookMgr::DoEnter),  // Wii Play
                KM_CALL(0x8022fae4, SceneHookMgr::DoEnter)); // Wii Sports Resort
// clang-format on

/**
 * @brief Reset state
 */
void SceneHookMgr::DoReset() {
    // Global hooks
    K_LIST_FOREACH(GetInstance().mGlobalHooks, ISceneHook,
                   { it->BeforeReset(GetCurrentScene()); });
    // Hooks for game scene
    if (IsPackScene()) {
        K_LIST_FOREACH(GetInstance().GetActiveHooks(), ISceneHook,
                       { it->BeforeReset(GetCurrentScene()); });
    }

    GetCurrentScene()->Reset();

    // Global hooks
    K_LIST_FOREACH(GetInstance().mGlobalHooks, ISceneHook,
                   { it->AfterReset(GetCurrentScene()); });
    // Hooks for game scene
    if (IsPackScene()) {
        K_LIST_FOREACH(GetInstance().GetActiveHooks(), ISceneHook,
                       { it->AfterReset(GetCurrentScene()); });
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_CALL(0x801852d8, SceneHookMgr::DoReset),  // Wii Sports
                KM_CALL(0x80184cd0, SceneHookMgr::DoReset),  // Wii Play
                KM_CALL(0x8022f9ac, SceneHookMgr::DoReset)); // Wii Sports Resort
KOKESHI_BY_PACK(KM_CALL(0x801853d4, SceneHookMgr::DoReset),  // Wii Sports
                KM_CALL(0x80184dcc, SceneHookMgr::DoReset),  // Wii Play
                KM_CALL(0x8022fb54, SceneHookMgr::DoReset)); // Wii Sports Resort
// clang-format on

/**
 * @brief LoadResource state
 */
void SceneHookMgr::DoLoadResource() {
    // Global hooks
    K_LIST_FOREACH(GetInstance().mGlobalHooks, ISceneHook,
                   { it->LoadResource(GetCurrentScene()); });

    // Hooks for game scene
    if (IsPackScene()) {
        K_LIST_FOREACH(GetInstance().GetActiveHooks(), ISceneHook,
                       { it->LoadResource(GetCurrentScene()); });
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x8018695c, SceneHookMgr::DoLoadResource),  // Wii Sports
                KM_BRANCH(0x801861fc, SceneHookMgr::DoLoadResource),  // Wii Play
                KM_BRANCH(0x802311ac, SceneHookMgr::DoLoadResource)); // Wii Sports Resort
// clang-format on

/**
 * @brief Calculate state
 */
void SceneHookMgr::DoCalculate() {
    // Global hooks
    K_LIST_FOREACH(GetInstance().mGlobalHooks, ISceneHook,
                   { it->BeforeCalculate(GetCurrentScene()); });
    // Hooks for game scene
    if (IsPackScene()) {
        K_LIST_FOREACH(GetInstance().GetActiveHooks(), ISceneHook,
                       { it->BeforeCalculate(GetCurrentScene()); });
    }

    // Run scene logic
    RP_GET_INSTANCE(RPSysSceneMgr)->updateState();
    RP_GET_INSTANCE(RPSysSceneMgr)->SceneManager::calcCurrentScene();

    // Global hooks
    K_LIST_FOREACH(GetInstance().mGlobalHooks, ISceneHook,
                   { it->AfterCalculate(GetCurrentScene()); });
    // Hooks for game scene
    if (IsPackScene()) {
        K_LIST_FOREACH(GetInstance().GetActiveHooks(), ISceneHook,
                       { it->AfterCalculate(GetCurrentScene()); });
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80185868, SceneHookMgr::DoCalculate),  // Wii Sports
                KM_BRANCH(0x801851f0, SceneHookMgr::DoCalculate),  // Wii Play
                KM_BRANCH(0x8023016c, SceneHookMgr::DoCalculate)); // Wii Sports Resort
// clang-format on

/**
 * @brief Exit state
 */
void SceneHookMgr::DoExit() {
    // Global hooks
    K_LIST_FOREACH(GetInstance().mGlobalHooks, ISceneHook,
                   { it->Exit(GetCurrentScene()); });

    // Hooks for game scene
    if (IsPackScene()) {
        K_LIST_FOREACH(GetInstance().GetActiveHooks(), ISceneHook,
                       { it->Exit(GetCurrentScene()); });
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80185000, SceneHookMgr::DoExit),  // Wii Sports
                KM_BRANCH(0x801849f8, SceneHookMgr::DoExit),  // Wii Play
                KM_BRANCH(0x8022f6f4, SceneHookMgr::DoExit)); // Wii Sports Resort
// clang-format on

/**
 * @brief Pause state
 */
void SceneHookMgr::DoPause() {
    // Global hooks
    K_LIST_FOREACH(GetInstance().mGlobalHooks, ISceneHook,
                   { it->Pause(GetCurrentScene(), true); });

    // Hooks for game scene
    if (IsPackScene()) {
        K_LIST_FOREACH(GetInstance().GetActiveHooks(), ISceneHook,
                       { it->Pause(GetCurrentScene(), true); });
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x801b68ec, SceneHookMgr::DoPause), // Wii Sports
                KM_BRANCH(0x801b3174, SceneHookMgr::DoPause), // Wii Play
                );                                            // Wii Sports Resort
// clang-format on

/**
 * @brief Un-pause state
 */
void SceneHookMgr::DoUnPause() {
    // Global hooks
    K_LIST_FOREACH(GetInstance().mGlobalHooks, ISceneHook,
                   { it->Pause(GetCurrentScene(), false); });

    // Hooks for game scene
    if (IsPackScene()) {
        K_LIST_FOREACH(GetInstance().GetActiveHooks(), ISceneHook,
                       { it->Pause(GetCurrentScene(), false); });
    }
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x801b6840, SceneHookMgr::DoUnPause), // Wii Sports
                KM_BRANCH(0x801b30c8, SceneHookMgr::DoUnPause), // Wii Play
                );                                              // Wii Sports Resort
// clang-format on

} // namespace kiwi
