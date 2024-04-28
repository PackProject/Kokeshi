#ifndef LIBKIWI_CORE_CONTROLLER_H
#define LIBKIWI_CORE_CONTROLLER_H
#include <egg/core.h>
#include <libkiwi/util/kiwiNonCopyable.h>
#include <revolution/WPAD.h>
#include <types.h>

namespace kiwi {

/**
 * @brief Player IDs
 */
enum EPlayer {
    EPlayer_1,
    EPlayer_2,
    EPlayer_3,
    EPlayer_4,

    EPlayer_Max
};

/**
 * EGG controller wrapper
 */
class WiiCtrl : public EGG::CoreController, private NonCopyable {
public:
    enum EButton {
        EButton_None = 0,

        EButton_A = (1 << 11),
        EButton_B = (1 << 10),

        EButton_Up = (1 << 3),
        EButton_Down = (1 << 2),
        EButton_Left = (1 << 0),
        EButton_Right = (1 << 1),

        EButton_1 = (1 << 9),
        EButton_2 = (1 << 8),

        EButton_Minus = (1 << 12),
        EButton_Plus = (1 << 4),

        EButton_Home = (1 << 15),

        EButton_DPad = EButton_Up | EButton_Down | EButton_Left | EButton_Right,

        EButton_Any = EButton_A | EButton_B | EButton_Up | EButton_Down |
                      EButton_Left | EButton_Right | EButton_1 | EButton_2 |
                      EButton_Minus | EButton_Plus | EButton_Home
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
        return Raw().hold;
    }

    /**
     * Gets buttons released during the most recent frame
     */
    u32 Release() const {
        return Raw().release;
    }

    /**
     * Gets buttons triggered during the most recent frame
     */
    u32 Trig() const {
        return Raw().trig;
    }

    /**
     * Gets KPAD data
     */
    KPADStatus& Raw() {
        return *reinterpret_cast<KPADStatus*>(getCoreStatus(0));
    }

    /**
     * Gets KPAD data
     */
    const KPADStatus& Raw() const {
        return *reinterpret_cast<KPADStatus*>(getCoreStatus(0));
    }
};

/**
 * EGG controller manager wrapper
 */
class CtrlMgr : protected EGG::CoreControllerMgr, private NonCopyable {
public:
    static CtrlMgr& GetInstance() {
        EGG::CoreControllerMgr* base = EGG::CoreControllerMgr::getInstance();
        K_ASSERT(base != NULL);
        return *static_cast<CtrlMgr*>(base);
    }

    WiiCtrl& GetWiiCtrl(EPlayer i) {
        K_ASSERT(i < EPlayer_Max);

        EGG::CoreController* base = getNthController(i);
        K_ASSERT(base != NULL);

        return *static_cast<WiiCtrl*>(base);
    }
};

} // namespace kiwi

#endif
