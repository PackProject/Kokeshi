#ifndef EGG_AUDIO_AUDIO_SOUND_3D_MGR_H
#define EGG_AUDIO_AUDIO_SOUND_3D_MGR_H
#include <nw4r/snd/snd_Sound3DListener.h>
#include <nw4r/snd/snd_Sound3DManager.h>

namespace EGG {

template <int N> class TAudioSound3DMgr {
private:
    nw4r::snd::Sound3DManager mManagers[N];
    nw4r::snd::Sound3DListener mListeners[N];
};

} // namespace EGG

#endif
