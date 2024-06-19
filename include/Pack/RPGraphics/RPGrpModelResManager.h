#ifndef RP_GRAPHICS_MODEL_RES_MANAGER_H
#define RP_GRAPHICS_MODEL_RES_MANAGER_H
#include <RPTypes.h>

class RPGrpModelResManager {
    RP_SINGLETON_DECL(RPGrpModelResManager);

public:
    void CreateResourceList(u16);

private:
    static RPGrpModelResManager* sInstance;
};

#endif
