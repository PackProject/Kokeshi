#define LIBKIWI_INTERNAL

#include <libkiwi.h>

namespace kiwi {

//! Pack Project scenes
const SceneCreator::Info SceneCreator::scPackScenes[] = {
// clang-format off
#ifdef PACK_SPORTS
    {nullptr, "Logo",                         "RPCommon/",             ESceneID_RPSysBootScene,               EPackID_AllPack,    ECreateType_0, EExitType_1,  true},
    {nullptr, "Player Select",                "RPCommon/",             ESceneID_RPSysPlayerSelectScene,       EPackID_AllPack,    ECreateType_0, EExitType_2,  true},
    {nullptr, "Nunchuk Check",                "RPCommon/",             ESceneID_RPSysNunchukScene,            EPackID_AllPack,    ECreateType_1, EExitType_3, false},
    {nullptr, "Baseball",                     "RPBsbScene/",           ESceneID_RPBsbScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {nullptr, "Bowling",                      "RPBowScene/",           ESceneID_RPBowScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {nullptr, "Golf",                         "RPGolScene/",           ESceneID_RPGolScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {nullptr, "Tennis",                       "RPTnsScene/",           ESceneID_RPTnsScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {nullptr, "Boxing",                       "RPBoxScene/",           ESceneID_RPBoxScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {nullptr, "Sports Pack",                  "RPSportsTitle/",        ESceneID_RPSportsTitleScene,           EPackID_SportsPack, ECreateType_0, EExitType_1,  true},
    {nullptr, "Sports Menu Select",           "RPSportsCommon/",       ESceneID_RPSportsMenuScene,            EPackID_SportsPack, ECreateType_0, EExitType_2,  true},
    {nullptr, "Training Menu",                "RPSportsTrainingMenu/", ESceneID_RPSportsTrainingMenuScene,    EPackID_SportsPack, ECreateType_1, EExitType_3,  true},
    {nullptr, "Physical Measure Menu",        "RPSportsPhysical/",     ESceneID_RPSportsPhysicalMenuScene,    EPackID_SportsPack, ECreateType_1, EExitType_3,  true},
    {nullptr, "Physical Measure Explanation", "RPSportsPhysical/",     ESceneID_RPSportsPhysicalPreviewScene, EPackID_SportsPack, ECreateType_1, EExitType_3,  true},
    {nullptr, "Physical Measure Results",     "RPSportsPhysical/",     ESceneID_RPSportsPhysicalResultScene,  EPackID_SportsPack, ECreateType_1, EExitType_3,  true},
    {nullptr, "Golf Course Select",           "RPGolScene/",           ESceneID_RPGolSelectScene,             EPackID_SportsPack, ECreateType_1, EExitType_0, false},
#elif PACK_PLAY
    {nullptr, "Logo",                         "RPCommonParty/",        ESceneID_RPSysBootScene,               EPackID_AllPack,    ECreateType_0, EExitType_1,  true},
    {nullptr, "Player Select",                "RPCommonParty/",        ESceneID_RPSysPlayerSelectScene,       EPackID_AllPack,    ECreateType_0, EExitType_2,  true},
    {nullptr, "Nunchuk Check",                "RPCommonParty/",        ESceneID_RPSysNunchukScene,            EPackID_AllPack,    ECreateType_1, EExitType_3, false},
    {nullptr, "Fishing",                      "RPFshScene/",           ESceneID_RPFshScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {nullptr, "Hockey",                       "RPHkyScene/",           ESceneID_RPHkyScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {nullptr, "Shooting",                     "RPDucScene/",           ESceneID_RPDucScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {nullptr, "Table Tennis",                 "RPPnpScene/",           ESceneID_RPPnpScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {nullptr, "Billiards",                    "RPBilScene/",           ESceneID_RPBilScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {nullptr, "Cow Race",                     "RPCowScene/",           ESceneID_RPCowScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {nullptr, "Wally",                        "RPWlyScene/",           ESceneID_RPWlyScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {nullptr, "Tank",                         "RPTnkScene/",           ESceneID_RPTnkScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {nullptr, "Bomb",                         "RPBomScene/",           ESceneID_RPBomScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {nullptr, "Party Pack",                   "RPPartyTitle/",         ESceneID_RPPartyTitleScene,            EPackID_PartyPack,  ECreateType_0, EExitType_1,  true},
    {nullptr, "Tour Menu",                    "RPPartyCommon/",        ESceneID_RPPartyMiiLoadScene,          EPackID_PartyPack,  ECreateType_0, EExitType_4,  true},
    {nullptr, "Tour Menu",                    "RPPartyCommon/",        ESceneID_RPPartyMenuScene,             EPackID_PartyPack,  ECreateType_1, EExitType_3,  true},
#elif PACK_RESORT
    {nullptr, "Logo",                         "RPCommon/",             ESceneID_RPSysBootScene,               EPackID_AllPack,    ECreateType_0, EExitType_1,  true},
#endif
    // clang-format on
};

//! User-registered scenes
TMap<s32, SceneCreator::Info> SceneCreator::sUserScenes;

/**
 * @brief Gets information on the specified scene
 *
 * @param id Scene ID
 */
const SceneCreator::Info* SceneCreator::GetSceneInfo(s32 id) {
    const Info* pInfo;

    // Supply -1 to use the current scene ID
    if (id < 0) {
        id = RP_GET_INSTANCE(RPSysSceneMgr)->getCurrentSceneID();
    }

    // Check user scenes first
    pInfo = sUserScenes.Find(id);
    if (pInfo != nullptr) {
        return pInfo;
    }

    // Check RP scenes
    for (int i = 0; i < LENGTHOF(scPackScenes); i++) {
        pInfo = &scPackScenes[i];

        if (pInfo->id == id) {
            return pInfo;
        }
    }

    return nullptr;
}

/**
 * @brief Registers user scene class
 *
 * @param rInfo Scene info
 */
void SceneCreator::RegistScene(const Info& rInfo) {
    K_ASSERT_EX(rInfo.id >= 0, "Invalid scene ID");

    // Don't allow duplicate scene IDs
    const Info* pInfo = GetSceneInfo(rInfo.id);
    K_ASSERT_EX(pInfo == nullptr, "Scene ID %d already used (%s)",
                pInfo->name.CStr());

    sUserScenes.Insert(rInfo.id, rInfo);
}

/**
 * @brief Fades out into a new scene
 *
 * @param id Scene ID
 * @param arg1 Unknown
 */
bool SceneCreator::ChangeSceneAfterFade(s32 id, bool arg1) {
    // Ensure all threads are idle
    if (!RP_GET_INSTANCE(RPSysSceneMgr)->isTaskFinished()) {
        return false;
    }

    // Optionally reload current scene
    s32 current = RP_GET_INSTANCE(RPSysSceneMgr)->getCurrentSceneID();
    if (arg1) {
        id = current;
    }

    // Send request to scene manager
    bool success =
        RP_GET_INSTANCE(RPSysSceneMgr)->changeNextSceneAfterFade(id, arg1);

    // Fade out common sounds if we are switching sound archives
    if (GetSceneCommonSound(id) != GetSceneCommonSound(current)) {
        s16 frame = RP_GET_INSTANCE(RPSysSceneMgr)->getFadeFrame();
        RP_GET_INSTANCE(RPSndAudioMgr)->setSystemSeFadeInFrame(frame);
    }

    // Update last scene for exiting
    if (GetSceneExitType(id) != EExitType_4) {
        setLastSceneID(current);
    }

    return success;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                                       // Wii Sports
                KM_BRANCH_MF(0x8018459c, SceneCreator, ChangeSceneAfterFade), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                      // Wii Sports Resort
// clang-format on

/**
 * @brief Gets the specified scene's name
 */
const char* SceneCreator::GetSceneName(s32 id) const {
    const Info* pInfo = GetSceneInfo(id);
    return pInfo->name;
}

/**
 * @brief Gets the specified scene's resource directory name
 */
const char* SceneCreator::GetSceneDirectory(s32 id) const {
    const Info* pInfo = GetSceneInfo(id);
    return pInfo->dir;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                                    // Wii Sports
                KM_BRANCH_MF(0x80184124, SceneCreator, GetSceneDirectory), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                   // Wii Sports Resort
// clang-format on

/**
 * @brief Gets the specified scene's target pack
 */
EPackID SceneCreator::GetScenePack(s32 id) const {
    const Info* pInfo = GetSceneInfo(id);
    return pInfo->pack;
}

/**
 * @brief Gets the specified scene's create type
 */
ECreateType SceneCreator::GetSceneCreateType(s32 id) const {
    const Info* pInfo = GetSceneInfo(id);
    return pInfo->create;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                                     // Wii Sports
                KM_BRANCH_MF(0x8018402c, SceneCreator, GetSceneCreateType), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                    // Wii Sports Resort
// clang-format on

/**
 * @brief Gets the specified scene's exit type
 */
EExitType SceneCreator::GetSceneExitType(s32 id) const {
    const Info* pInfo = GetSceneInfo(id);
    return pInfo->exit;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                                   // Wii Sports
                KM_BRANCH_MF(0x80183f90, SceneCreator, GetSceneExitType), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                  // Wii Sports Resort
// clang-format on

/**
 * @brief Tests whether the specified scene requires the RP common sound
 * archive
 */
bool SceneCreator::GetSceneCommonSound(s32 id) const {
    const Info* pInfo = GetSceneInfo(id);
    return pInfo->common;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                                      // Wii Sports
                KM_BRANCH_MF(0x80183ef4, SceneCreator, GetSceneCommonSound), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                     // Wii Sports Resort
// clang-format on

/**
 * @brief Creates a new scene by ID
 *
 * @param id Scene ID
 */
RPSysScene* SceneCreator::Create(s32 id) {
    RPSysScene* scene = nullptr;

    // Check for user scene ID
    if (id >= ESceneID_Max) {
        scene = CreateUserScene(id);
    } else {
        // Either system (all pack) or sports scene
        switch (GetScenePack(id)) {
        case EPackID_SportsPack:
        case EPackID_PartyPack:  scene = CreatePackScene(id); break;
        case EPackID_AllPack:    scene = CreateSystemScene(id); break;
        case EPackID_MusicPack:
        case EPackID_HealthPack:
        default:                 K_ASSERT_EX(false, "Invalid pack ID"); break;
        }
    }

    K_ASSERT_EX(scene != nullptr, "Failed to create scene (id %d)", id);

    if (GetSceneExitType(id) == EExitType_4) {
        scene->disableFadeIn();
    }

    scene->setCreatorSceneID(getLastSceneID());
    return scene;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                         // Wii Sports
                KM_BRANCH_MF(0x80184230, SceneCreator, Create), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                        // Wii Sports Resort
// clang-format on

/**
 * @brief Creates a new system scene by ID
 *
 * @param id Scene ID
 */
RPSysScene* SceneCreator::CreateSystemScene(s32 id) {
#ifdef PACK_RESORT
    K_ASSERT_EX(false, "Not for this pack yet.");
    return nullptr;
#else
    switch (id) {
    case ESceneID_RPSysBootScene:         return new RPSysBootScene();
    case ESceneID_RPSysPlayerSelectScene: return new RPSysPlayerSelectScene();
    case ESceneID_RPSysNunchukScene:      return new RPSysNunchukScene();
    default:                              return nullptr;
    }
#endif
}

/**
 * @brief Creates a new game scene by ID
 *
 * @param id Scene ID
 */
RPSysScene* SceneCreator::CreatePackScene(s32 id) {
    EGG::Scene* scene = nullptr;

#ifdef PACK_SPORTS
    scene = RPSysSceneCreator::createSportsScene(id);
#elif PACK_PLAY
    scene = RPSysSceneCreator::createPartyScene(id);
#elif PACK_RESORT
    scene = RPSysSceneCreator::createWS2Scene(id);
#endif

    return static_cast<RPSysScene*>(scene);
}

/**
 * @brief Creates a new user scene by ID
 *
 * @param id Scene ID
 */
RPSysScene* SceneCreator::CreateUserScene(s32 id) {
    const Info* pInfo = GetSceneInfo(id);
    K_ASSERT_EX(pInfo != nullptr, "Cannot create scene ID %d", id);
    return pInfo->pCt();
}

} // namespace kiwi
