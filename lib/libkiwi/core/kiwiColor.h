#ifndef LIBKIWI_CORE_COLOR_H
#define LIBKIWI_CORE_COLOR_H
#include <libkiwi/math/kiwiAlgorithm.h>
#include <nw4r/ut.h>
#include <types.h>

namespace kiwi {

/**
 * RGBA/YUV color
 */
class Color {
public:
    Color() {
        *this = WHITE;
    }
    Color(u8 r, u8 g, u8 b, u8 a) : r(r), g(g), b(b), a(a) {}
    Color(u32 color)
        : r(color >> 24 & 0xFF),
          g(color >> 16 & 0xFF),
          b(color >> 8 & 0xFF),
          a(color >> 0 & 0xFF) {}

    operator u32() const {
        return rgba32();
    }

    operator nw4r::ut::Color() const {
        return nw4r::ut::Color(rgba32());
    }

    /**
     * Convert color to u32 (RGBA order)
     */
    u32 rgba32() const {
        return r << 24 | g << 16 | b << 8 | a;
    }

    /**
     * Convert color to u32 (ARGB order)
     */
    u32 argb32() const {
        return a << 24 | r << 16 | g << 8 | b;
    }

    /**
     * Convert RGB color to YUV format
     */
    Color yuv() const {
        u8 y = (0.257f * r + 0.504f * g + 0.098f * b + 16.0f);
        u8 u = (-0.148f * r - 0.291f * g + 0.439f * b + 128.0f);
        u8 v = (0.439f * r - 0.368f * g - 0.071f * b + 128.0f);
        return Color(y, u, v, a);
    }

public:
    // Color components
    u8 r, g, b, a;

    static const Color RED;
    static const Color ORANGE;
    static const Color YELLOW;
    static const Color GREEN;
    static const Color BLUE;
    static const Color PURPLE;
    static const Color PINK;
    static const Color BLACK;
    static const Color WHITE;
    static const Color BROWN;
    static const Color GREY;
};

} // namespace kiwi

#endif
