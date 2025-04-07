#ifndef LIBKIWI_DEBUG_TEXT_WRITER_H
#define LIBKIWI_DEBUG_TEXT_WRITER_H
#include <libkiwi/core/kiwiFontMgr.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>
#include <libkiwi/util/kiwiStaticSingleton.h>

#include <nw4r/ut.h>

namespace kiwi {
//! @addtogroup libkiwi_debug
//! @{

/**
 * @brief Screen text writer
 */
class TextWriter : public StaticSingleton<TextWriter>,
                   public nw4r::ut::WideTextWriter {

    friend class StaticSingleton<TextWriter>;

public:
    /**
     * @brief Sets the specified resource font as the text font
     * @details The provided font must be one available in the font manager
     *
     * @param font Resource font ID
     * @return Success
     */
    bool SetResFont(EResFont font);
    /**
     * @brief Sets the specified resource font as the text font
     * @details The provided font must be one available in the font manager
     *
     * @param rName Resource font name
     * @return Success
     */
    bool SetResFont(const String& rName);

    /**
     * @brief Sets the text font
     *
     * @param rFont New font
     * @return Success
     */
    bool SetFont(const nw4r::ut::Font& rFont);

    /**
     * @brief Sets up GX rendering state for printing
     */
    void Begin();
    /**
     * @brief Tears down GX rendering state
     */
    void End();

    /**
     * @brief Prints formatted text to the screen
     * @details XY coordinates are normalized so they appear the same across
     * aspect ratios.
     *
     * @param x X position [0.0 - 1.0]
     * @param y Y position [0.0 - 1.0]
     * @param rStr Text string
     */
    template <typename T> void Print(f32 x, f32 y, const StringImpl<T>& rStr);

private:
    /**
     * @brief Constructor
     */
    TextWriter();
    /**
     * @brief Destructor
     */
    virtual ~TextWriter() {
        ClearFont();
    }

    /**
     * @brief Release font memory and resources
     */
    void ClearFont();

    /**
     * @brief Sets up GX rendering state
     */
    void SetupGX();

private:
    bool mIsRendering; //!< Whether the render state is active
    u32 mOldDrawFlags; //!< Backup of previous draw flags
};

//! @}
} // namespace kiwi

#endif
