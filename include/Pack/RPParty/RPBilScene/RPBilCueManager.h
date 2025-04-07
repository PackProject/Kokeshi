#ifndef RP_PARTY_BIL_CUE_MANAGER_H
#define RP_PARTY_BIL_CUE_MANAGER_H
#include "RPTypes.h"
#include <RPBilScene/RPBilCue.h>
#include <RPUtility/RPUtlBaseFsm.h>

class RPBilCueManager {
    RP_SINGLETON_DECL(RPBilCueManager);

public:
    RPBilCue* GetCue(int i) {
        return mCues[i];
    }

private:
    static const int scCueNum = 2;

    int mCueNum; // at 0x4
    char _08[0x10 - 0x8];
    RPBilCue* mCues[scCueNum]; // at 0x10
};

#endif
