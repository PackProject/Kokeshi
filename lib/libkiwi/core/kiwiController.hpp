#ifndef LIBKIWI_CORE_CONTROLLER_H
#define LIBKIWI_CORE_CONTROLLER_H
#include <egg/core.h>
#include <libkiwi/util/kiwiNonCopyable.hpp>
#include <revolution/WPAD.h>
#include <types.h>

namespace kiwi {

/**
 * EGG controller wrapper
 */
class WiiCtrl : public EGG::CoreController, NonCopyable {
public:
    enum Button {
        Button_None = 0,

        Button_A = (1 << 11),
        Button_B = (1 << 10),

        Button_Up = (1 << 3),
        Button_Down = (1 << 2),
        Button_Left = (1 << 0),
        Button_Right = (1 << 1),

        Button_1 = (1 << 9),
        Button_2 = (1 << 8),

        Button_Minus = (1 << 12),
        Button_Plus = (1 << 4),

        Button_Home = (1 << 15),

        Button_DPad = Button_Up | Button_Down | Button_Left | Button_Right,

        Button_Any = Button_A | Button_B | Button_Up | Button_Down |
                     Button_Left | Button_Right | Button_1 | Button_2 |
                     Button_Minus | Button_Plus | Button_Home
    };

public:
    /**
     * Tests whether the controller is connected
     */
    bool Connected() const {
        return getReadLength() > 0;
    }

    /**
     * Gets buttons held during the most recent frame
     */
    u32 Hold() const {
        return getCoreStatus(0)->hold;
    }

    /**
     * Gets buttons released during the most recent frame
     */
    u32 Release() const {
        return getCoreStatus(0)->release;
    }

    /**
     * Gets buttons triggered during the most recent frame
     */
    u32 Trig() const {
        return getCoreStatus(0)->trig;
    }
};

/**
 * EGG controller manager wrapper
 */
class CtrlManager : protected EGG::CoreControllerMgr, NonCopyable {
public:
    static const int scMaxPlayers = WPAD_MAX_CONTROLLERS;

    static CtrlManager& GetInstance() {
        EGG::CoreControllerMgr* base = EGG::CoreControllerMgr::getInstance();
        K_ASSERT(base != NULL);
        return static_cast<CtrlManager&>(*base);
    }

    WiiCtrl& GetWiiCtrl(int i) {
        K_ASSERT(i < scMaxPlayers);

        EGG::CoreController* base = getNthController(i);
        K_ASSERT(base != NULL);

        return static_cast<WiiCtrl&>(*base);
    }
};

} // namespace kiwi

#endif
