#include <Pack/RPGraphics.h>

#include <libkiwi.h>

#include <nw4r/math.h>

#include <revolution/GX.h>

namespace kiwi {

/**
 * @brief Constructor
 */
TextWriter::TextWriter() : mIsRendering(false), mOldDrawFlags(0) {

#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    bool success = SetResFont(EResFont_nRodDb_32_IA4);
#elif defined(PACK_RESORT)
    bool success = SetResFont(EResFont_nRodB_32_IA4);
#endif

    K_ASSERT_EX(success, "Could not prepare resource font");
}

/**
 * @brief Sets the specified resource font as the text font
 * @details The provided font must be one available in the font manager
 *
 * @param font Resource font ID
 * @return Success
 */
bool TextWriter::SetResFont(EResFont font) {
    K_ASSERT(font < EResFont_Max);

    const nw4r::ut::Font* pFont = FontMgr::GetInstance().GetResFont(font);
    K_ASSERT(pFont != nullptr);

    return SetFont(*pFont);
}

/**
 * @brief Sets the specified resource font as the text font
 * @details The provided font must be one available in the font manager
 *
 * @param rName Resource font name
 * @return Success
 */
bool TextWriter::SetResFont(const String& rName) {
    const nw4r::ut::Font* pFont = FontMgr::GetInstance().GetResFont(rName);
    K_ASSERT(pFont != nullptr);

    return SetFont(*pFont);
}

/**
 * @brief Sets the text font
 *
 * @param rFont New font
 * @return Success
 */
bool TextWriter::SetFont(const nw4r::ut::Font& rFont) {
    K_ASSERT_EX(rFont.GetEncoding() == nw4r::ut::FONT_ENCODING_UTF16,
                "Only UTF-16 fonts are supported");

    // Font is already loaded
    if (&rFont == GetFont()) {
        return true;
    }

    if (rFont.GetEncoding() != nw4r::ut::FONT_ENCODING_UTF16) {
        return false;
    }

    ClearFont();
    nw4r::ut::WideTextWriter::SetFont(rFont);
    return true;
}

/**
 * @brief Sets up GX rendering state for printing
 */
void TextWriter::Begin() {
    SetupGX();
    mIsRendering = true;
    mOldDrawFlags = GetDrawFlag();
}

/**
 * @brief Tears down GX rendering state
 */
void TextWriter::End() {
    mIsRendering = false;
    SetDrawFlag(mOldDrawFlags);
}

/**
 * @brief Prints formatted text to the screen
 * @details XY coordinates are normalized so they appear the same across
 * aspect ratios.
 *
 * @param x X position [0.0 - 1.0]
 * @param y Y position [0.0 - 1.0]
 * @param rStr Text string (UTF-16)
 */
template <typename T>
void TextWriter::Print(f32 x, f32 y, const StringImpl<T>& rStr) {
    K_ASSERT_EX(mIsRendering, "Please call TextWriter::Begin before printing");

    K_ASSERT_EX(0.0f <= x && x <= 1.0f,
                "X position is out of range [0, 1] (%.2f)", x);
    K_ASSERT_EX(0.0f <= y && y <= 1.0f,
                "Y position is out of range [0, 1] (%.2f)", y);

    // Don't clobber unrelated renderer passes
    if (!RPGrpRenderer::IsLytDraw()) {
        return;
    }

    // Convert to screen pixels
    x *= RPGrpScreen::GetSizeXMax();
    y *= RPGrpScreen::GetSizeYMax();

    // Correct for centered canvas mode
    if (RPGrpRenderer::GetActiveScreen()->GetCanvasMode() ==
        RPGrpScreen::CANVASMODE_CC) {

        // Convert to top-left origin
        x -= RPGrpScreen::GetSizeXMax() / 2.0f;
        y -= RPGrpScreen::GetSizeYMax() / 2.0f;
    }

    SetCursor(x, y);
    nw4r::ut::WideTextWriter::Print(rStr.ToWideChar(), rStr.Length());
}

/**
 * @brief Release font memory and resources
 */
void TextWriter::ClearFont() {
    const nw4r::ut::Font* pFont = GetFont();
    if (pFont == nullptr) {
        return;
    }

    // ROM font is owned by the font manager
    if (pFont == FontMgr::GetInstance().GetRomFont()) {
        return;
    }

    delete pFont;
}

/**
 * @brief Sets up GX rendering state
 */
void TextWriter::SetupGX() {
    nw4r::ut::WideTextWriter::SetupGX();

    GXSetZMode(FALSE, GX_ALWAYS, FALSE);

    nw4r::math::MTX34 posMtx;
    nw4r::math::MTX34Identity(&posMtx);

    // Correct for centered canvas mode
    if (RPGrpRenderer::GetActiveScreen()->GetCanvasMode() ==
        RPGrpScreen::CANVASMODE_CC) {

        posMtx._01 = -posMtx._01;
        posMtx._11 = -posMtx._11;
        posMtx._21 = -posMtx._21;
    }

    GXLoadPosMtxImm(posMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

// Instantiate function templates
template void TextWriter::Print<char>(f32 x, f32 y,
                                      const StringImpl<char>& rStr);
template void TextWriter::Print<wchar_t>(f32 x, f32 y,
                                         const StringImpl<wchar_t>& rStr);

} // namespace kiwi
