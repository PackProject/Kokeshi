#ifndef RP_KERNEL_EFFECT_MGR_H
#define RP_KERNEL_EFFECT_MGR_H
#include "types.h"

class RPSysEffectMgr {
    RP_SINGLETON_DECL(RPSysEffectMgr);

public:
    void LoadResource();
};

#endif
