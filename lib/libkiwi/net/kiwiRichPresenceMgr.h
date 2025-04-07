#ifndef LIBKIWI_NET_RICH_PRESENCE_MGR_H
#define LIBKIWI_NET_RICH_PRESENCE_MGR_H
#include <libkiwi/core/kiwiSceneHookMgr.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>
#include <libkiwi/util/kiwiStaticSingleton.h>

#include <revolution/OS.h>

namespace kiwi {

// Forward declarations
class IRichPresenceClient;

/**
 * @brief Rich presence application profile
 */
class IRichPresenceProfile {
public:
    /**
     * @brief Gets the Discord Developer Portal application ID
     */
    virtual String GetAppID() const = 0;

#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    /**
     * @brief Handles scene change event
     *
     * @param rClient Active rich presence client
     * @param scene Current scene ID
     */
    virtual void SceneCallback(IRichPresenceClient& rClient, s32 scene) {}
#elif defined(PACK_RESORT)
    /**
     * @brief Handles scene change event
     *
     * @param rClient Active rich presence client
     * @param scene Current scene ID
     * @param sequence Current sequence ID
     */
    virtual void SceneCallback(IRichPresenceClient& rClient, s32 scene,
                               s32 sequence) {}
#endif

    /**
     * @brief Handles periodic alarm event
     * @note The alarm period is 20 seconds.
     *
     * @param rClient Active rich presence client
     */
    virtual void AlarmCallback(IRichPresenceClient& rClient) {}
};

/**
 * @brief Discord rich presence manager
 */
class RichPresenceMgr : public StaticSingleton<RichPresenceMgr>,
                        public ISceneHook {
    friend class StaticSingleton<RichPresenceMgr>;

public:
    /**
     * @brief Sets the active rich presence application profile
     *
     * @param pProfile Application profile
     */
    void SetProfile(IRichPresenceProfile* pProfile);

private:
    //! Alarm period, in seconds
    static const int ALARM_PERIOD_SEC = 20;

private:
    /**
     * @brief Constructor
     */
    RichPresenceMgr();
    /**
     * @brief Destructor
     */
    ~RichPresenceMgr();

    /**
     * @brief Handles scene Reset event
     *
     * @param pScene Current scene
     */
    virtual void Reset(RPSysScene* pScene);

    /**
     * @brief Handles periodic alarm event
     *
     * @param pAlarm Alarm which fired
     * @param pCtx Context when the decrementer exception occurred
     */
    static void AlarmCallbackFunc(OSAlarm* pAlarm, OSContext* pCtx);

private:
    //! Active rich presence client
    IRichPresenceClient* mpClient;
    //! Active rich presence profile
    IRichPresenceProfile* mpProfile;

    //! Periodic alarm for updates
    OSAlarm mAlarm;
};

} // namespace kiwi

#endif
