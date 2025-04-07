#include <libkiwi.h>

#include <revolution/OS.h>

namespace kiwi {

/**
 * @brief Constructor
 */
RichPresenceMgr::RichPresenceMgr()
    : ISceneHook(-1), mpClient(nullptr), mpProfile(nullptr) {

    // TODO: Only Dolphin Emulator is supported for now
    mpClient = new EmuRichPresenceClient();
    K_ASSERT(mpClient != nullptr);

    // Start periodic alarm for routine updates
    OSCreateAlarm(&mAlarm);
    OSSetPeriodicAlarm(&mAlarm, OSGetTime(), OS_SEC_TO_TICKS(ALARM_PERIOD_SEC),
                       AlarmCallbackFunc);
}

/**
 * @brief Destructor
 */
RichPresenceMgr::~RichPresenceMgr() {
    OSCancelAlarm(&mAlarm);

    delete mpClient;
    mpClient = nullptr;

    delete mpProfile;
    mpProfile = nullptr;
}

/**
 * @brief Sets the active rich presence application profile
 *
 * @param pProfile Application profile
 */
void RichPresenceMgr::SetProfile(IRichPresenceProfile* pProfile) {
    mpProfile = pProfile;
    if (mpProfile == nullptr) {
        return;
    }

    K_ASSERT(mpClient != nullptr);
    mpClient->SetAppID(pProfile->GetAppID());
    mpClient->UpdateApp();
}

/**
 * @brief Handles scene Reset event
 *
 * @param pScene Current scene
 */
void RichPresenceMgr::Reset(RPSysScene* pScene) {
    K_ASSERT(pScene != nullptr);

    if (mpProfile == nullptr) {
        return;
    }

    K_ASSERT(mpClient != nullptr);

    // Allow user to update activity
#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    s32 scene = pScene->getSceneID();
    mpProfile->SceneCallback(*mpClient, scene);
#elif defined(PACK_RESORT)
    K_ASSERT_EX(false, "Not implemented.");
#endif
}

/**
 * @brief Handles periodic alarm event
 *
 * @param pAlarm Alarm which fired
 * @param pCtx Context when the decrementer exception occurred
 */
void RichPresenceMgr::AlarmCallbackFunc(OSAlarm* pAlarm, OSContext* pCtx) {
#pragma unused(pAlarm)
#pragma unused(pCtx)

    RichPresenceMgr& r = GetInstance();

    if (r.mpProfile == nullptr) {
        return;
    }

    K_ASSERT(r.mpClient != nullptr);

    // Allow user to update activity
    r.mpProfile->AlarmCallback(*r.mpClient);

    // Display most recent activity data
    r.mpClient->UpdateActivity();
}

} // namespace kiwi
