#ifndef EGG_GFX_I_DRAWGX_H
#define EGG_GFX_I_DRAWGX_H
#include "eggAssert.h"
#include "types_egg.h"

namespace EGG {
class IDrawGX {
public:
    static Screen& getScreen() {
#line 59
        EGG_ASSERT(spScreen);
        return *spScreen;
    }

    static void setScreen(Screen& screen) {
        spScreen = &screen;
    }

private:
    static Screen* spScreen;
};
} // namespace EGG

#endif
