#ifndef LIBKIWI_CORE_SCENE_HOOK_MGR_H
#define LIBKIWI_CORE_SCENE_HOOK_MGR_H
#include <Pack/RPSystem.h>
#include <libkiwi/core/kiwiArray.hpp>
#include <libkiwi/core/kiwiLinkList.hpp>
#include <libkiwi/util/kiwiStaticSingleton.hpp>
#include <types.h>

namespace kiwi {

// Forward declaration
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
     * @param id Scene ID
     */
    void AddHook(ISceneHook* hook, RPSysSceneCreator::ESceneID id) {
        K_ASSERT(hook != NULL);
        mHookLists[id].PushBack(hook);
    }

    /**
     * @brief Unregister existing hook
     *
     * @param hook Scene hook
     * @param id Scene ID
     */
    void RemoveHook(ISceneHook* hook, RPSysSceneCreator::ESceneID id) {
        K_ASSERT(hook != NULL);
        mHookLists[id].Remove(hook);
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
        s32 id = RPSysSceneMgr::getInstance()->getCurrentSceneID();

        // Ignore custom scenes
        if (id > RPSysSceneCreator::RP_SCENE_MAX) {
            return NULL;
        }

        return &mHookLists[id];
    }

private:
    // Lists of scene hooks
    TArray<TList<ISceneHook>, RPSysSceneCreator::RP_SCENE_MAX> mHookLists;
};

/**
 * Scene hook interface for extension of RP scenes
 */
class ISceneHook {
public:
    /**
     * @brief Constructor
     *
     * @param id Scene ID
     */
    ISceneHook(RPSysSceneCreator::ESceneID id) : mSceneID(id) {
        SceneHookMgr::GetInstance().AddHook(this, mSceneID);
    }

    /**
     * @brief Destructor
     */
    virtual ~ISceneHook() {
        SceneHookMgr::GetInstance().RemoveHook(this, mSceneID);
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
    // Scene ID to which this hook belongs
    RPSysSceneCreator::ESceneID mSceneID;
};

} // namespace kiwi

#endif
