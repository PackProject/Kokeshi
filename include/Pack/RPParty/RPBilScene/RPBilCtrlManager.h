#ifndef RP_PARTY_BIL_CTRL_MANAGER_H
#define RP_PARTY_BIL_CTRL_MANAGER_H
#include "RPTypes.h"

struct RPBilCtrl {
    void TurnX(f32 amount);
    void TurnY(f32 amount);

    bool CanCtrl() const {
        return BOOL_0x1FC == false;
    }

    char _00[0x1FC];
    bool BOOL_0x1FC;
};

class RPBilCtrlManager {
    RP_SINGLETON_DECL(RPBilCtrlManager);

public:
    RPBilCtrl* GetCtrl() const {
        return PTR_0x4;
    }

private:
    char _00[0x4];
    RPBilCtrl* PTR_0x4;
};

#endif
