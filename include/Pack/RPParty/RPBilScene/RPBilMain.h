#ifndef RP_PARTY_BIL_MAIN_H
#define RP_PARTY_BIL_MAIN_H
#include "RPTypes.h"

#include <RPUtility/RPUtlBaseFsm.h>

class RPBilMain {
    RP_SINGLETON_DECL(RPBilMain);

public:
    RP_UTL_FSM_STATE_DECL(AFTERSHOT);
    RP_UTL_FSM_STATE_DECL(FOUL);

    void Calculate();
    void Reset();

private:
    RPBilMain();
    virtual ~RPBilMain();

private:
    RPUtlBaseFsm<RPBilMain>* mpStateMachine; // at 0x4

    static RPBilMain* sInstance;
};

#endif
