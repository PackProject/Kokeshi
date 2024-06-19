#ifndef RP_SYSTEM_PLAYER_MGR_H
#define RP_SYSTEM_PLAYER_MGR_H
#include "types.h"

class RPSysPlayerMgr {
    RP_SINGLETON_DECL(RPSysPlayerMgr);

public:
    void resetData();
    void setPlayerNum(int num);
    void setControllerNum(int num);

private:
    static RPSysPlayerMgr* sInstance;
};

#endif
