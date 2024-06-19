#include <libkiwi.h>

namespace kiwi {

const Color Color::RED(255, 0, 0, 255);
const Color Color::ORANGE(255, 128, 0, 255);
const Color Color::YELLOW(255, 255, 0, 255);
const Color Color::GREEN(0, 255, 0, 255);
const Color Color::BLUE(0, 0, 255, 255);
const Color Color::PURPLE(128, 0, 255, 255);
const Color Color::PINK(255, 0, 255, 255);
const Color Color::CYAN(0, 255, 255, 255);
const Color Color::BLACK(0, 0, 0, 255);
const Color Color::WHITE(255, 255, 255, 255);
const Color Color::BROWN(100, 50, 0, 255);
const Color Color::GREY(128, 128, 128, 255);

/**
 * @brief Converts RGB color to YUV format
 */
Color Color::Yuv() const {
    u8 y = 0.257f * r + 0.504f * g + 0.098f * b + 16.0f;
    u8 u = -0.148f * r - 0.291f * g + 0.439f * b + 128.0f;
    u8 v = 0.439f * r - 0.368f * g - 0.071f * b + 128.0f;
    return Color(y, u, v, a);
}

} // namespace kiwi
