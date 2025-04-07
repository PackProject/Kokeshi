#ifndef LIBKIWI_DEBUG_TEXT_BUILDER_H
#define LIBKIWI_DEBUG_TEXT_BUILDER_H
#include <libkiwi/core/kiwiColor.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>
#include <libkiwi/util/kiwiStaticSingleton.h>

#include <nw4r/ut.h>

namespace kiwi {
//! @addtogroup libkiwi_debug
//! @{

/**
 * @brief Text draw flag
 */
enum ETextFlag {
    //! (Default) Align text to the left of the screen
    ETextFlag_TextLeft = 0,

    //! Align text to the middle of the screen
    ETextFlag_TextCenter = nw4r::ut::WideTextWriter::DRAWFLAG_ALIGN_H_CENTER,

    //! Align text to the right of the screen
    ETextFlag_TextRight = nw4r::ut::WideTextWriter::DRAWFLAG_ALIGN_H_RIGHT,
};

/**
 * @brief Text stroke type
 */
enum ETextStroke {
    ETextStroke_None,    //!< No stroke
    ETextStroke_Outline, //!< Outlined text
    ETextStroke_Shadow,  //!< Drop shadow (bottom-left)

    ETextStroke_Max
};

namespace detail {
//! @addtogroup libkiwi_debug
//! @{

/**
 * @brief Screen text object
 * @details Configure display using method chaining
 * @note When the object is destroyed, the text is committed to the screen.
 *
 * @tparam T Character type (char/wchar_t)
 */
template <typename T> class TextImpl {
public:
    /**
     * @brief Constructor
     *
     * @param pFmt Format C-style string
     * @param args Format arguments
     */
    TextImpl(const T* pFmt, std::va_list args);
    /**
     * @brief Constructor
     *
     * @param rFmt Format string
     * @param args Format arguments
     */
    TextImpl(const StringImpl<T>& rFmt, std::va_list args);

    /**
     * @brief Destructor
     * @details Only at this point is the text committed
     */
    ~TextImpl() {
        Print();
    }

    /**
     * @brief Sets the text fill color
     *
     * @param color New fill color
     * @return Self-reference for method chaining
     */
    TextImpl& SetTextColor(Color color);

    /**
     * @brief Sets the text stroke color
     *
     * @param color New stroke color
     * @return Self-reference for method chaining
     */
    TextImpl& SetStrokeColor(Color color);
    /**
     * @brief Sets the text stroke type
     *
     * @param stroke New stroke type
     * @return Self-reference for method chaining
     */
    TextImpl& SetStrokeType(ETextStroke stroke);

    /**
     * @brief Sets the text stroke
     *
     * @param color New stroke color
     * @param stroke New stroke type
     * @return Self-reference for method chaining
     */
    TextImpl& SetStroke(Color color, ETextStroke stroke);

    /**
     * @brief Sets the text position
     * @details XY coordinates are normalized so they appear the same across
     * aspect ratios.
     *
     * @param x X position [0.0 - 1.0]
     * @param y Y position [0.0 - 1.0]
     * @return Self-reference for method chaining
     */
    TextImpl& SetPosition(f32 x, f32 y);
    /**
     * @brief Sets the text position
     * @details XY coordinates are normalized so they appear the same across
     * aspect ratios.
     *
     * @param rPos New position
     * @return Self-reference for method chaining
     */
    TextImpl& SetPosition(const nw4r::math::VEC2& rPos) {
        return SetPosition(rPos.x, rPos.y);
    }

    /**
     * @brief Sets the text scale
     *
     * @param sx X-scale
     * @param sy Y-scale
     * @return Self-reference for method chaining
     */
    TextImpl& SetScale(f32 sx, f32 sy);
    /**
     * @brief Sets the text scale
     *
     * @param scale New scale
     * @return Self-reference for method chaining
     */
    TextImpl& SetScale(f32 scale) {
        return SetScale(scale, scale);
    }
    /**
     * @brief Sets the text scale
     *
     * @param rScale New scale
     * @return Self-reference for method chaining
     */
    TextImpl& SetScale(const nw4r::math::VEC2& rScale) {
        return SetScale(rScale.x, rScale.y);
    }

    /**
     * @brief Sets the text draw flags
     *
     * @param flags New draw flags
     * @return Self-reference for method chaining
     */
    TextImpl& SetDrawFlags(u32 flags);

private:
    //! Base text scale
    static const f32 BASE_SCALE;

    //! Base stroke offset
    static const f32 BASE_STROKE_OFFSET;

private:
    /**
     * @brief Performs initialization common between constructors
     */
    void Init();

    /**
     * @brief Displays text content on the screen
     */
    void Print();

private:
    StringImpl<T> mTextBuffer; //!< Text content
    Color mTextColor;          //!< Fill color

    ETextStroke mStrokeType; //!< Stroke setting
    Color mStrokeColor;      //!< Stroke color

    nw4r::math::VEC2 mPosition; //!< Text position
    nw4r::math::VEC2 mScale;    //!< Text scale

    u32 mFlags; //!< Draw flags
};

//! @}
} // namespace detail

/**
 * @brief Creates a text object for printing to the screen
 * @details Configure display using method chaining
 * @note When the object is destroyed, the text is committed to the screen.
 *
 * @tparam T Character type
 * @param pFmt Format C-style string
 * @param ... Format arguments
 */
template <typename T> K_INLINE detail::TextImpl<T> Text(const T* pFmt, ...) {
    std::va_list args;
    va_start(args, pFmt);
    detail::TextImpl<T> obj(pFmt, args);
    va_end(args);

    return obj;
}

/**
 * @brief Creates a text object for printing to the screen
 * @details Configure display using method chaining
 * @note When the object is destroyed, the text is committed to the screen.
 *
 * @tparam T Character type
 * @param rFmt Format string
 * @param ... Format arguments
 */
template <typename T>
K_INLINE detail::TextImpl<T> Text(const StringImpl<T>& rFmt, ...) {
    std::va_list args;
    va_start(args, rFmt);
    detail::TextImpl<T> obj(rFmt, args);
    va_end(args);

    return obj;
}

/**
 * @brief Creates a text object for printing to the screen
 * @details Configure display using method chaining
 * @note When the object is destroyed, the text is committed to the screen.
 *
 * @tparam T Character type
 * @param pFmt Format C-style string
 * @param args Format arguments
 */
template <typename T>
K_INLINE detail::TextImpl<T> Text(const T* pFmt, std::va_list args) {
    return detail::TextImpl<T>(pFmt, args);
}

/**
 * @brief Creates a text object for printing to the screen
 * @details Configure display using method chaining
 * @note When the object is destroyed, the text is committed to the screen.
 *
 * @tparam T Character type
 * @param rFmt Format string
 * @param args Format arguments
 */
template <typename T>
K_INLINE detail::TextImpl<T> Text(const StringImpl<T>& rFmt,
                                  std::va_list args) {

    return detail::TextImpl<T>(rFmt, args);
}

//! @}
} // namespace kiwi

#endif
