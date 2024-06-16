#ifndef LIBKIWI_DEBUG_DEBUG_PRINT_H
#define LIBKIWI_DEBUG_DEBUG_PRINT_H
#include <libkiwi/core/kiwiColor.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>

namespace kiwi {
//! @addtogroup libkiwi_debug
//! @{

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
     * @param pMsg Format string
     * @param ... Format arguments
     */
    static void Printf(f32 x, f32 y, f32 scale, bool center, Color color,
                       const char* pMsg, ...);

    /**
     * @brief Prints formatted text to the screen with a shadow
     *
     * @param x X position
     * @param y Y position
     * @param scale Text scale
     * @param center Center-align text
     * @param text Text color
     * @param shadow Shadow color
     * @param pMsg Format string
     * @param ... Format arguments
     */
    static void PrintfShadow(f32 x, f32 y, f32 scale, bool center, Color text,
                             Color shadow, const char* pMsg, ...);

    /**
     * @brief Prints formatted text to the screen with an outline
     *
     * @param x X position
     * @param y Y position
     * @param scale Text scale
     * @param center Center-align text
     * @param text Text color
     * @param outline Outline color
     * @param pMsg Format string
     * @param ... Format arguments
     */
    static void PrintfOutline(f32 x, f32 y, f32 scale, bool center, Color text,
                              Color outline, const char* pMsg, ...);

private:
    /**
     * @brief Prints text to the screen (internal implementation)
     *
     * @param x X position
     * @param y Y position
     * @param scale Text scale
     * @param center Center-align text
     * @param color Text color
     * @param rMsg Text message
     */
    static void PrintImpl(f32 x, f32 y, f32 scale, bool center, Color color,
                          const String& rMsg);

private:
    // Text offset for shadow/outline
    static const f32 scEffectOffset;
};

//! @}
} // namespace kiwi

#endif
