#ifndef RP_PARTY_BIL_CTRL_MANAGER_H
#define RP_PARTY_BIL_CTRL_MANAGER_H
#include "types_RP.h"

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
public:
    static RPBilCtrlManager* GetInstance() {
        return sInstance;
    }

    RPBilCtrl* GetCtrl() const {
        return PTR_0x4;
    }

private:
    char _00[0x4];
    RPBilCtrl* PTR_0x4;

    static RPBilCtrlManager* sInstance;
};

#endif
