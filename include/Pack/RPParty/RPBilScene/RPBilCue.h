#ifndef RP_PARTY_BIL_CUE_H
#define RP_PARTY_BIL_CUE_H
#include "types_RP.h"

#include <RPUtility/RPUtlBaseFsm.h>

class RPBilCue {
public:
    enum EState {
        EState_Null,
        EState_PlaceFoul,
        EState_Wait,
        EState_Hold,
        EState_Pull,
        EState_FastTurn,
        EState_Hit,
        EState_Dummy
    };

private:
    char _00[0x68];
    RPUtlBaseFsm<RPBilCue>* mpStateMachine; // at 0x68
};

#endif
