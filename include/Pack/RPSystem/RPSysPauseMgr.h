#ifndef RP_SYSTEM_PAUSE_MGR_H
#define RP_SYSTEM_PAUSE_MGR_H
#include "types.h"

class RPSysPauseMgr {
    RP_SINGLETON_DECL(RPSysPauseMgr);

public:
    void LoadResource();
};

#endif
