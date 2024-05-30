#ifndef RP_KERNEL_HOME_MENU_MGR_H
#define RP_KERNEL_HOME_MENU_MGR_H
#include "types.h"

class RPSysHomeMenuMgr {
public:
    static RPSysHomeMenuMgr* GetInstance() {
        return sInstance;
    }

    void LoadResource();

private:
    static RPSysHomeMenuMgr* sInstance;
};

#endif
