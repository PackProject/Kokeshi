#ifndef LIBKIWI_DEBUG_DEBUG_PRINT_H
#define LIBKIWI_DEBUG_DEBUG_PRINT_H
#include <libkiwi/core/kiwiColor.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>

namespace kiwi {

/**
 * @brief Screen text writer shortcut
 */
class DebugPrint {
public:
    /**
     * @brief Prints formatted text to the screen
     *
     * @param x X position
     * @param y Y position
     * @param scale Text scale
     * @param center Center-align text
     * @param color Text color
     * @param fmt Format string
     * @param ... Format arguments
     */
    static void Printf(f32 x, f32 y, f32 scale, bool center, kiwi::Color color,
                       const char* fmt, ...);

    /**
     * @brief Prints formatted text to the screen with an outline
     *
     * @param x X position
     * @param y Y position
     * @param scale Text scale
     * @param center Center-align text
     * @param text Text color
     * @param outline Outline color
     * @param fmt Format string
     * @param ... Format arguments
     */
    static void PrintfShadow(f32 x, f32 y, f32 scale, bool center,
                             kiwi::Color text, kiwi::Color shadow,
                             const char* fmt, ...);

    /**
     * @brief Prints text to the screen
     *
     * @param x X position
     * @param y Y position
     * @param scale Text scale
     * @param center Center-align text
     * @param color Text color
     * @param msg Text message
     */
    static void PrintfOutline(f32 x, f32 y, f32 scale, bool center,
                              kiwi::Color text, kiwi::Color outline,
                              const char* fmt, ...);

private:
    static void PrintImpl(f32 x, f32 y, f32 scale, bool center,
                          kiwi::Color color, const kiwi::String& msg);
};

} // namespace kiwi

#endif
