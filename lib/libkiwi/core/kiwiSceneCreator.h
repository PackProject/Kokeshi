#ifndef LIBKIWI_CORE_SCENE_CREATOR_H
#define LIBKIWI_CORE_SCENE_CREATOR_H

#include <RPSystem/RPSysSceneCreator.h>
#include <libkiwi/core/kiwiController.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiExtension.h>

#include <nw4r/ut.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief Game scene ID
 */
enum ESceneID {
#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    // RPSystem
    ESceneID_RPSysBootScene,
    ESceneID_RPSysPlayerSelectScene,
    ESceneID_RPSysNunchukScene,

    // RPSports
    ESceneID_RPBsbScene,
    ESceneID_RPBowScene,
    ESceneID_RPGolScene,
    ESceneID_RPTnsScene,
    ESceneID_RPBoxScene,
    ESceneID_RPSportsTitleScene,
    ESceneID_RPSportsMenuScene,
    ESceneID_RPSportsTrainingMenuScene,
    ESceneID_RPSportsPhysicalMenuScene,
    ESceneID_RPSportsPhysicalPreviewScene,
    ESceneID_RPSportsPhysicalResultScene,
    ESceneID_RPGolSelectScene,

#if defined(PACK_SPORTS)
    ESceneID_Max,
#endif

    // RPParty
    ESceneID_Unknown09,
    ESceneID_RPFshScene,
    ESceneID_RPHkyScene,
    ESceneID_RPDucScene,
    ESceneID_RPPnpScene,
    ESceneID_RPBilScene,
    ESceneID_RPCowScene,
    ESceneID_RPWlyScene,
    ESceneID_RPTnkScene,
    ESceneID_RPBomScene,
    ESceneID_RPPartyTitleScene,
    ESceneID_RPPartyMiiLoadScene,
    ESceneID_RPPartyMenuScene,

#if defined(PACK_PLAY)
    ESceneID_Max,
#endif
#endif

#if defined(PACK_RESORT)
    // System
    ESceneID_Sp2StrapScene,
    ESceneID_Sp2SaveDataLoadScene,
    ESceneID_Sp2TitleScene,
    ESceneID_Sp2MiiSelectScene,

    // Sports
    ESceneID_Sp2SwfScene,
    ESceneID_Sp2JskScene,
    ESceneID_Sp2ArcScene,
    ESceneID_Sp2FldScene,
    ESceneID_Sp2BskScene,
    ESceneID_Sp2BwlScene,
    ESceneID_Sp2CanScene,
    ESceneID_Sp2PngScene,
    ESceneID_Sp2WkbScene,
    ESceneID_Sp2PlnScene,
    ESceneID_Sp2GlfScene,
    ESceneID_Sp2DglScene,
    ESceneID_Sp2BicScene,
    ESceneID_Sp2OmkScene,

    // Debug
    ESceneID_Sp2DebugRootScene,
    ESceneID_Sp2SaveDataDebugScene,
    ESceneID_Sp2ContestMiiDebugScene,
    ESceneID_Sp2MessageCheckScene,
    ESceneID_Sp2LayoutCheckScene,
    ESceneID_Sp2LayoutCheck2Scene,
    ESceneID_Sp2NkjTestScene,
    ESceneID_Sp2LayoutTestScene,
    ESceneID_Sp2SoundTestScene,
    ESceneID_Sp2CpuMiiScene,

    ESceneID_Max,
#endif
};

#if defined(PACK_RESORT)
/**
 * @brief Scene group ID
 */
enum EGroupID {
    EGroupID_Cmn = -1, //!< Common
    EGroupID_Swf,      //!< Swordplay
    EGroupID_Jsk,      //!< Power Cruising
    EGroupID_Arc,      //!< Archery
    EGroupID_Fld,      //!< Frisbee
    EGroupID_Bsk,      //!< Basketball
    EGroupID_Bwl,      //!< Bowling
    EGroupID_Can,      //!< Canoeing
    EGroupID_Png,      //!< Table Tennis
    EGroupID_Wkb,      //!< Wakeboarding
    EGroupID_Pln,      //!< Air Sports
    EGroupID_Glf,      //!< Golf
    EGroupID_Dgl,      //!< Frisbee Golf
    EGroupID_Bic,      //!< Cycling
    EGroupID_Omk,      //!< Skydiving
    EGroupID_Debug,    //!< Debug

    EGroupID_Max,
};
#endif

/**
 * @brief Pack Project ID
 */
enum EPackID {
    EPackID_SportsPack, //!< Wii Sports
    EPackID_PartyPack,  //!< Wii Play
    EPackID_HealthPack, //!< Wii Fit
    EPackID_MusicPack,  //!< Wii Music
    EPackID_AllPack,    //!< Pack Project

    EPackID_Max
};

/**
 * @brief Scene create type
 */
enum ECreateType {
    ECreateType_Standard, //!< Overwrite outgoing scene
    ECreateType_Sibling,  //!< Become sibling of outgoing scene
    ECreateType_Child,    //!< Become child of outgoing scene
};

/**
 * @brief Scene exit type
 */
