#ifndef RP_KERNEL_TUTORIAL_WINDOW_MGR_H
#define RP_KERNEL_TUTORIAL_WINDOW_MGR_H
#include "types.h"

class RPSysTutorialWinMgr {
public:
    static RPSysTutorialWinMgr* GetInstance() {
        return sInstance;
    }

    void LoadResource();

private:
    static RPSysTutorialWinMgr* sInstance;
};

#endif
