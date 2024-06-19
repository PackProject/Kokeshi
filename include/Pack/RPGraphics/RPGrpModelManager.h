#ifndef RP_GRAPHICS_MODEL_MANAGER_H
#define RP_GRAPHICS_MODEL_MANAGER_H
#include <RPTypes.h>

// Forward declarations
class RPGrpLightManager;
class RPGrpFogManager;

class RPGrpModelManager {
public:
    static RPGrpModelManager* GetInstance() {
        return sInstance;
    }

    void CreateModelScene(u32, u8, u32, RPGrpLightManager*, RPGrpFogManager*);

private:
    static RPGrpModelManager* sInstance;
};

#endif
