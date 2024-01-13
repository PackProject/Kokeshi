#ifndef EGG_AUDIO_HEAP_MGR_H
#define EGG_AUDIO_HEAP_MGR_H
#include "snd_SoundHeap.h"
#include "types_egg.h"

#include <revolution/OS.h>

namespace EGG {
class SoundHeapMgr {
public:
    SoundHeapMgr() : mHeap() {
        OSInitMessageQueue(&mMesgQueue1, &mMesg1, 4);
        OSInitMessageQueue(&mMesgQueue2, &mMesg2, 4);
        OSInitMessageQueue(&mMesgQueue3, &mMesg3, 4);
    }

    ~SoundHeapMgr() {
        destroySoundHeap();
    }

    // TO-DO: Implement these weak funcs
    virtual UNKWORD loadState(s32);    // at 0x8
    virtual UNKWORD getCurrentLevel(); // at 0xC
    virtual void saveState();          // at 0x10

    void createSoundHeap(Heap*, u32);
    void createSoundHeap(Allocator*, u32);
    void destroySoundHeap();

    nw4r::snd::SoundHeap* getSoundHeap() {
        return &mHeap;
    }

private:
    nw4r::snd::SoundHeap mHeap; // at 0x4
    u8 UNK_0x38[0x68 - 0x38];
    OSMessageQueue mMesgQueue1; // at 0x68
    OSMessage mMesg1;           // at 0x88
    u8 UNK_0x8C[0x98 - 0x8C];
    OSMessageQueue mMesgQueue2; // at 0x98
    OSMessage mMesg2;           // at 0xB8
    u8 UNK_0xBC[0xC8 - 0xBC];
    OSMessageQueue mMesgQueue3; // at 0xC8
    OSMessage mMesg3;           // at 0xE8
    u8 UNK_0xEC[0xF8 - 0xEC];
};
} // namespace EGG

#endif
