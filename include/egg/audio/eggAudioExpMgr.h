#ifndef EGG_AUDIO_AUDIO_EXP_MGR_H
#define EGG_AUDIO_AUDIO_EXP_MGR_H
#include "eggAudioFxMgr.h"
#include "eggAudioMgr.h"
#include "eggAudioSound3DMgr.h"
#include "types_egg.h"

namespace EGG {

class ExpAudioMgr : public SimpleAudioMgr,
                    public TAudioSound3DMgr<4>,
                    public AudioFxMgr {};

} // namespace EGG

#endif
