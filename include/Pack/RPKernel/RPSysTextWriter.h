#ifndef RP_KERNEL_TEXT_WRITER_H
#define RP_KERNEL_TEXT_WRITER_H
#include "RPSysFontManager.h"
#include "RPTypes.h"
#include <nw4r/ut/ut_RomFont.h>
#include <nw4r/ut/ut_TextWriterBase.h>

/**
 * @brief RP engine screen text writer
 * @details Provides base functionality from NW4R, but with some extra features
 * to make printing easier, such as the ability to print to an arbitrary
 * screenspace location, or begin/end rendering.
 * @wfuname
 */
class RPSysTextWriter : public nw4r::ut::TextWriterBase<char> {
    RP_SINGLETON_DECL(RPSysTextWriter);

public:
    /**
     * @brief Reset all settings
     * @address 80190b9c
     */
    void Reset();

    /**
     * @brief Print formatted text to screen coordinates (relative to the
     * top-left)
     * @address 80190af8
     */
    void Printf(f32 x, f32 y, const char* msg, ...);
    /**
     * @brief Print formatted text to screen coordinates (relative to the
     * center)
     * @address 80190960
     */
    void PrintfZeroCenter(f32 x, f32 y, const char* msg, ...);

    /**
     * @brief Setup GX for rendering
     * @address 801908d8
     */
    void Begin();
    /**
     * @brief End rendering
     * @address 801908cc
     */
    void End();

private:
    // @brief Render status (Begin/End)
    BOOL mIsRendering; // at 0x68
};

#endif
