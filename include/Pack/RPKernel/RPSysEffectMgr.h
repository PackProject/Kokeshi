#ifndef RP_KERNEL_EFFECT_MGR_H
#define RP_KERNEL_EFFECT_MGR_H
#include "types.h"

class RPSysEffectMgr {
public:
    static RPSysEffectMgr* GetInstance() {
        return sInstance;
    }

    void LoadResource();

private:
    static RPSysEffectMgr* sInstance;
};

#endif
