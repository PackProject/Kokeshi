#ifndef RP_GRAPHICS_MODEL_H
#define RP_GRAPHICS_MODEL_H
#include "types_RP.h"

#include <egg/gfx/eggModelEx.h>

class RPGrpModel {
public:
    EGG::ModelEx* GetModelEx() const {
        return mModelEx;
    }

private:
    char _00[0x40];
    EGG::ModelEx* mModelEx; // at 0x40
};

#endif
