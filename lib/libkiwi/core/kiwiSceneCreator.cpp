#define LIBKIWI_INTERNAL

#include <kokeshi.hpp>
#include <libkiwi.h>

namespace kiwi {

/**
 * @brief User-registered scenes
 */
TMap<s32, SceneCreator::Info> SceneCreator::sUserScenes;

/**
 * @brief Pack Project scenes
 */
const SceneCreator::Info SceneCreator::scPackScenes[] = {
// clang-format off
#ifdef PACK_SPORTS
    {NULL, "Logo",                         "RPCommon/",             ESceneID_RPSysBootScene,               EPackID_AllPack,    ECreateType_0, EExitType_1,  true},
    {NULL, "Player Select",                "RPCommon/",             ESceneID_RPSysPlayerSelectScene,       EPackID_AllPack,    ECreateType_0, EExitType_2,  true},
    {NULL, "Nunchuk Check",                "RPCommon/",             ESceneID_RPSysNunchukScene,            EPackID_AllPack,    ECreateType_1, EExitType_3, false},
    {NULL, "Baseball",                     "RPBsbScene/",           ESceneID_RPBsbScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {NULL, "Bowling",                      "RPBowScene/",           ESceneID_RPBowScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {NULL, "Golf",                         "RPGolScene/",           ESceneID_RPGolScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {NULL, "Tennis",                       "RPTnsScene/",           ESceneID_RPTnsScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {NULL, "Boxing",                       "RPBoxScene/",           ESceneID_RPBoxScene,                   EPackID_SportsPack, ECreateType_1, EExitType_0, false},
    {NULL, "Sports Pack",                  "RPSportsTitle/",        ESceneID_RPSportsTitleScene,           EPackID_SportsPack, ECreateType_0, EExitType_1,  true},
    {NULL, "Sports Menu Select",           "RPSportsCommon/",       ESceneID_RPSportsMenuScene,            EPackID_SportsPack, ECreateType_0, EExitType_2,  true},
    {NULL, "Training Menu",                "RPSportsTrainingMenu/", ESceneID_RPSportsTrainingMenuScene,    EPackID_SportsPack, ECreateType_1, EExitType_3,  true},
    {NULL, "Physical Measure Menu",        "RPSportsPhysical/",     ESceneID_RPSportsPhysicalMenuScene,    EPackID_SportsPack, ECreateType_1, EExitType_3,  true},
    {NULL, "Physical Measure Explanation", "RPSportsPhysical/",     ESceneID_RPSportsPhysicalPreviewScene, EPackID_SportsPack, ECreateType_1, EExitType_3,  true},
    {NULL, "Physical Measure Results",     "RPSportsPhysical/",     ESceneID_RPSportsPhysicalResultScene,  EPackID_SportsPack, ECreateType_1, EExitType_3,  true},
    {NULL, "Golf Course Select",           "RPGolScene/",           ESceneID_RPGolSelectScene,             EPackID_SportsPack, ECreateType_1, EExitType_0, false},
#elif PACK_PLAY
    {NULL, "Logo",                         "RPCommonParty/",        ESceneID_RPSysBootScene,               EPackID_AllPack,    ECreateType_0, EExitType_1,  true},
    {NULL, "Player Select",                "RPCommonParty/",        ESceneID_RPSysPlayerSelectScene,       EPackID_AllPack,    ECreateType_0, EExitType_2,  true},
    {NULL, "Nunchuk Check",                "RPCommonParty/",        ESceneID_RPSysNunchukScene,            EPackID_AllPack,    ECreateType_1, EExitType_3, false},
    {NULL, "Fishing",                      "RPFshScene/",           ESceneID_RPFshScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {NULL, "Hockey",                       "RPHkyScene/",           ESceneID_RPHkyScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {NULL, "Shooting",                     "RPDucScene/",           ESceneID_RPDucScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {NULL, "Table Tennis",                 "RPPnpScene/",           ESceneID_RPPnpScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {NULL, "Billiards",                    "RPBilScene/",           ESceneID_RPBilScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {NULL, "Cow Race",                     "RPCowScene/",           ESceneID_RPCowScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {NULL, "Wally",                        "RPWlyScene/",           ESceneID_RPWlyScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {NULL, "Tank",                         "RPTnkScene/",           ESceneID_RPTnkScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {NULL, "Bomb",                         "RPBomScene/",           ESceneID_RPBomScene,                   EPackID_PartyPack,  ECreateType_1, EExitType_0, false},
    {NULL, "Party Pack",                   "RPPartyTitle/",         ESceneID_RPPartyTitleScene,            EPackID_PartyPack,  ECreateType_0, EExitType_1,  true},
    {NULL, "Tour Menu",                    "RPPartyCommon/",        ESceneID_RPPartyMiiLoadScene,          EPackID_PartyPack,  ECreateType_0, EExitType_4,  true},
    {NULL, "Tour Menu",                    "RPPartyCommon/",        ESceneID_RPPartyMenuScene,             EPackID_PartyPack,  ECreateType_1, EExitType_3,  true},
#elif PACK_RESORT
    {NULL, "Logo",                         "RPCommon/",             ESceneID_RPSysBootScene,               EPackID_AllPack,    ECreateType_0, EExitType_1,  true},
#endif
    // clang-format on
};

/**
 * @brief Get information on the specified scene
 *
 * @param id Scene ID
 */
const SceneCreator::Info* SceneCreator::GetSceneInfo(s32 id) {
    const Info* info;

    // Supply -1 to use the current scene ID
    if (id < 0) {
        id = RP_GET_INSTANCE(RPSysSceneMgr)->getCurrentSceneID();
    }

    // Check user scenes first
    info = sUserScenes.Find(id);
    if (info != NULL) {
        return info;
    }

    // Check RP scenes
    for (int i = 0; i < LENGTHOF(scPackScenes); i++) {
        info = &scPackScenes[i];

        if (info->id == id) {
            return info;
        }
    }

    return NULL;
}

/**
 * @brief Access singleton instance
 */
SceneCreator& SceneCreator::GetInstance() {
    return *static_cast<SceneCreator*>(RP_GET_INSTANCE(RPSysSceneCreator));
}

/**
 * @brief Register user scene class
 */
void SceneCreator::RegistScene(const Info& info) {
    K_ASSERT_EX(info.id >= 0, "Invalid scene ID");

    // Don't allow duplicate scene IDs
    const Info* p = GetSceneInfo(info.id);
    K_ASSERT_EX(p == NULL, "Scene ID %d already used (%s)", p->name.CStr());

    sUserScenes.Insert(info.id, info);
}

/**
 * @brief Fade out into a new scene
 *
 * @param id Scene ID (-1 to reload scene)
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
 * @brief Get the specified scene's name
 */
const char* SceneCreator::GetSceneName(s32 id) const {
    const Info* info = GetSceneInfo(id);
    return info->name;
}

/**
 * @brief Get the scene's resource directory name
 */
const char* SceneCreator::GetSceneDirectory(s32 id) const {
    const Info* info = GetSceneInfo(id);
    return info->dir;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                                    // Wii Sports
                KM_BRANCH_MF(0x80184124, SceneCreator, GetSceneDirectory), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                   // Wii Sports Resort
// clang-format on

/**
 * @brief Get the scene's supported Pack Project
 */
EPackID SceneCreator::GetScenePack(s32 id) const {
    const Info* info = GetSceneInfo(id);
    return info->pack;
}

/**
 * @brief Get the scene's create type
 */
ECreateType SceneCreator::GetSceneCreateType(s32 id) const {
    const Info* info = GetSceneInfo(id);
    return info->create;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                                     // Wii Sports
                KM_BRANCH_MF(0x8018402c, SceneCreator, GetSceneCreateType), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                    // Wii Sports Resort
// clang-format on

/**
 * @brief Get the scene's exit type
 */
EExitType SceneCreator::GetSceneExitType(s32 id) const {
    const Info* info = GetSceneInfo(id);
    return info->exit;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                                   // Wii Sports
                KM_BRANCH_MF(0x80183f90, SceneCreator, GetSceneExitType), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                  // Wii Sports Resort
// clang-format on

/**
 * @brief Whether the scene requires the RP common sound archive
 */
bool SceneCreator::GetSceneCommonSound(s32 id) const {
    const Info* info = GetSceneInfo(id);
    return info->common;
}
// clang-format off
KOKESHI_BY_PACK(KOKESHI_NOTIMPLEMENTED,                                      // Wii Sports
                KM_BRANCH_MF(0x80183ef4, SceneCreator, GetSceneCommonSound), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                                     // Wii Sports Resort
// clang-format on

/**
 * @brief Create a new scene by ID
 *
 * @param id Scene ID
 */
RPSysScene* SceneCreator::Create(s32 id) {
    RPSysScene* scene = NULL;

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

    K_ASSERT_EX(scene != NULL, "Failed to create scene (id %d)", id);

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
 * @brief Create a new system scene by ID
 *
 * @param id Scene ID
 */
RPSysScene* SceneCreator::CreateSystemScene(s32 id) {
#ifdef PACK_RESORT
    K_ASSERT_EX(false, "Not for this pack yet.");
    return NULL;
#else
    switch (id) {
    case ESceneID_RPSysBootScene:         return new RPSysBootScene();
    case ESceneID_RPSysPlayerSelectScene: return new RPSysPlayerSelectScene();
    case ESceneID_RPSysNunchukScene:      return new RPSysNunchukScene();
    default:                              return NULL;
    }
#endif
}

/**
 * @brief Create a new game scene by ID
 *
 * @param id Scene ID
 */
RPSysScene* SceneCreator::CreatePackScene(s32 id) {
    EGG::Scene* scene = NULL;

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
 * @brief Create a new user scene by ID
 *
 * @param id Scene ID
 */
RPSysScene* SceneCreator::CreateUserScene(s32 id) {
    const Info* info = GetSceneInfo(id);
    K_ASSERT_EX(info != NULL, "Cannot create scene ID %d", id);
    return info->ct();
}

} // namespace kiwi
