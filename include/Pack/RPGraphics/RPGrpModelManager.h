#ifndef RP_GRAPHICS_MODEL_MANAGER_H
#define RP_GRAPHICS_MODEL_MANAGER_H
#include <RPTypes.h>

// Forward declarations
class RPGrpLightManager;
class RPGrpFogManager;

class RPGrpModelManager {
    RP_SINGLETON_DECL(RPGrpModelManager);

public:
    void CreateModelScene(u32, u8, u32, RPGrpLightManager*, RPGrpFogManager*);

private:
    static RPGrpModelManager* sInstance;
};

#endif
