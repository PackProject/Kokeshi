#ifndef RP_PARTY_BIL_MAIN_H
#define RP_PARTY_BIL_MAIN_H
#include "types_RP.h"

#include <RPUtility/RPUtlBaseFsm.h>

class RPBilMain {
public:
    static void CreateInstance();
    static void DestroyInstance();
    static RPBilMain* GetInstance() {
        return sInstance;
    }

private:
    RPBilMain();
    virtual ~RPBilMain();

private:
    RPUtlBaseFsm<RPBilMain>* mStateMachine; // at 0x4

    static RPBilMain* sInstance;
};

#endif
