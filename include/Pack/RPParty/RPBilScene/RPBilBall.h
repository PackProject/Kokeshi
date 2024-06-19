#ifndef RP_PARTY_BIL_BALL_H
#define RP_PARTY_BIL_BALL_H
#include "RPTypes.h"

#include <RPUtility/RPUtlBaseFsm.h>

class RPBilBall {
public:
    enum EState { EState_Null, EState_Wait, EState_Pocket, EState_OffTable };

    bool IsCueBall() const {
        return mId == 0;
    }

    bool IsState(EState state) const {
        return mStateMachine->IsState(state);
    }

    void ChangeState(EState state) {
        mStateMachine->ChangeState(state);
    }

private:
    // RPPartyGameObjBase -> IRPSysHostIOSocket, etc.
    char _00[0x60];

    u32 mId;                                // at 0x60
    RPUtlBaseFsm<RPBilBall>* mStateMachine; // at 0x64
};

#endif
