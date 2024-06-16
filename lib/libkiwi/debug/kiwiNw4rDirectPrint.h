#ifndef LIBKIWI_DEBUG_NW4R_DIRECT_PRINT_H
#define LIBKIWI_DEBUG_NW4R_DIRECT_PRINT_H
#include <libkiwi/core/kiwiColor.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>

namespace kiwi {
//! @addtogroup libkiwi_debug
//! @{

/**
 * @brief Reimplementation of NW4R's direct print system
 */
class Nw4rDirectPrint : public DynamicSingleton<Nw4rDirectPrint> {
    friend class DynamicSingleton<Nw4rDirectPrint>;

public:
    /**
     * @brief Tests whether direct print is currently possible
     */
    bool IsActive() const {
        return mpBuffer != nullptr;
    }

    /**
     * @brief Sets up XFB for printing
     */
    void SetupXfb();

    /**
     * @brief Changes framebuffer information
     *
     * @param pXfb Framebuffer in memory
     * @param w Framebuffer width
     * @param h Framebuffer height
     */
    void ChangeXfb(void* pXfb, u16 w, u16 h);

    /**
     * @brief Erases framebuffer contents
     *
     * @param x X position
     * @param y Y position
     * @param w Width
     * @param h Height
     */
    void EraseXfb(s32 x, s32 y, s32 w, s32 h) const;

    /**
     * @brief Keeps main framebuffer copy updated via cache blocks
     */
    void StoreCache() const;

    /**
     * @brief Draws string to framebuffer
     *
     * @param x Text X position
     * @param y Text Y position
     * @param pMsg Format string
     * @param ... Format arguments
     */
    void DrawString(s32 x, s32 y, const char* pMsg, ...) const;

    /**
     * @brief Sets framebuffer color
     *
     * @param rgb Color (RGB)
     */
    void SetColor(Color rgb);

private:
    /**
     * @brief Constructor
     */
    Nw4rDirectPrint();

    /**
     * @brief Destructor
     */
    virtual ~Nw4rDirectPrint();

    /**
     * @brief Gets framebuffer dot/unit width (in pixels)
     */
    s32 GetDotWidth() const;

    /**
     * @brief Gets framebuffer dot/unit height (in pixels)
     */
    s32 GetDotHeight() const;

    /**
     * @brief Draws string to framebuffer (internal implementation)
     *
     * @param x Text X position
     * @param y Text Y position
     * @param pMsg Text string
     */
    void DrawStringImpl(s32 x, s32 y, const char* pMsg) const;

    /**
     * @brief Draws line of string to framebuffer
     *
     * @param x String X position
     * @param y String Y position
     * @param pMsg Text string
     * @param maxlen Max line width
     * @return String contents after what was drawn
     */
    const char* DrawStringLine(s32 x, s32 y, const char* pMsg,
                               s32 maxlen) const;

    /**
     * @brief Draws character to framebuffer
     *
     * @param x Character X position
     * @param y Character Y position
     * @param code Character code
     */
    void DrawStringChar(s32 x, s32 y, s32 code) const;

public:
    // Font character width (in dots/units)
    static const u32 scFontCharWidth = 6;
    // Font character height (in dots/units)
    static const u32 scFontCharHeight = 7;

    // Font leading (space between lines, in dots/units)
    static const u32 scFontLeading = 10;

    // Tab size (in spaces)
    static const u32 scTabSize = 4;

private:
    u8* mpBuffer;    // Framebuffer in memory
    u32 mBufferSize; // Framebuffer size (in bytes)

    u16 mBufferWidth;  // Framebuffer width (in pixels)
    u16 mBufferHeight; // Framebuffer height (in pixels)
    u16 mBufferRows;   // Framebuffer rows

    Color mBufferColor; // Framebuffer text color

    // Default buffer width (in pixels)
    static const u16 scBufferWidthDefault = 640;
    // Default buffer height (in pixels)
    static const u16 scBufferHeightDefault = 480;

    static const u32 scFontData[];  // Font data pt. 1
    static const u32 scFontData2[]; // Font data pt. 2

    // Converts from ASCII index to font data index
    static const u8 scAscii2Font[128];
};

//! @}
} // namespace kiwi

#endif
