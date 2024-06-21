#ifndef RP_SYSTEM_SCENE_CREATOR_H
#define RP_SYSTEM_SCENE_CREATOR_H
#include <Pack/RPTypes.h>
#include <egg/core.h>

//! @addtogroup rp_system
//! @{

/**
 * @brief Pack Project scene factory
 * @wfuname
 */
class RPSysSceneCreator : public EGG::SceneCreator {
    RP_SINGLETON_DECL_EX(RPSysSceneCreator);

public:
    /**
     * @brief Game scene ID
     * @wfuname
     */
    enum ESceneID {
#if !defined(PACK_RESORT) || defined(KOKESHI_PARSED_BY_DOXYGEN)
        // RPSystem
        ESceneID_RPSysBootScene,         //!< Logo
        ESceneID_RPSysPlayerSelectScene, //!< Player Select
        ESceneID_RPSysNunchukScene,      //!< Nunchuk Check

        // RPSports
        ESceneID_RPBsbScene,                   //!< Baseball
        ESceneID_RPBowScene,                   //!< Bowling
        ESceneID_RPGolScene,                   //!< Golf
        ESceneID_RPTnsScene,                   //!< Tennis
        ESceneID_RPBoxScene,                   //!< Boxing
        ESceneID_RPSportsTitleScene,           //!< Sports Pack
        ESceneID_RPSportsMenuScene,            //!< Main Menu
        ESceneID_RPSportsTrainingMenuScene,    //!< Training Menu
        ESceneID_RPSportsPhysicalMenuScene,    //!< Physical Test (Menu)
        ESceneID_RPSportsPhysicalPreviewScene, //!< Physical Test (Description)
        ESceneID_RPSportsPhysicalResultScene,  //!< Physical Test (Result)
        ESceneID_RPGolSelectScene,             //!< Golf (Course Select)
#endif

#if defined(PACK_SPORTS)
        ESceneID_Max,
#endif

        // RPParty
        ESceneID_Unknown09h,
        ESceneID_RPFshScene,          //!< Fishing
        ESceneID_RPHkyScene,          //!< Laser Hockey
        ESceneID_RPDucScene,          //!< Shooting Range
        ESceneID_RPPnpScene,          //!< Table Tennis
        ESceneID_RPBilScene,          //!< Billiards
        ESceneID_RPCowScene,          //!< Charge!
        ESceneID_RPWlyScene,          //!< Find Mii
        ESceneID_RPTnkScene,          //!< Tanks!
        ESceneID_RPBomScene,          //!< Pose Mii
        ESceneID_RPPartyTitleScene,   //!< Party Pack
        ESceneID_RPPartyMiiLoadScene, //!< Mii/GameMgr Setup
        ESceneID_RPPartyMenuScene,    //!< Main Menu

#if defined(PACK_PLAY)
        ESceneID_Max,
#endif

        // RPHealth
        ESceneID_RPHealthTitleScene,    //!< Health Pack
        ESceneID_RPHealthCounselScene,  //!< Counseling
        ESceneID_RPHealthYogaScene,     //!< Yoga
        ESceneID_RPHealthTrainingScene, //!< Training (Menu)
        ESceneID_RPHealthRadioGymScene, //!< Radio Calisthenics
        ESceneID_RPHealthGraphScene,    //!< Health Graph
        ESceneID_RPHealthBalanceScene,  //!< Balance Test
        ESceneID_RPHealthSkiScene,      //!< Balance Ski
        ESceneID_Unknown23h,
        ESceneID_Unknown24h,

        // RPMusic
        ESceneID_RPDrmScene,        //!< Drum
        ESceneID_RPOchScene,        //!< Orchestra
        ESceneID_RPMpeScene1,       //!< Music Experiment 1
        ESceneID_RPMpeScene2,       //!< Music Experiment 2
        ESceneID_RPMpeScene3,       //!< Music Experiment 3
        ESceneID_RPMpeScene4,       //!< Music Experiment 4
        ESceneID_RPMpeScene5,       //!< Music Experiment 5
        ESceneID_RPMusicTitleScene, //!< Music Pack

        // Pack Project
        ESceneID_RPAllPackScene,        //!< Pack Project
        ESceneID_RPBasicDebugScene,     //!< Basic
        ESceneID_RPGraphicsDebugScene,  //!< Graphics
        ESceneID_RPSystemDebugScene,    //!< System
        ESceneID_RPLayoutDebugScene,    //!< Layout
        ESceneID_RPCursorDebugScene,    //!< Cursor
        ESceneID_RPVibrationDebugScene, //!< Vibration
        ESceneID_RPResultDebugScene,    //!< Result
        ESceneID_RPUIDebugScene,        //!< UI Test
        ESceneID_RPSmkTrainingScene,    //!< New Recruit Training (Smk)
        ESceneID_RPMntTrainingScene,    //!< New Recruit Training (Mnt)
        ESceneID_RPBsbDebugScene,       //!< Baseball Debug
        ESceneID_RPDrmDebugScene,       //!< Drum Debug
        ESceneID_RPOchDebugScene,       //!< Orchestra Debug
        ESceneID_RPOchSelectScene,      //!< Orchestra Music Select
    };

