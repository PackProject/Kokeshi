#ifndef RP_SYSTEM_SCENE_CREATOR_H
#define RP_SYSTEM_SCENE_CREATOR_H
#include "RPTypes.h"

#include <egg/core/eggSceneCreator.h>

/**
 * @brief Factory for all RP scenes
 * @details Not to be confused with the scene manager, the scene creator simply
 * creates/destroys scenes. The scene creator ensures that all scenes created
 * are intended for the current pack by referencing the scene attributes.
 * @wfuname
 */
class RPSysSceneCreator : public EGG::SceneCreator {
public:
    // @brief RP Scene ID
    // @wfuname
    enum ESceneID {
        // System scenes
        ESceneID_RPSysBootScene,
        ESceneID_RPSysPlayerSelectScene,
        ESceneID_RPSysNunchukScene,

        // Sports Pack scenes
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
        RP_SCENE_MAX,
#endif

        // Party Pack scenes
        RP_SCENE_09h,
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
        RP_SCENE_MAX,
#endif

        // Health Pack scenes
        RP_HEALTH_TITLE_SCENE,
        RP_HEALTH_COUNSEL_SCENE,
        RP_HEALTH_YOGA_SCENE,
        RP_HEALTH_TRAINING_SCENE,
        RP_HEALTH_RADIO_GYM_SCENE,
        RP_HEALTH_GRAPH_SCENE,
        RP_HEALTH_BALANCE_SCENE,
        RP_HEALTH_SKI_SCENE,
        RP_SCENE_23H,
        RP_SCENE_24H,

        // Music Pack scenes
        RP_DRM_SCENE,
        RP_OCH_SCENE,
        RP_MPE_1_SCENE,
        RP_MPE_2_SCENE,
        RP_MPE_3_SCENE,
        RP_MPE_4_SCENE,
        RP_MPE_5_SCENE,
        RP_MUSIC_TITLE_SCENE,

        // All pack scenes
        RP_ALL_PACK_SCENE,
        RP_BASIC_DEBUG_SCENE,
        RP_GRAPHICS_DEBUG_SCENE,
        RP_SYSTEM_DEBUG_SCENE,
        RP_LAYOUT_DEBUG_SCENE,
        RP_CURSOR_DEBUG_SCENE,
        RP_VIBRATION_DEBUG_SCENE,
        RP_RESULT_DEBUG_SCENE,
        RP_UI_DEBUG_SCENE,
        RP_SMK_TRAINING_SCENE,
        RP_MNT_TRAINING_SCENE,
        RP_BSB_DEBUG_SCENE,
        RP_DRM_DEBUG_SCENE,
        RP_OCH_DEBUG_SCENE,
        RP_OCH_MUSIC_SELECT_SCENE,
    };

    /**
     * @brief Sport ID
     */
    enum ESportID {
        SPORT_BASEBALL,
        SPORT_TENNIS,
        SPORT_GOLF,
        SPORT_BOXING,
        SPORT_BOWLING,

        SPORT_MAX
    };

    // @brief Pack Project game ID
    enum EPackID {
        RP_SPORTS_PACK,
        RP_PARTY_PACK,
        RP_HEALTH_PACK,
        RP_MUSIC_PACK,
        RP_ALLPACK
    };

    /**
     * @brief Attributes to show the creator how to handle a given scene
     * @customname
     */
    struct SceneAttr {
        // @brief Scene ID
        s32 scene; // at 0x0
        // @brief Pack ID
        s32 pack; // at 0x4
        // @brief How to create the scene
        s32 createType; // at 0x8
        // @brief Toggle "Now Loading" text (unused)
        s32 showLoadingText; // at 0xC
        // @brief How to exit the scene
        s32 exitType; // at 0x10
        // @brief Whether to use the RP common sound archive
        s32 useCommonSound; // at 0x14
        // @brief Scene file directory
        const char* resDirName; // at 0x18
        // @brief Proper name (unused lookup feature?)
        const char* sceneName; // at 0x1C
    };

public:
    // @address 80184e10
    static RPSysSceneCreator* CreateInstance(EGG::Heap* heap);
    static RPSysSceneCreator* GetInstance() {
        return sInstance;
    }

    s32 getLastSceneID() const {
        return mLastSceneID;
    }
    void setLastSceneID(s32 scene) {
        mLastSceneID = scene;
    }

    /**
     * @brief Fade out into a new scene, optionally reloading the current scene
     * @address 80184ba4
     * @return Success
     */
    bool changeSceneAfterFade(s32 sceneID, bool reenterCurrent = false);
    /**
     * @brief Change to the boot scene (performing a soft reset)
     * @address 80184b98
     */
    void changeSoftReset();

    /**
     * @brief Create a Sports Pack scene
     * @address 80184000
     */
#ifdef PACK_SPORTS
    EGG::Scene* createSportsScene(s32 sceneID);
#elif PACK_PLAY
    EGG::Scene* createPartyScene(s32 sceneID);
#elif PACK_RESORT
    EGG::Scene* createWS2Scene(s32 sceneID);
#endif

    // @address 801845f4
    s32 getCreateType(s32 sceneID);
    // @address 80184558
    s32 getExitType(s32 sceneID);
    // @address 801844bc
    s32 getUseCommonSound(s32 sceneID);
    // @address 80184758
    s32 getResDirName(s32 sceneID);
    // @address 80184690
    s32 getSceneID(s32 sportID);
    // @address 801846ec
    s32 getSportID(s32 sceneID);

    /**
     * @brief Create any Pack Project scene
     * @address 80184838
     */
    virtual EGG::Scene* create(s32 sceneID);

    /**
     * @brief Request engine shutdown
     * @address 80184804
     */
    virtual void destroy(s32);

private:
    RPSysSceneCreator(EGG::Heap* heap) : mParentHeap(heap), mLastSceneID(-1) {}
    // @address 80183f68
    virtual ~RPSysSceneCreator() {}

private:
    // @brief Heap in which this object was allocated
    EGG::Heap* mParentHeap; // at 0x4
    // @brief Last created scene's ID
    s32 mLastSceneID; // at 0x8

    /**
     * @brief Configuration for all supported Pack Project scenes
     * @address 803820f8
     */
    static SceneAttr sSceneAttributeTable[];

    /**
     * @brief Static instance
     * @address 804bf4f8
     */
    static RPSysSceneCreator* sInstance;
};

#endif
