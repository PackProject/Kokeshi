#ifndef EGG_AUDIO_AUDIO_FX_MGR_H
#define EGG_AUDIO_AUDIO_FX_MGR_H
#include "types_egg.h"
#include <nw4r/snd/snd_Types.h>

namespace EGG {

class AudioFx {
public:
    AudioFx();
    ~AudioFx();
    void init(nw4r::snd::AuxBus, void*, u32);
    void destroy();

private:
    UNKWORD WORD_0x0;
    nw4r::snd::AuxBus mAuxBus; // at 0x4
    UNKWORD WORD_0x8;
    UNKWORD WORD_0xC;
    ExpHeap* mExpHeap; // at 0x10
    UNKWORD WORD_0x14;
};

class AudioFxMgr {
public:
    struct AudioFxMgrArg {
        u32 neededAllocSize[nw4r::snd::AUX_BUS_NUM]; // at 0x0
    };

    AudioFxMgr();
    ~AudioFxMgr();

private:
    AudioFx mAudioFx[nw4r::snd::AUX_BUS_NUM]; // at 0x0
};

} // namespace EGG

#endif