    /**
     * @brief Sport ID
     * @customname
     */
    enum ESportID {
        ESportID_Bsb, //!< Baseball
        ESportID_Tns, //!< Tennis
        ESportID_Gol, //!< Golf
        ESportID_Box, //!< Boxing
        ESportID_Bow, //!< Bowling

        ESportID_Max
    };

    /**
     * @brief Pack Project game ID
     * @customname
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
     * @customname
     */
    enum ECreateType {
        ECreateType_Standard, //!< Overwrite outgoing scene
        ECreateType_Sibling,  //!< Become sibling of outgoing scene
        ECreateType_Child,    //!< Become child of outgoing scene
    };

    /**
     * @brief Scene exit type
     * @customname
     */
    enum EExitType {
        EExitType_Standard, //!< Incoming scene overwrites this scene
        EExitType_Sibling,  //!< Incoming scene is a sibling of this scene
        EExitType_2,
        EExitType_3,
        EExitType_Child, //!< Incoming scene is a child of this scene
    };

    /**
     * @brief Scene information
     * @customname
     */
    struct SceneEntry {
        //! @brief Scene ID
        ESceneID scene; // at 0x0
        //! @brief Pack ID
        EPackID pack; // at 0x4
        //! @brief How to create the scene
        ECreateType createType; // at 0x8
        //! @brief Toggle "Now Loading" text (unused)
        BOOL loadingText; // at 0xC
        //! @brief How to exit the scene
        EExitType exitType; // at 0x10
        //! @brief Whether to use the RP common sound archive
        BOOL commonSound; // at 0x14
        //! @brief Scene resource directory
        const char* pResDirName; // at 0x18
        //! @brief Proper name
        const char* pSceneName; // at 0x1C
    };

public:
#ifdef PACK_SPORTS
    /**
     * @brief Creates a Sports Pack scene by ID
     *
     * @param id Scene ID
     */
    EGG::Scene* createSportsScene(s32 id);
#elif PACK_PLAY
    /**
     * @brief Creates a Party Pack scene by ID
     *
     * @param id Scene ID
     */
    EGG::Scene* createPartyScene(s32 id);
#elif PACK_RESORT
    /**
     * @brief Creates a Wii Sports 2 scene by ID
     *
     * @param id Scene ID
     */
    EGG::Scene* createSp2Scene(s32 id);
#endif

    /**
     * @brief Fades out the current scene and changes to a new scene
     * @details Supply an ID of -1 to use the ID of the current scene
     *
     * @param id New scene ID (-1 for current scene)
     * @param reload Whether to reload the current scene
     * @return Success
     */
    bool changeSceneAfterFade(s32 id, bool reload = false);
    /**
     * @brief Changes the scene to the boot scene (performs a soft reset)
     */
    void changeSoftReset();

    /**
     * @brief Creates a scene by ID
     *
     * @param id Scene ID
     */
    virtual EGG::Scene* create(s32 id);
    /**
     * @brief Destroys a scene
     * @details If the scene manager has requested a shutdown, this function
     * will tell the system to shutdown.
     *
     * @param id ID of the scene being destroyed
     */
    virtual void destroy(s32 id);

    /**
     * @brief Gets the scene's resource directory
     * @bug If the specified scene has no entry, this function's result is
     * undefined behavior. It reads from memory before the array of entries.
     *
     * @param id Scene ID (-1 to use the current scene)
     */
    const char* getResDirName(s32 id = -1);

#ifdef PACK_SPORTS
    /**
     * @brief Gets the sport corresponding to the scene ID
     * @details If the specified scene has no entry, this function returns -1.
     *
     * @param id Scene ID (-1 to use the current scene)
     */
    s32 getSportID(s32 id = -1);
#endif

    /**
     * @brief Gets the scene ID corresponding to the sport
     * @details If the specified sport has no entry, this function returns -1.
     *
     * @param sport Sport ID
     */
    s32 getSceneID(ESportID sport);

    /**
     * @brief Gets the scene's create logic type
     * @bug If the specified scene has no entry, this function's result is
     * undefined behavior. It reads from memory before the array of entries.
     *
     * @param id Scene ID (-1 to use the current scene)
     */
    s32 getCreateType(s32 id = -1);

    /**
     * @brief Gets the scene's exit logic type
     * @bug If the specified scene has no entry, this function's result is
     * undefined behavior. It reads from memory before the array of entries.
     *
     * @param id Scene ID (-1 to use the current scene)
     */
    s32 getExitType(s32 id = -1);

    /**
     * @brief Tests whether the scene should use the pack's common sound
     * archive.
     * @bug If the specified scene has no entry, this function's result is
     * undefined behavior. It reads from memory before the array of entries.
     *
     * @param id Scene ID (-1 to use the current scene)
     */
    bool isCommonSound(s32 id = -1);

private:
    /**
     * @brief Constructor
     *
     * @param pHeap Parent heap
     */
    RPSysSceneCreator(EGG::Heap* pHeap);
    /**
     * @brief Destructor
     */
    virtual ~RPSysSceneCreator();

// KOKESHI: We need to access private members in SceneCreator
#ifdef __KOKESHI__
public:
#else
private:
#endif
    //! Heap in which this object was allocated
    EGG::Heap* mpParentHeap; // at 0x4
    //! ID of the previous scene
    s32 mLastSceneID; // at 0x8

    //! Entries for all Pack Project scenes
    static SceneEntry sSceneEntryTable[];
};

//! @}

#endif
