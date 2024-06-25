#ifndef RP_KERNEL_TUTORIAL_WINDOW_MGR_H
#define RP_KERNEL_TUTORIAL_WINDOW_MGR_H
#include "types.h"

class RPSysTutorialWinMgr {
    RP_SINGLETON_DECL(RPSysTutorialWinMgr);

public:
    void LoadResource();
};

#endif
