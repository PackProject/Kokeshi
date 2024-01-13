#ifndef EGG_AUDIO_AUDIO_MGR_H
#define EGG_AUDIO_AUDIO_MGR_H
#include "eggAudioArcPlayerMgr.h"
#include "eggAudioHeapMgr.h"
#include "eggAudioSystem.h"
#include "eggIAudioMgr.h"
#include "types_egg.h"

namespace EGG {

class SimpleAudioMgr : public IAudioMgr,
                       public SoundHeapMgr,
                       public ArcPlayer,
                       public AudioSystem {
public:
    struct SimpleAudioMgrArg : IAudioMgr::Arg {
        SimpleAudioMgrArg();
    };

    SimpleAudioMgr();
    virtual ~SimpleAudioMgr();                // at 0x80
    virtual void initialize(IAudioMgr::Arg*); // at 0x8
    virtual void calc();                      // at 0xC
};

} // namespace EGG

#endif
