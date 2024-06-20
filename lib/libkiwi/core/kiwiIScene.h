#ifndef LIBKIWI_CORE_I_SCENE_H
#define LIBKIWI_CORE_I_SCENE_H
#include <Pack/RPSystem.h>
#include <libkiwi/core/kiwiSceneCreator.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>

/**
 * @brief Declare new scene to the creator
 *
 * @param T Scene class type
 */
#define K_SCENE_DECL(T) static kiwi::SceneDecl<T> SceneDecl_##T;

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief User scene interface
 */
class IScene : public RPSysScene {
    //! Needs to see 'Create' method
    template <typename> friend class SceneDecl;

public:
    /**
     * @brief Constructor
     */
    IScene() {}

    /**
     * @brief Destructor
     */
    virtual ~IScene() {}

    /**
     * @name Configuration
     * @brief Scene settings
     */
    /**@{*/
    /**
     * @brief Gets the scene's name
     */
    virtual String GetName() const {
        return "Dummy";
    }

    /**
     * @brief Gets the scene's resource directory name
     */
    virtual String GetDirectory() const {
        return "RPCommon/";
    }

    /**
     * @brief Gets the scene's ID
     */
    virtual s32 GetID() const = 0;

    /**
     * @brief Gets the scene's target pack
     */
    virtual EPackID GetPack() const {
        return EPackID_AllPack;
    }

    /**
     * @brief Gets the scene's create type
     */
    virtual ECreateType GetCreateType() const {
        return ECreateType_Sibling;
    }

    /**
     * @brief Gets the scene's exit type
     */
    virtual EExitType GetExitType() const {
        return EExitType_Standard;
    }

    /**
     * @brief Tests whether the scene requires the RP common sound archive
     */
    virtual bool GetCommonSound() const {
        return true;
    }

#ifdef PACK_RESORT
    /**
     * @brief Gets the scene's type
     */
    virtual RPSysScene::EKind getKind() {
        return RPSysScene::EKind_System;
    }

    /**
     * @brief Gets the scene's target time of day for Wuhu Island
     */
    virtual RPSysScene::ETime getIslandTime() {
        return RPSysScene::ETime_Auto;
    }
#endif
    /**@}*/

    /**
     * @name User state
     */
    /**@{*/
    /**
     * @brief Configure state user callback
     * @details Ran once on initial scene setup
     */
    virtual void OnConfigure() {}
    /**
     * @brief LoadResource state user callback
     * @details Ran once on asset loading
     */
    virtual void OnLoadResource() {}
    /**
     * @brief Reset state user callback
     * @details Ran once on initial scene setup and on every restart
     */
    virtual void OnReset() {}
    /**
     * @brief Calculate state user callback
     * @details Ran once per frame
     */
    virtual void OnCalculate() {}
#ifdef PACK_RESORT
    /**
     * @brief CalculateMPlus state user callback
     * @details Ran once per frame
     */
    virtual void OnCalculateMPlus() {}
#endif
    /**
     * @brief Exit state user callback
     * @details Ran once on scene exit (including restarts)
     */
    virtual void OnExit() {}

    /**
     * @brief Pause user callback
     * @details Ran on pause menu open/close
     *
     * @param enter Whether the pause menu is opening
     */
    virtual void pauseCallBack(bool enter) {}

    /**
     * @brief User-level draw
     * @details Ran once per renderer draw pass, per frame
     * @see EDrawPass
     */
    virtual void OnUserDraw() {}
    /**
     * @brief Debug-level draw
     * @details Ran once per renderer draw pass, per frame
     * @see EDrawPass
     */
    virtual void OnDebugDraw() {}
    /**@}*/

    /**
     * @brief Start task(s) on another thread
     * @details Ran once before entering scene
     */
    virtual void taskAsync() {}

private:
    /**
     * @name Internal state
     */
    /**@{*/
    /**
     * @brief Configure state
     * @details Performs initial scene setup
     */
    virtual void Configure();
    /**
     * @brief LoadResource state
     * @details Loads scene assets from the DVD
     */
    virtual void LoadResource();
    /**
     * @brief Reset state
     * @details Resets scene state
     */
    virtual void Reset();
    /**
     * @brief Calculate state
     * @details Runs one step of scene logic
     */
    virtual void Calculate();
#ifdef PACK_RESORT
    /**
     * @brief CalculateMPlus state
     * @details Runs one step of MotionPlus logic
     */
    virtual void CalculateMPlus();
#endif
    /**
     * @brief Exit state
     * @details Finalizes scene before exiting
     */
    virtual void Exit();

    /**
     * @brief User-level draw
     * @details Ran once per renderer draw pass, per frame
     * @see EDrawPass
     */
    virtual void UserDraw();
    /**
     * @brief Debug-level draw
     * @details Ran once per renderer draw pass, per frame
     * @see EDrawPass
     */
    virtual void DebugDraw();
    /**@}*/

    /**
     * @brief Factory method (for scene creator)
     */
    template <typename TDerived> static RPSysScene* Create() {
        return new TDerived();
    }
}; // namespace kiwi

/**
 * @brief User scene declaration
 * @details Makes user scene visible to the scene creator
 */
template <typename T> class SceneDecl {
public:
    /**
     * @brief Constructor
     */
    SceneDecl() {
        // Devirtualize calls
        const SceneCreator::Info info = {
            &IScene::Create<T>,
            static_cast<T*>(nullptr)->T::GetName(),
            static_cast<T*>(nullptr)->T::GetDirectory(),
            static_cast<T*>(nullptr)->T::GetID(),
            static_cast<T*>(nullptr)->T::GetPack(),
            static_cast<T*>(nullptr)->T::GetCreateType(),
            static_cast<T*>(nullptr)->T::GetExitType(),
            static_cast<T*>(nullptr)->T::GetCommonSound(),
        };

        SceneCreator::GetInstance().RegistScene(info);
    }
};

//! @}
} // namespace kiwi

#endif
