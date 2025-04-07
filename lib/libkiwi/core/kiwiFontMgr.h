#ifndef LIBKIWI_CORE_FONT_MGR_H
#define LIBKIWI_CORE_FONT_MGR_H
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiStaticSingleton.h>

#include <nw4r/ut.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief Resource font ID
 */
enum EResFont {
#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    //! New Rodin (Demibold, 32pt, I4 tex format)
    EResFont_nRodDb_32_I4,
    //! New Rodin (Demibold, 32pt, IA4 tex format)
    EResFont_nRodDb_32_IA4,
    //! New Rodin (Demibold, 62pt, IA4 tex format)
    EResFont_nRodDb_62_IA4,
    //! New Rodin (Demibold, 200pt, IA4 tex format)
    EResFont_nRodDb_200_IA4,

    //! New Rodin (Extrabold, 32pt, I4 tex format)
    EResFont_nRodEb_32_I4,
    //! New Rodin (Extrabold, 70pt, IA4 tex format)
    EResFont_nRodEb_70_IA4,

#elif defined(PACK_RESORT)
    //! New Rodin (Bold, 32pt, IA4 tex format)
    EResFont_nRodB_32_IA4,

    //! New Rodin (Extrabold, 60pt, IA4 tex format)
    EResFont_nRodEb_60_IA4,
#endif

    EResFont_Max,
};

/**
 * @brief Font manager
 */
class FontMgr : public StaticSingleton<FontMgr> {
    friend class StaticSingleton<FontMgr>;

public:
    /**
     * @brief Gets the ROM font
     */
    const nw4r::ut::Font* GetRomFont() const {
        return mpRomFont;
    }

    /**
     * @brief Gets the specified resource font by ID
     *
     * @param font Font ID
     */
    const nw4r::ut::ResFont* GetResFont(EResFont font) {
        K_ASSERT(font < EResFont_Max);
        return mpResFonts[font];
    }

    /**
     * @brief Gets the specified resource font by name
     *
     * @param rName Font name
     */
    const nw4r::ut::ResFont* GetResFont(const String& rName);

private:
    /**
     * @brief Constructor
     */
    FontMgr();

private:
    nw4r::ut::RomFont* mpRomFont;                //!< ROM font
    nw4r::ut::ResFont* mpResFonts[EResFont_Max]; //!< Resource fonts

    //! Resource font name table
    static const char* scFontNames[EResFont_Max];
};

//! @}
} // namespace kiwi

#endif
