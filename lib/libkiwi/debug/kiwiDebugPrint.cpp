#include <Pack/RPGraphics.h>
#include <Pack/RPKernel.h>
#include <libkiwi.h>

#if defined(PACK_SPORTS) || defined(PACK_PLAY)

namespace kiwi {

//! Text offset for shadow/outline
const f32 DebugPrint::scEffectOffset = 0.0035f;

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
void DebugPrint::Printf(f32 x, f32 y, f32 scale, bool center, Color color,
                        const char* pMsg, ...) {
    if (!RPGrpRenderer::IsDrawLayout()) {
        return;
    }

    std::va_list list;
    va_start(list, pMsg);
    String msg = VFormat(pMsg, list);
    va_end(list);

    PrintImpl(x, y, scale, center, color, msg);
}

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
void DebugPrint::PrintfShadow(f32 x, f32 y, f32 scale, bool center, Color text,
                              Color shadow, const char* pMsg, ...) {
    if (!RPGrpRenderer::IsDrawLayout()) {
        return;
    }

    std::va_list list;
    va_start(list, pMsg);
    String msg = VFormat(pMsg, list);
    va_end(list);

    // Shadow
    PrintImpl(x + scEffectOffset, y + scEffectOffset, scale, center, shadow,
              msg);
    // Text
    PrintImpl(x, y, scale, center, text, msg);
}

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
void DebugPrint::PrintfOutline(f32 x, f32 y, f32 scale, bool center, Color text,
                               Color outline, const char* pMsg, ...) {
    if (!RPGrpRenderer::IsDrawLayout()) {
        return;
    }

    std::va_list list;
    va_start(list, pMsg);
    String msg = VFormat(pMsg, list);
    va_end(list);

    // Outline
    PrintImpl(x - scEffectOffset, y, scale, center, outline, msg);
    PrintImpl(x + scEffectOffset, y, scale, center, outline, msg);
    PrintImpl(x, y - scEffectOffset, scale, center, outline, msg);
    PrintImpl(x, y + scEffectOffset, scale, center, outline, msg);
    // Text
    PrintImpl(x, y, scale, center, text, msg);
}

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
void DebugPrint::PrintImpl(f32 x, f32 y, f32 scale, bool center, Color color,
                           const String& rMsg) {
    RP_GET_INSTANCE(RPSysTextWriter)->Begin();
    {
        u32 flag = 0;
        u32 oldFlag = RP_GET_INSTANCE(RPSysTextWriter)->GetDrawFlag();

        // Text alignment
        if (center) {
            flag |= RPSysTextWriter::DrawFlag_AlignCenterX;
            flag |= RPSysTextWriter::DrawFlag_AlignCenterY;
        }

        RP_GET_INSTANCE(RPSysTextWriter)->SetDrawFlag(flag);
        RP_GET_INSTANCE(RPSysTextWriter)->SetTextColor(color);
        RP_GET_INSTANCE(RPSysTextWriter)->SetScale(scale, scale);
        RP_GET_INSTANCE(RPSysTextWriter)->PrintfZeroCenter(x, y, rMsg);
        RP_GET_INSTANCE(RPSysTextWriter)->SetDrawFlag(oldFlag);
    }
    RP_GET_INSTANCE(RPSysTextWriter)->End();
}

} // namespace kiwi

#endif
