#ifndef RP_KERNEL_KOKESHI_MANAGER_H
#define RP_KERNEL_KOKESHI_MANAGER_H
#include "RPTypes.h"

class RPSysKokeshiManager {
    RP_SINGLETON_DECL(RPSysKokeshiManager);

public:
    enum GenType {

    };

public:
    void LoadStaticResource();

private:
    static RPSysKokeshiManager* sInstance;
};

#endif
