#ifndef RP_KERNEL_KOKESHI_MANAGER_H
#define RP_KERNEL_KOKESHI_MANAGER_H
#include "RPTypes.h"

class RPSysKokeshiManager {
public:
    enum GenType {

    };

public:
    static RPSysKokeshiManager* GetInstance() {
        return sInstance;
    }

    void LoadStaticResource();

private:
    static RPSysKokeshiManager* sInstance;
};

#endif
