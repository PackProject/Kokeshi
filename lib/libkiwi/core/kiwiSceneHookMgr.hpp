#ifndef LIBKIWI_CORE_SCENE_HOOK_MGR_H
#define LIBKIWI_CORE_SCENE_HOOK_MGR_H
#include <Pack/RPSystem.h>
#include <libkiwi/core/kiwiArray.hpp>
#include <libkiwi/core/kiwiLinkList.hpp>
#include <libkiwi/util/kiwiStaticSingleton.hpp>
#include <types.h>

namespace kiwi {

// Forward declaration
class IScnHook;

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
    void RegistHook(IScnHook* hook, RPSysSceneCreator::ESceneID id) {
        K_ASSERT(hook != NULL);
        mHookLists[id].PushBack(hook);
    }

    /**
     * @brief Unregister existing hook
     *
     * @param hook Scene hook
     * @param id Scene ID
     */
    void UnRegistHook(IScnHook* hook, RPSysSceneCreator::ESceneID id) {
        K_ASSERT(hook != NULL);
        mHookLists[id].Remove(hook);
    }

    static void DoConfigure();
    static void DoLoadResource();
    static void DoCalculate();
    static void DoReset();
    static void DoExit();
    static void DoPause();
    static void DoUnPause();

private:
    /**
     * @brief Gets list of hooks for the current scene
     */
    TList<IScnHook>* GetActiveHooks() {
        s32 id = RPSysSceneMgr::getInstance()->getCurrentSceneID();

        // Ignore custom scenes
        if (id > RPSysSceneCreator::RP_SCENE_MAX) {
            return NULL;
        }

        return &mHookLists[id];
    }

private:
    // Lists of scene hooks
    TArray<TList<IScnHook>, RPSysSceneCreator::RP_SCENE_MAX> mHookLists;
};

/**
 * Scene hook interface for extension of RP scenes
 */
class IScnHook {
public:
    /**
     * @brief Constructor
     *
     * @param id Scene ID
     */
    IScnHook(RPSysSceneCreator::ESceneID id) : mSceneID(id) {
        SceneHookMgr::GetInstance().RegistHook(this, mSceneID);
    }

    /**
     * @brief Destructor
     */
    virtual ~IScnHook() {
        SceneHookMgr::GetInstance().UnRegistHook(this, mSceneID);
    }

    /**
     * @brief Enter scene
     */
    virtual void Configure(RPSysScene* scene) {}

    /**
     * @brief Load scene assets
     */
    virtual void LoadResource(RPSysScene* scene) {}

    /**
     * @brief Logic tick
     */
    virtual void Calculate(RPSysScene* scene) {}

    /**
     * @brief Exit scene
     */
    virtual void Exit(RPSysScene* scene) {}

    /**
     * @brief Pause game
     *
     * @param enter Entering pause menu
     */
    virtual void Pause(RPSysScene* scene, bool enter) {}

private:
    // Scene ID to which this hook belongs
    RPSysSceneCreator::ESceneID mSceneID;
};

} // namespace kiwi

#endif
