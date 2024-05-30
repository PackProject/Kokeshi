#ifndef RP_AUDIO_SND_AUDIO_MANAGER_H
#define RP_AUDIO_SND_AUDIO_MANAGER_H
#include <egg/audio.h>
#include <egg/core.h>
#include <nw4r/snd.h>
#include <types_RP.h>

/**
 * @brief RP sound manager (BGM, SFX, etc.)
 */
class RPSndAudioMgr : public EGG::ExpAudioMgr {
public:
    // @address 801b82dc
    static RPSndAudioMgr* CreateInstance(EGG::Heap* heap);
    static RPSndAudioMgr* GetInstance() {
        return sInstance;
    }

    /* virtual */ bool loadGroup(UNKWORD, nw4r::snd::SoundHeap*, UNKWORD);

    void setSystemSeFadeInFrame(s16 frame);

    bool startSound(u32 id) {
        return RPSndAudioMgr::startSound(&mSndHandle, id);
    }
    bool startSound(const char* name) {
        return RPSndAudioMgr::startSound(&mSndHandle, name);
    }

    void stopAllSoud();

    bool attachArchive(const char* archiveName, bool bStaticPath,
                       nw4r::snd::SoundHeap* heap);
    bool startSound(nw4r::snd::SoundHandle* handle, u32 id);
    bool startSound(nw4r::snd::SoundHandle* handle, const char* name);

    void changeScene();

private:
    char UNK_0x0[0x928];
    nw4r::snd::SoundHandle mSndHandle; // at 0x928

    static RPSndAudioMgr* sInstance;
};

#endif
