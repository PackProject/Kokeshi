#ifndef LIBKIWI_CORE_I_SCENE_H
#define LIBKIWI_CORE_I_SCENE_H
#include <Pack/RPSystem.h>
#include <libkiwi/core/kiwiSceneCreator.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>

namespace kiwi {

/**
 * @brief User scene interface
 */
class IScene : public RPSysScene {
    friend class SceneCreator;
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
     * @brief Information for custom scenes
     */
    /*@{*/
    /**
     * @brief Get the scene's name
     */
    virtual String GetName() const {
        return "Dummy";
    }

    /**
     * @brief Get the scene's resource directory name
     */
    virtual String GetDirectory() const {
        return "RPCommon/";
    }

    /**
     * @brief Get the scene's ID
     */
    virtual s32 GetID() const = 0;

    /**
     * @brief Get the scene's supported Pack Project
     */
    virtual EPackID GetPack() const {
        return EPackID_AllPack;
    }

    /**
     * @brief Get the scene's create type
     */
    virtual ECreateType GetCreateType() const {
        return ECreateType_1;
    }

    /**
     * @brief Get the scene's exit type
     */
    virtual EExitType GetExitType() const {
        return EExitType_0;
    }

    /**
     * @brief Whether the scene requires the RP common sound archive
     */
    virtual bool GetCommonSound() const {
        return true;
    }
    /*@}*/

#ifdef PACK_RESORT
    /**
     * @brief Get the type of this scene
     */
    virtual RPSysScene::EKind getKind() {
        return RPSysScene::EKind_System;
    }

    /**
     * @brief Get the island time of day
     */
    virtual RPSysScene::ETime getIslandTime() {
        return RPSysScene::ETime_Auto;
    }
#endif

    /**
     * @brief Pause callback
     *
     * @param enter Whether entering menu
     */
    virtual void pauseCallBack(bool enter) {}

    /**
     * @brief Base state functions
     */
    /*@{*/
    /**
     * @brief Setup scene
     */
    virtual void Configure();
    /**
     * @brief Load required assets
     */
    virtual void LoadResource();
    /**
     * @brief Reload scene
     */
    virtual void Reset();
    /**
     * @brief Scene logic
     */
    virtual void Calculate();
#ifdef PACK_RESORT
    /**
     * @brief Scene MotionPlus logic
     */
    virtual void CalculateMPlus();
#endif
    /**
     * @brief Exit scene
     */
    virtual void Exit();

    /**
     * @brief User-level draw
     */
    virtual void UserDraw();
    /**
     * @brief Debug-level draw
     */
    virtual void DebugDraw();
    /*@}*/

    /**
     * @brief User state functions
     */
    /*@{*/
    /**
     * @brief Setup scene
     */
    virtual void OnConfigure() {}
    /**
     * @brief Load required assets
     */
    virtual void OnLoadResource() {}
    /**
     * @brief Reload scene
     */
    virtual void OnReset() {}
    /**
     * @brief Scene logic
     */
    virtual void OnCalculate() {}
#ifdef PACK_RESORT
    /**
     * @brief Scene MotionPlus logic
     */
    virtual void OnCalculateMPlus() {}
#endif
    /**
     * @brief Exit scene
     */
    virtual void OnExit() {}

    /**
     * @brief User-level draw
     */
    virtual void OnUserDraw() {}
    /**
     * @brief Debug-level draw
     */
    virtual void OnDebugDraw() {}
    /*@}*/

    /**
     * @brief Any async task(s) to start before entering this scene
     */
    virtual void taskAsync() {}

private:
    /**
     * @brief Factory method (for scene creator)
     */
    template <typename TDerived> static RPSysScene* Create() {
        return new TDerived();
    }

}; // namespace kiwi

/**
 * @brief Declares scene to the creator
 * @note Construct one at the file scope
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
            static_cast<T*>(NULL)->T::GetName(),
            static_cast<T*>(NULL)->T::GetDirectory(),
            static_cast<T*>(NULL)->T::GetID(),
            static_cast<T*>(NULL)->T::GetPack(),
            static_cast<T*>(NULL)->T::GetCreateType(),
            static_cast<T*>(NULL)->T::GetExitType(),
            static_cast<T*>(NULL)->T::GetCommonSound(),
        };

        SceneCreator::GetInstance().RegistScene(info);
    }
};

/**
 * @brief Helper macro for creating SceneDecl
 */
#define K_SCENE_DECL(T) static kiwi::SceneDecl<T> SceneDecl_##T;

} // namespace kiwi

#endif
