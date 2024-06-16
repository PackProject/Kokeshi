// Includes ALL of libkiwi
#include <libkiwi.h>

//=============================================================================
// Scene IDs
//=============================================================================
//
// Your scene IDs must begin after the Pack Project scene IDs.
// The easiest way to do this is to begin at kiwi::ESceneID_Max.
//
enum EMySceneID {
    EMySceneID_UserScene0 = kiwi::ESceneID_Max,
    EMySceneID_UserScene1,
    EMySceneID_UserScene2,
    // And so on ...
};

//=============================================================================
// Scene class
//=============================================================================
//
// Your class must inherit from the scene interface, IScene.
//
class CustomScene : public kiwi::IScene {
    CustomScene() {}
    virtual ~CustomScene() {}

    //
    // Scene configuration
    //
    // This example demonstrates a few useful functions.
    // Check out the "Configuration" section of the IScene class to see more.
    //

    // At a minimum, you must override GetID so your scene can be constructed.
    virtual s32 GetID() const {
        return EMySceneID_UserScene0;
    }

    // However, for debugging you should also consider GetName.
    virtual kiwi::String GetName() const {
        return "My Scene";
    }

    // If you want to use your own directory, override GetDirectory.
    // Otherwise, it will default to "RPCommon/".
    virtual kiwi::String GetDirectory() const {
        return "CustomScene/";
    }

    //
    // State functions
    //
    // This example demonstrates a few useful functions.
    // Check out the "User state" section of the IScene class to see more.
    //

    // Ran once on initial scene setup
    virtual void OnConfigure() {
        K_LOG("Configuring CustomScene!\n");
    }

    // Ran once on initial scene setup and on every restart
    virtual void OnReset() {
        K_LOG("Resetting CustomScene!\n");
    }

    // Ran once on scene exit (including restarts)
    virtual void OnExit() {
        K_LOG("Exiting CustomScene!\n");
    }
};

//=============================================================================
// Scene declaration
//=============================================================================
//
// To make the scene creator aware of your custom scene, you need to construct a
// scene declaration, or SceneDecl object.
//
// To simplify construction, libkiwi provides the K_SCENE_DECL macro.
// Its argument must be the scene's class name.
//
// This macro creates a static object, so it must be placed within the .cpp
// file, not the header file.
//
K_SCENE_DECL(CustomScene);

//=============================================================================
// Scene change
//=============================================================================
//
// To enter your custom scene, simply use the ChangeSceneAfterFade method of the
// scene creator elsewhere in your code. Below is an example of a callsite:
//
void dummy() {
    kiwi::SceneCreator::GetInstance().ChangeSceneAfterFade(
        EMySceneID_UserScene0);
}

//=============================================================================
// More Information
//=============================================================================
//
// For more information, please see libkiwi/core/kiwiIScene.h.
//
// The important functionality is covered in this example, but the scene
// interface allows for much more!
//
