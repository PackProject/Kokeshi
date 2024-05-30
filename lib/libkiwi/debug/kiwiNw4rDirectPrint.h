#ifndef LIBKIWI_DEBUG_NW4R_DIRECT_PRINT_H
#define LIBKIWI_DEBUG_NW4R_DIRECT_PRINT_H
#include <libkiwi/core/kiwiColor.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>

namespace kiwi {

/**
 * Reimplementation of NW4R's direct print system
 */
class Nw4rDirectPrint : public DynamicSingleton<Nw4rDirectPrint> {
    friend class DynamicSingleton<Nw4rDirectPrint>;

public:
    bool IsActive() const {
        return mpBuffer != NULL;
    }

    void SetupXfb();
    void ChangeXfb(void* buffer, u16 w, u16 h);
    void EraseXfb(s32 x, s32 y, s32 w, s32 h) const;
    void StoreCache() const;
    void DrawString(s32 x, s32 y, const char* fmt, ...) const;
    void SetColor(const Color rgb);

private:
    Nw4rDirectPrint();
    ~Nw4rDirectPrint();

    s32 GetDotWidth() const;
    s32 GetDotHeight() const;

    void DrawStringImpl(s32 x, s32 y, const char* str) const;
    const char* DrawStringLine(s32 x, s32 y, const char* str, s32 maxlen) const;
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

} // namespace kiwi

#endif
