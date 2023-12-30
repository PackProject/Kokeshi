#ifndef LIBKIWI_CORE_SCENE_HOOK_MGR_H
#define LIBKIWI_CORE_SCENE_HOOK_MGR_H
#include <Pack/RPSystem.h>
#include <libkiwi/core/kiwiArray.hpp>
#include <libkiwi/util/kiwiDynamicSingleton.hpp>
#include <types.h>

namespace kiwi {

/**
 * Scene hook manager
 */
class SceneHookMgr : public DynamicSingleton<SceneHookMgr> {
    friend class DynamicSingleton<SceneHookMgr>;

public:
    /**
     * Scene "hook" for extension of RP scenes
     */
    class Hook {
        friend class SceneHookMgr;

    public:
        /**
         * Scene callback
         * @param scene Current scene
         */
        typedef void (*SceneFn)(RPSysScene* scene);

        /**
         * Pause callback
         * @param scene Current scene
         * @param status If entering pause state
         */
        typedef void (*PauseFn)(RPSysScene* scene, bool status);

    public:
        /**
         * Constructor
         */
        Hook(SceneFn configure, SceneFn loadResource, SceneFn calculate,
             SceneFn userDraw, SceneFn exit, PauseFn pause)
            : mConfigureFn(configure),
              mLoadResourceFn(loadResource),
              mCalculateFn(calculate),
              mUserDrawFn(userDraw),
              mExitFn(exit),
              mPauseFn(pause) {}

        /**
         * Constructor
         */
        Hook() {
            Hook(NULL, NULL, NULL, NULL, NULL, NULL);
        }

    private:
        // Configure callback
        SceneFn mConfigureFn;
        // LoadResource callback
        SceneFn mLoadResourceFn;
        // Calculate callback
        SceneFn mCalculateFn;
        // UserDraw callback
        SceneFn mUserDrawFn;
        // Exit callback
        SceneFn mExitFn;
        // Pause callback
        PauseFn mPauseFn;
    };

public:
    /**
     * Sets scene hook
     *
     * @param id Scene ID
     * @param hook Hook functions
     */
    void SetHook(RPSysSceneCreator::ESceneID id, const Hook hook) {
        mHooks[id] = hook;
    }

    static void DoConfigure();
    static void DoLoadResource();
    static void DoCalculate();
    static void DoUserDraw();
    static void DoExit();
    static void DoPause();
    static void DoUnpause();

private:
    // RP scene hooks
    TArray<Hook, RPSysSceneCreator::RP_SCENE_MAX> mHooks;
};

} // namespace kiwi

#endif
