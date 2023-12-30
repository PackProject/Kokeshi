#ifndef LIBKIWI_UTIL_NW4R_DIRECT_PRINT_H
#define LIBKIWI_UTIL_NW4R_DIRECT_PRINT_H
#include <libkiwi/core/kiwiColor.hpp>
#include <libkiwi/util/kiwiDynamicSingleton.hpp>
#include <types.h>

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
    void EraseXfb(int x, int y, int w, int h) const;
    void StoreCache() const;
    void DrawString(int x, int y, const char* fmt, ...) const;
    void SetColor(const Color rgb);

private:
    Nw4rDirectPrint();
    ~Nw4rDirectPrint();

    int GetDotWidth() const;
    int GetDotHeight() const;

    void DrawStringImpl(int x, int y, const char* str) const;
    const char* DrawStringLine(int x, int y, const char* str, int maxlen) const;
    void DrawStringChar(int x, int y, int code) const;

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
    // Framebuffer in memory
    u8* mpBuffer;
    // Framebuffer size (in bytes)
    u32 mBufferSize;

    // Framebuffer width (in pixels)
    u16 mBufferWidth;
    // Framebuffer height (in pixels)
    u16 mBufferHeight;
    // Framebuffer rows
    u16 mBufferRows;

    // Framebuffer text color
    Color mBufferColor;

    // Default buffer width (in pixels)
    static const u16 scBufferWidthDefault = 640;
    // Default buffer height (in pixels)
    static const u16 scBufferHeightDefault = 480;

    // Font data pt. 1
    static const u32 scFontData[];
    // Font data pt. 2
    static const u32 scFontData2[];

    // Converts from ASCII index to font data index
    static const u8 scAscii2Font[128];
};

} // namespace kiwi

#endif
