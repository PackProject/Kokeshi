#ifndef LIBKIWI_CORE_CONTROLLER_H
#define LIBKIWI_CORE_CONTROLLER_H
#include <egg/core.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiExtView.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

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
 * @brief Controller buttons (type agnostic)
 */
enum EButton {
    EButton_Up = (1 << 0),
    EButton_Down = (1 << 1),
    EButton_Left = (1 << 2),
    EButton_Right = (1 << 3),
    EButton_A = (1 << 4),
    EButton_B = (1 << 5),
    EButton_1 = (1 << 6),
    EButton_2 = (1 << 7),
    EButton_Minus = (1 << 8),
    EButton_Plus = (1 << 9),
    EButton_Home = (1 << 10),
};

/**
 * @brief Wii Remote controller
 * @extview{EGG::CoreController}
 */
class WiiCtrl : public ExtView<EGG::CoreController> {
public:
    /**
     * @brief Tests whether the controller is connected
     */
    bool IsConnected() const {
        return getReadLength() > 0;
    }

    /**
     * @brief Tests whether specific buttons were held down last frame
     *
     * @param buttons Button mask
     */
    bool IsHold(u32 buttons) const {
        u32 mask = ConvertMask(buttons);
        return (GetStatus().hold & mask) == mask;
    }
    /**
     * @brief Tests whether specific buttons were released last frame
     *
     * @param buttons Button mask
     */
    bool IsRelease(u32 buttons) const {
        u32 mask = ConvertMask(buttons);
        return (GetStatus().release & mask) == mask;
    }
    /**
     * @brief Tests whether specific buttons were triggered (pressed) last frame
     *
     * @param buttons Button mask
     */
    bool IsTrig(u32 buttons) const {
        u32 mask = ConvertMask(buttons);
        return (GetStatus().trig & mask) == mask;
    }

private:
    /**
     * @brief Converts generic (EButton) mask to button mask for KPAD
     *
     * @param mask Generic (EButton) mask
     * @return u32 KPAD button mask
     */
    static u32 ConvertMask(u32 mask);

    /**
     * @brief Gets KPAD data (read-only)
     */
    const KPADStatus& GetStatus() const {
        return *reinterpret_cast<KPADStatus*>(getCoreStatus(0));
    }
};

/**
 * @brief Controller manager
 * @extview{EGG::CoreControllerMgr}
 */
class CtrlMgr : public ExtView<EGG::CoreControllerMgr> {
public:
    K_EXTVIEW_GET_INSTANCE(CtrlMgr, EGG::CoreControllerMgr::getInstance);

    /**
     * @brief Gets Wii Remote controller by player index
     *
     * @param i Player index
     */
    const WiiCtrl& GetWiiCtrl(EPlayer i);
};

//! @}
} // namespace kiwi

#endif
