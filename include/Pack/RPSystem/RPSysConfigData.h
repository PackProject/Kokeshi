#ifndef RP_SYSTEM_CONFIG_DATA_H
#define RP_SYSTEM_CONFIG_DATA_H
#include <Pack/RPAudio/RPSndAudioMgr.h>
#include <Pack/RPTypes.h>
#include <RPSystem/RPSysSceneMgr.h>
#include <egg/core.h>

//! @addtogroup rp_system
//! @{

/**
 * @brief Pack Project engine configuration
 */
class RPSysConfigData : public EGG::ConfigurationData {
public:
    /**
     * @brief Constructor
     */
    RPSysConfigData() : EGG::ConfigurationData(0) {}

    /**
     * @brief Gets the active video configuration
     */
    virtual EGG::Video* getVideo() {
        return mpVideo;
    } // at 0x8

    /**
     * @brief Gets the system heap
     */
    virtual EGG::Heap* getSystemHeap() {
        return mSystemHeap;
    } // at 0xC

    /**
     * @brief Gets the active display
     */
    virtual EGG::Display* getDisplay() {
        return mpAsyncDisplay;
    } // at 0x10

    /**
     * @brief Gets the active framebuffer manager
     */
    virtual EGG::XfbManager* getXfbMgr() {
        return mpXfbMgr;
    } // at 0x14

    /**
     * @brief Gets the active performance view
     */
    virtual EGG::PerformanceView* getPerfView() {
        return mpProcessMeter;
    } // at 0x18

    /**
     * @brief Gets the active scene manager
     */
    virtual EGG::SceneManager* getSceneMgr() {
        return mpSceneMgr;
    } // at 0x1C

    /**
     * @brief Gets the active audio manager
     */
    virtual EGG::IAudioMgr* getAudioMgr() {
        return mpAudioMgr;
    } // at 0x20

private:
    //! Video configuration
    EGG::Video* mpVideo; // at 0x38
    //! Framebuffer manager
    EGG::XfbManager* mpXfbMgr; // at 0x3C
    //! Asynchronous display
    EGG::AsyncDisplay* mpAsyncDisplay; // at 0x40
    //! Performance monitor display
    EGG::ProcessMeter* mpProcessMeter; // at 0x44
    //! Scene manager
    RPSysSceneMgr* mpSceneMgr; // at 0x48
    //! Sound/audio manager
    RPSndAudioMgr* mpAudioMgr; // at 0x4C
};

//! @}

#endif
