#ifndef LIBKIWI_CORE_SCENE_HOOK_MGR_H
#define LIBKIWI_CORE_SCENE_HOOK_MGR_H
#include <Pack/RPSystem.h>
#include <libkiwi/core/kiwiSceneCreator.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiArray.h>
#include <libkiwi/prim/kiwiLinkList.h>
#include <libkiwi/util/kiwiStaticSingleton.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

// Forward declarations
class ISceneHook;

/**
 * @brief Scene hook manager
 */
class SceneHookMgr : public StaticSingleton<SceneHookMgr> {
    friend class StaticSingleton<SceneHookMgr>;

public:
    /**
     * @brief Registers new hook
     *
     * @param rHook Scene hook
     * @param id Scene ID (-1 for all scenes)
     */
    void AddHook(ISceneHook& rHook, s32 id);

    /**
     * @brief Unregisters existing hook
     *
     * @param rHook Scene hook
     * @param id Scene ID (-1 for all scenes)
     */
    void RemoveHook(const ISceneHook& rHook, s32 id);

private:
    LIBKIWI_KAMEK_PUBLIC

    /**
     * @brief Enter state
     */
    static void DoEnter();
    /**
     * @brief Reset state
     */
    static void DoReset();
    /**
     * @brief LoadResource state
     */
    static void DoLoadResource();
    /**
     * @brief Calculate state
     */
    static void DoCalculate();
    /**
     * @brief Exit state
     */
    static void DoExit();
    /**
     * @brief Pause state
     */
    static void DoPause();
    /**
     * @brief Un-pause state
     */
    static void DoUnPause();

    /**
     * @brief Gets list of hooks for the current scene
     */
    TList<ISceneHook>& GetActiveHooks();

private:
    TArray<TList<ISceneHook>, ESceneID_Max> mHookLists; // Lists of scene hooks
    TList<ISceneHook> mGlobalHooks; // Global hooks (always active)
};

/**
 * @brief Scene hook interface
 */
class ISceneHook {
public:
    /**
     * @brief Constructor
     *
     * @param id Scene ID (-1 for all scenes)
     */
    explicit ISceneHook(s32 id) : mSceneID(id) {
        K_ASSERT_EX(id == -1 || id < ESceneID_Max,
                    "Only RP scenes and -1 (all) are supported");

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
     * @details Ran once on initial scene setup
     *
     * @param pScene Current scene
     */
    virtual void Configure(RPSysScene* pScene) {}

    /**
     * @brief LoadResource callback
     * @details Ran once on asset loading
     *
     * @param pScene Current scene
     */
    virtual void LoadResource(RPSysScene* pScene) {}

    /**
     * @brief Reset callback (before game logic)
     * @details Ran once on initial scene setup and on every restart
     *
     * @param pScene Current scene
     */
    virtual void BeforeReset(RPSysScene* pScene) {}
    /**
     * @brief Reset callback (after game logic)
     * @details Ran once on initial scene setup and on every restart
     *
     * @param pScene Current scene
     */
    virtual void AfterReset(RPSysScene* pScene) {}

    /**
     * @brief Calculate callback (before game logic)
     * @details Ran once per frame
     *
     * @param pScene Current scene
     */
    virtual void BeforeCalculate(RPSysScene* pScene) {}
    /**
     * @brief Calculate callback (after game logic)
     * @details Ran once per frame
     *
     * @param pScene Current scene
     */
    virtual void AfterCalculate(RPSysScene* pScene) {}

    /**
     * @brief Exit callback
     * @details Ran once on scene exit (including restarts)
     *
     * @param pScene Current scene
     */
    virtual void Exit(RPSysScene* pScene) {}

    /**
     * @brief Pause callback
     * @details Ran on pause menu open/close
     *
     * @param pScene Current scene
     * @param enter Whether the pause menu is opening
     */
    virtual void Pause(RPSysScene* pScene, bool enter) {}

private:
    s32 mSceneID; // Scene to which this hook belongs
};

//! @}
} // namespace kiwi

#endif
