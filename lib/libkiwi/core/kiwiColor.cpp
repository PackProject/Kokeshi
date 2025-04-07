#include <libkiwi.h>

namespace kiwi {

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
