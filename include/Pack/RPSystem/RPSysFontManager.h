#ifndef RP_SYSTEM_FONT_MANAGER_H
#define RP_SYSTEM_FONT_MANAGER_H
#include "RPTypes.h"

#include <nw4r/ut/ut_ResFont.h>
#include <nw4r/ut/ut_RomFont.h>

/**
 * @brief RP engine font manager
 * @details Responsible for loading and setting up the rom font and any res
 * fonts needed (Whether specific fonts are used is determined at compile time)
 * @wfuname
 */
class RPSysFontManager {
    RP_SINGLETON_DECL(RPSysFontManager);

public:
    // @brief Pack Project res fonts
    enum EResFont {
        FONT_NRODDB_32_I4,
        FONT_NRODDB_32_IA4,
        FONT_NRODDB_62_IA4,
        FONT_NRODDB_200_IA4,
        FONT_NRODEB_32_I4,
        FONT_NRODEB_70_IA4,
        FONT_HEALTHTIME,

        FONT_MAX
    };

public:
    /**
     * @brief Set up rom font
     * @address 8018be10
     */
    void LoadRomFont();
    /**
     * @brief Set up res fonts
     * @address 8018bd64
     */
    void LoadResFonts(); // 8018bd64

    /**
     * @brief Retrieve ResFont data by name
     * @address 8018bcdc
     * @return Serialized res font
     */
    void* GetResFontData(const char* name) const;

    // @brief Access RomFont (for text writer)
    nw4r::ut::RomFont& GetRomFont() const {
        return *mRomFont;
    }

private:
    // @brief Deserialized rom font
    nw4r::ut::RomFont* mRomFont; // at 0x4
    // @brief Deserialized res fonts
    nw4r::ut::ResFont* mResFonts[FONT_MAX]; // at 0x8
    // @brief Serialized res fonts
    void* mResFontData[FONT_MAX]; // at 0x24

    /**
     * @brief Boolean(?) array that marks which fonts are used by the pack
     * @address 80382a50
     */
    static u32 sIsResFontUsed[FONT_MAX];
    /**
     * @brief Strings of font filenames
     * @address 803b9950
     */
    static const char* sPackResFonts[FONT_MAX];
};

#endif
