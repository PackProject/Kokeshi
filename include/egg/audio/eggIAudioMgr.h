#ifndef EGG_AUDIO_I_AUDIO_MGR_H
#define EGG_AUDIO_I_AUDIO_MGR_H
#include "types_egg.h"

namespace EGG {
class IAudioMgr {
public:
    struct Arg {
        Arg() {
            mHeap = NULL;
            WORD_0x8 = 4;
            WORD_0xC = 3;
            WORD_0x4 = 0;
            WORD_0x10 = 0;
            WORD_0x14 = 0;
            WORD_0x18 = 1;
            WORD_0x1C = 0x8CA000;
        }

        Heap* mHeap;
        UNKWORD WORD_0x4;
        s32 WORD_0x8;
        s32 WORD_0xC;
        UNKWORD WORD_0x10;
        UNKWORD WORD_0x14;
        UNKWORD WORD_0x18;
        u32 WORD_0x1C;
    };

    virtual void initialize(Arg*) {}
    virtual void calc() = 0;

protected:
    bool mIsInitialized; // at 0x4
};
} // namespace EGG

#endif