enum EExitType {
    EExitType_Standard, //!< Incoming scene overwrites this scene
    EExitType_Sibling,  //!< Incoming scene becomes a sibling of this scene
    EExitType_2,
    EExitType_3,
    EExitType_Child, //!< Incoming scene becomes a child of this scene
};

/**
 * @brief Scene factory
 */
class SceneCreator : public ExtSingletonPtr<RPSysSceneCreator, SceneCreator> {
public:
    /**
     * @brief Scene information
     */
    struct Info {
        RPSysScene* (*pCt)(); //!< Scene create function (for user scenes)
        String name;          //!< Scene name
        String dir;           //!< Resource directory
        s32 id;               //!< Scene ID (for RP scenes)
#if defined(PACK_RESORT)
        bool warnAsSceneLoading; //!< Show warning screen while scene loads
        kiwi::EGroupID group;    //!< Scene group
#endif
        kiwi::EPackID pack;       //!< Pack ID
        kiwi::ECreateType create; //!< How to create the scene
        kiwi::EExitType exit;     //!< How to exit the scene
        bool common;              //!< Use the RP common sound archive
    };

public:
    /**
     * @brief Registers user scene class
     *
     * @param rInfo Scene info
     */
    static void RegistScene(const Info& rInfo);

    /**
     * @brief Registers root debug menu information
     * @details The debug root scene is entered when transitioning to the main
     * menu while the specified button mask is held.
     *
     * @param id Debug root scene ID (-1 to disable)
     * @param buttons Buttons that must be held to visit the debug menu
     * (defaults to B+MINUS+2)
     */
    static void RegistDebugRoot(s32 id,
                                u16 buttons = (EButton_B | EButton_Minus |
                                               EButton_2));

    /**
     * @brief Gets the scene ID of the main menu scene
     */
    static s32 GetMenuScene();

    /**
     * @brief Gets the scene ID of the bootup scene
     */
    static s32 GetBootScene();

#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    /**
     * @brief Fades out into a new scene
     *
     * @param id Scene ID
     * @param reload Reload the current scene
     * @return Success
     */
    bool ChangeSceneAfterFade(s32 id, bool reload = false);
#elif defined(PACK_RESORT)
    /**
     * @brief Fades out into a new scene
     *
     * @param id Scene ID
     * @param pColor Fade color (optional)
     * @return Success
     */
    bool ChangeSceneAfterFade(s32 id, const nw4r::ut::Color* pColor = nullptr);
#endif

    /**
     * @brief Changes to the bootup scene
     *
     * @return Success
     */
    bool ChangeBootScene() {
        return ChangeSceneAfterFade(GetBootScene());
    }

    /**
     * @brief Gets the specified scene's name
     *
     * @param id Scene ID
     */
    const char* GetSceneName(s32 id = -1) const;

    /**
     * @brief Gets the specified scene's resource directory name
     *
     * @param id Scene ID
     */
    const char* GetSceneDirectory(s32 id = -1) const;

#if defined(PACK_RESORT)
    /**
     * @brief Tests whether the specified scene should display the warning
     * screen while loading
     *
     * @param id Scene ID
     */
    bool IsSceneWarnAsLoading(s32 id = -1) const;

    /**
     * @brief Gets the specified scene's group
     *
     * @param id Scene ID
     */
    EGroupID GetSceneGroup(s32 id = -1) const;
#endif

    /**
     * @brief Gets the specified scene's target pack
     *
     * @param id Scene ID
     */
    kiwi::EPackID GetScenePack(s32 id = -1) const;

    /**
     * @brief Gets the specified scene's create type
     *
     * @param id Scene ID
     */
    kiwi::ECreateType GetSceneCreateType(s32 id = -1) const;

    /**
     * @brief Gets the specified scene's exit type
     *
     * @param id Scene ID
     */
    kiwi::EExitType GetSceneExitType(s32 id = -1) const;

    /**
     * @brief Tests whether the specified scene requires the RP common sound
     * archive
     *
     * @param id Scene ID
     */
    bool IsSceneCommonSound(s32 id = -1) const;

private:
    LIBKIWI_KAMEK_PUBLIC

    /**
     * @brief Creates a new scene by ID
     *
     * @param id Scene ID
     */
    RPSysScene* Create(s32 id);

    /**
     * @brief Creates a new system scene by ID
     *
     * @param id Scene ID
     */
    RPSysScene* CreateSystemScene(s32 id);

    /**
     * @brief Creates a new game scene by ID
     *
     * @param id Scene ID
     */
    RPSysScene* CreatePackScene(s32 id);

    /**
     * @brief Creates a new user scene by ID
     *
     * @param id Scene ID
     */
    RPSysScene* CreateUserScene(s32 id);

    /**
     * @brief Gets information on the specified scene
     *
     * @param id Scene ID
     */
    static const Info* GetSceneInfo(s32 id);

private:
    //! Pack Project scenes
    static const Info scPackScenes[];
    //! User-registered scenes
    static TMap<s32, Info> sUserScenes;

    //! Root debug menu scene ID
    static s32 sDebugRootID;
    //! Root debug menu button combination (held)
    static u16 sDebugRootButtons;
};

//! @}
} // namespace kiwi

#endif
