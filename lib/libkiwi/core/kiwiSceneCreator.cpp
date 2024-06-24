#define LIBKIWI_INTERNAL

#include <Pack/RPSystem.h>
#include <libkiwi.h>

namespace kiwi {

//! Pack Project scenes
const SceneCreator::Info SceneCreator::scPackScenes[] = {
// clang-format off
#ifdef PACK_SPORTS
    {nullptr, "Logo",                         "RPCommon/",             kiwi::ESceneID_RPSysBootScene,               kiwi::EPackID_AllPack,    kiwi::ECreateType_Standard, kiwi::EExitType_Sibling,  true},
    {nullptr, "Player Select",                "RPCommon/",             kiwi::ESceneID_RPSysPlayerSelectScene,       kiwi::EPackID_AllPack,    kiwi::ECreateType_Standard, kiwi::EExitType_2,        true},
    {nullptr, "Nunchuk Check",                "RPCommon/",             kiwi::ESceneID_RPSysNunchukScene,            kiwi::EPackID_AllPack,    kiwi::ECreateType_Sibling,  kiwi::EExitType_3,        false},
    {nullptr, "Baseball",                     "RPBsbScene/",           kiwi::ESceneID_RPBsbScene,                   kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Bowling",                      "RPBowScene/",           kiwi::ESceneID_RPBowScene,                   kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Golf",                         "RPGolScene/",           kiwi::ESceneID_RPGolScene,                   kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Tennis",                       "RPTnsScene/",           kiwi::ESceneID_RPTnsScene,                   kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Boxing",                       "RPBoxScene/",           kiwi::ESceneID_RPBoxScene,                   kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Sports Pack",                  "RPSportsTitle/",        kiwi::ESceneID_RPSportsTitleScene,           kiwi::EPackID_SportsPack, kiwi::ECreateType_Standard, kiwi::EExitType_Sibling,  true},
    {nullptr, "Sports Menu Select",           "RPSportsCommon/",       kiwi::ESceneID_RPSportsMenuScene,            kiwi::EPackID_SportsPack, kiwi::ECreateType_Standard, kiwi::EExitType_2,        true},
    {nullptr, "Training Menu",                "RPSportsTrainingMenu/", kiwi::ESceneID_RPSportsTrainingMenuScene,    kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_3,        true},
    {nullptr, "Physical Measure Menu",        "RPSportsPhysical/",     kiwi::ESceneID_RPSportsPhysicalMenuScene,    kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_3,        true},
    {nullptr, "Physical Measure Explanation", "RPSportsPhysical/",     kiwi::ESceneID_RPSportsPhysicalPreviewScene, kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_3,        true},
    {nullptr, "Physical Measure Results",     "RPSportsPhysical/",     kiwi::ESceneID_RPSportsPhysicalResultScene,  kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_3,        true},
    {nullptr, "Golf Course Select",           "RPGolScene/",           kiwi::ESceneID_RPGolSelectScene,             kiwi::EPackID_SportsPack, kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
#elif PACK_PLAY
    {nullptr, "Logo",                         "RPCommonParty/",        kiwi::ESceneID_RPSysBootScene,               kiwi::EPackID_AllPack,    kiwi::ECreateType_Standard, kiwi::EExitType_Sibling,  true},
    {nullptr, "Player Select",                "RPCommonParty/",        kiwi::ESceneID_RPSysPlayerSelectScene,       kiwi::EPackID_AllPack,    kiwi::ECreateType_Standard, kiwi::EExitType_2,        true},
    {nullptr, "Nunchuk Check",                "RPCommonParty/",        kiwi::ESceneID_RPSysNunchukScene,            kiwi::EPackID_AllPack,    kiwi::ECreateType_Sibling,  kiwi::EExitType_3,        false},
    {nullptr, "Fishing",                      "RPFshScene/",           kiwi::ESceneID_RPFshScene,                   kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Hockey",                       "RPHkyScene/",           kiwi::ESceneID_RPHkyScene,                   kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Shooting",                     "RPDucScene/",           kiwi::ESceneID_RPDucScene,                   kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Table Tennis",                 "RPPnpScene/",           kiwi::ESceneID_RPPnpScene,                   kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Billiards",                    "RPBilScene/",           kiwi::ESceneID_RPBilScene,                   kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Cow Race",                     "RPCowScene/",           kiwi::ESceneID_RPCowScene,                   kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Wally",                        "RPWlyScene/",           kiwi::ESceneID_RPWlyScene,                   kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Tank",                         "RPTnkScene/",           kiwi::ESceneID_RPTnkScene,                   kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Bomb",                         "RPBomScene/",           kiwi::ESceneID_RPBomScene,                   kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_Standard, false},
    {nullptr, "Party Pack",                   "RPPartyTitle/",         kiwi::ESceneID_RPPartyTitleScene,            kiwi::EPackID_PartyPack,  kiwi::ECreateType_Standard, kiwi::EExitType_Sibling,  true},
    {nullptr, "Tour Menu",                    "RPPartyCommon/",        kiwi::ESceneID_RPPartyMiiLoadScene,          kiwi::EPackID_PartyPack,  kiwi::ECreateType_Standard, kiwi::EExitType_Child,    true},
    {nullptr, "Tour Menu",                    "RPPartyCommon/",        kiwi::ESceneID_RPPartyMenuScene,             kiwi::EPackID_PartyPack,  kiwi::ECreateType_Sibling,  kiwi::EExitType_3,        true},
#elif PACK_RESORT
    {nullptr, "Logo",                         "RPCommon/",             kiwi::ESceneID_RPSysBootScene,               kiwi::EPackID_AllPack,    kiwi::ECreateType_Standard, kiwi::EExitType_Sibling,  true},
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
 * @param reload Reload the current scene
 */
bool SceneCreator::ChangeSceneAfterFade(s32 id, bool reload) {
    // Ensure all threads are idle
    if (!RP_GET_INSTANCE(RPSysSceneMgr)->isTaskFinished()) {
        return false;
    }

    // Optionally reload current scene
    s32 current = RP_GET_INSTANCE(RPSysSceneMgr)->getCurrentSceneID();
    if (reload) {
        id = current;
    }

    // Send request to scene manager
    bool success =
        RP_GET_INSTANCE(RPSysSceneMgr)->changeNextSceneAfterFade(id, reload);

    // Fade out common sounds if we are switching sound archives
    if (GetSceneCommonSound(id) != GetSceneCommonSound(current)) {
        s16 frame = RP_GET_INSTANCE(RPSysSceneMgr)->getSceneFadeFrame();
        RP_GET_INSTANCE(RPSndAudioMgr)->setSystemSeFadeInFrame(frame);
    }

    // Update last scene for exiting
    if (GetSceneExitType(id) != EExitType_Child) {
        mLastSceneID = current;
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

    if (GetSceneExitType(id) == EExitType_Child) {
        scene->setEnterFast();
    }

    scene->setCreatorSceneID(mLastSceneID);
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
