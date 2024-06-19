#ifndef LIBKIWI_CORE_COLOR_H
#define LIBKIWI_CORE_COLOR_H
#include <libkiwi/k_types.h>
#include <nw4r/ut.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief RGBA/YUV color
 */
class Color {
public:
    /**
     * @brief Constructor
     * @details The default color is white.
     */
    Color() {
        *this = WHITE;
    }

    /**
     * @brief Constructor
     *
     * @param r Red component
     * @param g Green component
     * @param b Blue component
     * @param a Alpha component
     */
    Color(u8 r, u8 g, u8 b, u8 a) : r(r), g(g), b(b), a(a) {}

    /**
     * @brief Constructor
     *
     * @param color RGBA32 color
     */
    Color(u32 color)
        : r(color >> 24 & 0xFF),
          g(color >> 16 & 0xFF),
          b(color >> 8 & 0xFF),
          a(color >> 0 & 0xFF) {}

    /**
     * @brief Converts RGB color to YUV format
     */
    Color Yuv() const;

    /**
     * @brief Converts color to u32 (RGBA order)
     */
    u32 Rgba32() const {
        return r << 24 | g << 16 | b << 8 | a;
    }
    /**
     * @brief Converts color to u32 (ARGB order)
     */
    u32 Argb32() const {
        return a << 24 | r << 16 | g << 8 | b;
    }

    // clang-format off
    operator             u32() const { return Rgba32(); }                  //!< Implicitly converts color to u32 (RGBA order)
    operator nw4r::ut::Color() const { return nw4r::ut::Color(Rgba32()); } //!< Implicitly converts color to NW4R color
    // clang-format on

public:
    u8 r; //!< Red component
    u8 g; //!< Green component
    u8 b; //!< Blue component
    u8 a; //!< Alpha component

    /**
     * @name Preset Colors
     */
    /**@{*/
    static const Color RED;
    static const Color ORANGE;
    static const Color YELLOW;
    static const Color GREEN;
    static const Color BLUE;
    static const Color PURPLE;
    static const Color PINK;
    static const Color CYAN;
    static const Color BLACK;
    static const Color WHITE;
    static const Color BROWN;
    static const Color GREY;
    /**@}*/
};

//! @}
} // namespace kiwi

#endif
