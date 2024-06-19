#ifndef RP_GRAPHICS_SCREEN_H
#define RP_GRAPHICS_SCREEN_H
#include "RPTypes.h"

#include <egg/gfx/eggScreen.h>

class RPGrpScreen : public EGG::Screen {
public:
    RPGrpScreen() {
        SetFarZ(10000.0f);
    }

    virtual ~RPGrpScreen();

private:
};

#endif
