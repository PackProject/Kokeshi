#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Converts generic (EButton) mask to button mask for KPAD
 *
 * @param mask Generic (EButton) mask
 * @return u32 KPAD button mask
 */
u32 WiiCtrl::ConvertMask(u32 mask) {
    static const u32 table[][2] = {
        // clang-format off
        {EButton_Up,    KPAD_BTN_DUP},
        {EButton_Down,  KPAD_BTN_DDOWN},
        {EButton_Left,  KPAD_BTN_DLEFT},
        {EButton_Right, KPAD_BTN_DRIGHT},
        {EButton_A,     KPAD_BTN_A},
        {EButton_B,     KPAD_BTN_B},
        {EButton_1,     KPAD_BTN_1},
        {EButton_2,     KPAD_BTN_2},
        {EButton_Minus, KPAD_BTN_MINUS},
        {EButton_Plus,  KPAD_BTN_PLUS},
        {EButton_Home,  KPAD_BTN_HOME},
        // clang-format on
    };

    u32 conv = 0;

    for (int i = 0; i < LENGTHOF(table); i++) {
        if (mask & table[i][0]) {
            conv |= table[i][1];
        }
    }

    return conv;
}

/**
 * @brief Gets Wii Remote controller by player index
 *
 * @param i Player index
 */
const WiiCtrl& CtrlMgr::GetWiiCtrl(EPlayer i) {
    K_ASSERT(i < EPlayer_Max);

    EGG::CoreController* pBase = getNthController(i);
    K_ASSERT(pBase != nullptr);

    return *reinterpret_cast<WiiCtrl*>(pBase);
}

} // namespace kiwi
