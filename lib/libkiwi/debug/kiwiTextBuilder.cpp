#include <Pack/RPGraphics.h>
#include <Pack/RPKernel.h>

#include <libkiwi.h>

#include <nw4r/math.h>

namespace kiwi {
namespace detail {

/**
 * @brief Base text scale
 */
template <typename T> const f32 TextImpl<T>::BASE_SCALE = 0.70f;

/**
 * @brief Base stroke offset
 */
template <typename T> const f32 TextImpl<T>::BASE_STROKE_OFFSET = 0.002f;

/**
 * @brief Constructor
 *
 * @param pFmt Format C-style string
 * @param args Format arguments
 */
template <typename T> TextImpl<T>::TextImpl(const T* pFmt, std::va_list args) {
    mTextBuffer = VFormat(pFmt, args);
    Init();
}

/**
 * @brief Constructor
 *
 * @param rFmt Format string
 * @param args Format arguments
 */
template <typename T>
TextImpl<T>::TextImpl(const StringImpl<T>& rFmt, std::va_list args) {
    mTextBuffer = VFormat(rFmt, args);
    Init();
}

/**
 * @brief Performs initialization common between constructors
 */
template <typename T> void TextImpl<T>::Init() {
    mTextColor = Color::WHITE;

    mStrokeType = ETextStroke_None;
    mStrokeColor = Color::BLACK;

    mPosition.x = mPosition.y = 0.0f;
    mScale.x = mScale.y = 1.0f;

    mFlags = 0;
}

/**
 * @brief Displays text content on the screen
 */
template <typename T> void TextImpl<T>::Print() {
    // Don't clobber unrelated renderer passes
    if (!RPGrpRenderer::IsLytDraw()) {
        return;
    }

    TextWriter::GetInstance().Begin();
    {
        // Common text parameters
        TextWriter::GetInstance().SetScale(mScale.x * BASE_SCALE,
                                           mScale.y * BASE_SCALE);
        TextWriter::GetInstance().SetDrawFlag(mFlags);

        // Stroke offset changes with text scale
        f32 strokeOffsetX = BASE_STROKE_OFFSET * mScale.x;
        f32 strokeOffsetY = BASE_STROKE_OFFSET * mScale.y;

        // Draw stroke underneath main text
        switch (mStrokeType) {
        case ETextStroke_None: {
            break;
        }

        case ETextStroke_Outline: {
            TextWriter::GetInstance().SetTextColor(mStrokeColor);

            // Top-left (-X, -Y)
            TextWriter::GetInstance().Print(mPosition.x - strokeOffsetX,
                                            mPosition.y - strokeOffsetY,
                                            mTextBuffer);

            // Bottom-left (-X, +Y)
            TextWriter::GetInstance().Print(mPosition.x - strokeOffsetX,
                                            mPosition.y + strokeOffsetY,
                                            mTextBuffer);

            // Top-right (+X, +Y)
            TextWriter::GetInstance().Print(mPosition.x + strokeOffsetX,
                                            mPosition.y + strokeOffsetY,
                                            mTextBuffer);

            // Bottom-right (+X, -Y)
            TextWriter::GetInstance().Print(mPosition.x + strokeOffsetX,
                                            mPosition.y - strokeOffsetY,
                                            mTextBuffer);
            break;
        }

        case ETextStroke_Shadow: {
            TextWriter::GetInstance().SetTextColor(mStrokeColor);

            // Bottom-left (-X, +Y)
            TextWriter::GetInstance().Print(mPosition.x - strokeOffsetX,
                                            mPosition.y + strokeOffsetY,
                                            mTextBuffer);
            break;
        }

        default: {
            K_ASSERT(false);
            break;
        }
        }

        TextWriter::GetInstance().SetTextColor(mTextColor);
        TextWriter::GetInstance().Print(mPosition.x, mPosition.y, mTextBuffer);
    }
    TextWriter::GetInstance().End();
}

/**
 * @brief Sets the text fill color
 *
 * @param color New fill color
 * @return Self-reference for method chaining
 */
template <typename T> TextImpl<T>& TextImpl<T>::SetTextColor(Color color) {
    mTextColor = color;
    return *this;
}

/**
 * @brief Sets the text stroke color
 *
 * @param color New stroke color
 * @return Self-reference for method chaining
 */
template <typename T> TextImpl<T>& TextImpl<T>::SetStrokeColor(Color color) {
    mStrokeColor = color;
    return *this;
}

/**
 * @brief Sets the text stroke type
 *
 * @param stroke New stroke type
 * @return Self-reference for method chaining
 */
template <typename T>
TextImpl<T>& TextImpl<T>::SetStrokeType(ETextStroke stroke) {
    K_ASSERT(stroke < ETextStroke_Max);

    mStrokeType = stroke;
    return *this;
}

/**
 * @brief Sets the text stroke
 *
 * @param color New stroke color
 * @param stroke New stroke type
 * @return Self-reference for method chaining
 */
template <typename T>
TextImpl<T>& TextImpl<T>::SetStroke(Color color, ETextStroke stroke) {
    SetStrokeColor(color);
    SetStrokeType(stroke);
    return *this;
}

/**
 * @brief Sets the text position
 * @details XY coordinates are normalized so they appear the same across
 * aspect ratios.
 *
 * @param x X position [0.0 - 1.0]
 * @param y Y position [0.0 - 1.0]
 * @return Self-reference for method chaining
 */
template <typename T> TextImpl<T>& TextImpl<T>::SetPosition(f32 x, f32 y) {
    K_ASSERT_EX(0.0f <= x && x <= 1.0f,
                "X position is out of range [0, 1] (%.2f)", x);
    K_ASSERT_EX(0.0f <= y && y <= 1.0f,
                "Y position is out of range [0, 1] (%.2f)", y);

    mPosition.x = x;
    mPosition.y = y;
    return *this;
}

/**
 * @brief Sets the text scale
 *
 * @param sx X-scale
 * @param sy Y-scale
 * @return Self-reference for method chaining
 */
template <typename T> TextImpl<T>& TextImpl<T>::SetScale(f32 sx, f32 sy) {
    K_ASSERT(sx > 0.0f && sy > 0.0f);

    mScale.x = sx;
    mScale.y = sy;
    return *this;
}

/**
 * @brief Sets the text draw flags
 *
 * @param flags New draw flags
 * @return Self-reference for method chaining
 */
template <typename T> TextImpl<T>& TextImpl<T>::SetDrawFlags(u32 flags) {
    mFlags = flags;
    return *this;
}

// Instantiate class templates
template class TextImpl<char>;
template class TextImpl<wchar_t>;

} // namespace detail
} // namespace kiwi
