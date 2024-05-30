#ifndef LIBKIWI_CORE_SCENE_CREATOR_H
#define LIBKIWI_CORE_SCENE_CREATOR_H
#include <RPSystem/RPSysSceneCreator.h>
#include <libkiwi/k_config.h>
#include <libkiwi/k_types.h>

namespace kiwi {

/**
 * @brief List of game scene IDs
 */
enum ESceneID {
#ifndef PACK_RESORT
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

#ifdef PACK_SPORTS
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

#ifdef PACK_PLAY
    ESceneID_Max,
#endif

#else
    // System
    ESceneID_Sp2StrapScene,
    ESceneID_Sp2SaveDataLoadScene,
    ESceneID_Sp2TitleScene,
    ESceneID_Sp2PlayerSelectScene,

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

/**
 * @brief Pack Project ID
 */
enum EPackID {
    EPackID_SportsPack,
    EPackID_PartyPack,
    EPackID_HealthPack,
    EPackID_MusicPack,
    EPackID_AllPack,
};

/**
 * @brief Scene create type
 */
enum ECreateType {
    ECreateType_0,
    ECreateType_1,
    ECreateType_2,
};

/**
 * @brief Scene exit type
 */
enum EExitType {
    EExitType_0,
    EExitType_1,
    EExitType_2,
    EExitType_3,
    EExitType_4,
    EExitType_5,
    EExitType_6,
};

/**
 * @brief Scene factory
 */
class SceneCreator : private RPSysSceneCreator, private NonCopyable {
public:
    /**
     * @brief Scene information
     */
    struct Info {
        RPSysScene* (*ct)();      // Scene create function (for user scenes)
        String name;              // Scene name
        String dir;               // Resource directory
        s32 id;                   // Scene ID (for RP scenes)
        kiwi::EPackID pack;       // Pack ID
        kiwi::ECreateType create; // How to create the scene
        kiwi::EExitType exit;     // How to exit the scene
        bool common;              // Use the RP common sound archive
    };

public:
    /**
     * @brief Access singleton instance
     */
    static SceneCreator& GetInstance();

    /**
     * @brief Register user scene class
     */
    static void RegistScene(const Info& info);

    /**
     * @brief Fade out into a new scene
     *
     * @param id Scene ID
     * @param arg1 Unknown
     */
    bool ChangeSceneAfterFade(s32 id, bool arg1 = false);

    /**
     * @brief Get the specified scene's name
     */
    const char* GetSceneName(s32 id) const;

    /**
     * @brief Get the specified scene's name
     */
    const char* GetSceneDirectory(s32 id) const;

    /**
     * @brief Get the specified scene's name
     */
    kiwi::EPackID GetScenePack(s32 id) const;

    /**
     * @brief Get the specified scene's name
     */
    kiwi::ECreateType GetSceneCreateType(s32 id) const;

    /**
     * @brief Get the specified scene's name
     */
    kiwi::EExitType GetSceneExitType(s32 id) const;

    /**
     * @brief Get the specified scene's name
     */
    bool GetSceneCommonSound(s32 id) const;

private:
    LIBKIWI_KAMEK_PUBLIC

    RPSysScene* Create(s32 id);
    RPSysScene* CreateSystemScene(s32 id);
    RPSysScene* CreatePackScene(s32 id);
    RPSysScene* CreateUserScene(s32 id);

private:
    static const Info* GetSceneInfo(s32 id);

private:
    static TMap<s32, Info> sUserScenes;
    static const Info scPackScenes[];
};

} // namespace kiwi

#endif
