#ifndef LIBKIWI_CORE_SCENE_HOOK_MGR_H
#define LIBKIWI_CORE_SCENE_HOOK_MGR_H
#include <Pack/RPSystem.h>
#include <libkiwi/core/kiwiSceneCreator.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiArray.h>
#include <libkiwi/prim/kiwiLinkList.h>
#include <libkiwi/util/kiwiStaticSingleton.h>

namespace kiwi {

// Forward declarations
class ISceneHook;

/**
 * Scene hook manager
 */
class SceneHookMgr : public StaticSingleton<SceneHookMgr> {
    friend class StaticSingleton<SceneHookMgr>;

public:
    /**
     * @brief Register new hook
     *
     * @param hook Scene hook
     * @param id Scene ID (-1 for all scenes)
     */
    void AddHook(ISceneHook& hook, s32 id) {
        if (id == -1) {
            mGlobalHooks.PushBack(&hook);
        } else {
            K_ASSERT(id < ESceneID_Max);
            mHookLists[id].PushBack(&hook);
        }
    }

    /**
     * @brief Unregister existing hook
     *
     * @param hook Scene hook
     * @param id Scene ID (-1 for all scenes)
     */
    void RemoveHook(ISceneHook& hook, s32 id) {
        if (id == -1) {
            mGlobalHooks.Remove(&hook);
        } else {
            K_ASSERT(id < ESceneID_Max);
            mHookLists[id].Remove(&hook);
        }
    }

    static void OnSceneEnter();
    static void OnSceneReEnter();
    static void OnSceneLoadResource();
    static void OnSceneCalculate();
    static void OnSceneExit();
    static void OnScenePause();
    static void OnSceneUnPause();

private:
    /**
     * @brief Gets list of hooks for the current scene
     */
    TList<ISceneHook>* GetActiveHooks() {
        s32 id = RP_GET_INSTANCE(RPSysSceneMgr)->getCurrentSceneID();

        // Ignore custom scenes
        if (id >= ESceneID_Max) {
            return NULL;
        }

        return &mHookLists[id];
    }

private:
    TArray<TList<ISceneHook>, ESceneID_Max> mHookLists; // Lists of scene hooks
    TList<ISceneHook> mGlobalHooks; // Global hooks (always active)
};

/**
 * Scene hook interface for extension of RP scenes
 */
class ISceneHook {
public:
    /**
     * @brief Constructor
     *
     * @param id Scene ID (-1 for all scenes)
     */
    explicit ISceneHook(s32 id) : mSceneID(id) {
        K_ASSERT(id == -1 || id < ESceneID_Max);
        SceneHookMgr::GetInstance().AddHook(*this, mSceneID);
    }

    /**
     * @brief Destructor
     */
    virtual ~ISceneHook() {
        SceneHookMgr::GetInstance().RemoveHook(*this, mSceneID);
    }

    /**
     * @brief Configure callback
     */
    virtual void Configure(RPSysScene* scene) {}

    /**
     * @brief Reset callback (before game logic)
     */
    virtual void BeforeReset(RPSysScene* scene) {}

    /**
     * @brief Reset callback (after game logic)
     */
    virtual void AfterReset(RPSysScene* scene) {}

    /**
     * @brief LoadResource callback
     */
    virtual void LoadResource(RPSysScene* scene) {}

    /**
     * @brief Calculate callback (before game logic)
     */
    virtual void BeforeCalculate(RPSysScene* scene) {}

    /**
     * @brief Calculate callback (after game logic)
     */
    virtual void AfterCalculate(RPSysScene* scene) {}

    /**
     * @brief Exit callback
     */
    virtual void Exit(RPSysScene* scene) {}

    /**
     * @brief Pause callback
     *
     * @param enter Whether entering menu
     */
    virtual void Pause(RPSysScene* scene, bool enter) {}

private:
    s32 mSceneID; // Scene to which this hook belongs
};

} // namespace kiwi

#endif
