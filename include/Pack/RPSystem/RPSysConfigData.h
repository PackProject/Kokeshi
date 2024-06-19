#ifndef RP_SYSTEM_CONFIG_DATA_H
#define RP_SYSTEM_CONFIG_DATA_H
#include "RPTypes.h"

#include <RPAudio/RPSndAudioMgr.h>
#include <RPSystem/RPSysSceneMgr.h>
#include <egg/core.h>

class RPSysConfigData : public EGG::ConfigurationData {
public:
    RPSysConfigData() : EGG::ConfigurationData(0) {}

    virtual EGG::Video* getVideo() {
        return mVideo;
    } // at 0x8

    virtual EGG::Heap* getSystemHeap() {
        return mSystemHeap;
    } // at 0xC

    virtual EGG::Display* getDisplay() {
        return mAsyncDisplay;
    } // at 0x10

    virtual EGG::XfbManager* getXfbMgr() {
        return mXfbMgr;
    } // at 0x14

    virtual EGG::PerformanceView* getPerfView() {
        return mProcessMeter;
    } // at 0x18

    virtual EGG::SceneManager* getSceneMgr() {
        return mSceneMgr;
    } // at 0x1C

    virtual EGG::IAudioMgr* getAudioMgr() {
        return mAudioMgr;
    } // at 0x20

private:
    EGG::Video* mVideo;               // at 0x38
    EGG::XfbManager* mXfbMgr;         // at 0x3C
    EGG::AsyncDisplay* mAsyncDisplay; // at 0x40
    EGG::ProcessMeter* mProcessMeter; // at 0x44
    RPSysSceneMgr* mSceneMgr;         // at 0x48
    RPSndAudioMgr* mAudioMgr;         // at 0x4C
};

#endif
