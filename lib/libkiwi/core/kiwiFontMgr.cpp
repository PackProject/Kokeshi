#include <Pack/RPSystem.h>

#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Resource font name table
 */
const char* FontMgr::scFontNames[EResFont_Max] = {
#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    "pac_nRodDb_32_I4.brfnt",  "pac_nRodDb_32_IA4.brfnt",
    "pac_nRodDb_62_IA4.brfnt", "pac_nRodDb_200_IA4.brfnt",
    "pac_nRodEb_32_I4.brfnt",  "pac_nRodEb_70_IA4.brfnt",
#elif defined(PACK_RESORT)
    "00_WS2_nRodB_32_IA4.brfnt", "01_WS2_nRodEb_60_IA4.brfnt"
#endif
};

/**
 * @brief Constructor
 */
FontMgr::FontMgr() : mpRomFont(nullptr), mpResFonts() {
    mpRomFont = RP_GET_INSTANCE(RPSysFontManager)->GetRomFont();
    K_ASSERT(mpRomFont != nullptr);

    for (int i = 0; i < EResFont_Max; i++) {
        mpResFonts[i] = RP_GET_INSTANCE(RPSysFontManager)->GetResFont(i);
        K_ASSERT(mpResFonts[i] != nullptr);
    }
}

/**
 * @brief Gets the specified resource font by name
 *
 * @param rName Font name
 */
const nw4r::ut::ResFont* FontMgr::GetResFont(const String& rName) {
    for (int i = 0; i < EResFont_Max; i++) {
        if (rName == scFontNames[i]) {
            return GetResFont(static_cast<EResFont>(i));
        }
    }

    K_ASSERT_EX(false, "Unknown resource font: %s", rName.CStr());
    return NULL;
}

} // namespace kiwi
