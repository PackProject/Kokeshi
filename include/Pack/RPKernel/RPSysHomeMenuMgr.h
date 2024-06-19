#ifndef RP_KERNEL_HOME_MENU_MGR_H
#define RP_KERNEL_HOME_MENU_MGR_H
#include "types.h"

class RPSysHomeMenuMgr {
    RP_SINGLETON_DECL(RPSysHomeMenuMgr);

public:
    void LoadResource();

private:
    static RPSysHomeMenuMgr* sInstance;
};

#endif
