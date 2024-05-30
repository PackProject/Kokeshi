#ifndef LIBKIWI_DEBUG_NW4R_CONSOLE_H
#define LIBKIWI_DEBUG_NW4R_CONSOLE_H
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>

namespace kiwi {

/**
 * Reimplementation of NW4R's debug console
 */
class Nw4rConsole : public DynamicSingleton<Nw4rConsole> {
    friend class DynamicSingleton<Nw4rConsole>;

public:
    /**
     * Toggles console visibilty
     */
    void SetVisible(bool vis) {
        mIsVisible = vis;
    }

    /**
     * Scrolls display one character up
     */
    void ScrollUp() {
        ScrollY(-1);
    }

    /**
     * Scrolls display one character down
     */
    void ScrollDown() {
        ScrollY(+1);
    }

    /**
     * Scrolls display one character left
     */
    void ScrollLeft() {
        ScrollX(+1);
    }

    /**
     * Scrolls display one character right
     */
    void ScrollRight() {
        ScrollX(-1);
    }

    void Printf(const char* fmt, ...);
    void VPrintf(const char* fmt, std::va_list args);
    void DrawDirect() const;

private:
    Nw4rConsole();
    ~Nw4rConsole();

    void ScrollX(s32 n);
    void ScrollY(s32 n);
    char* GetTextPtr(u16 line, u16 pos) const;
    char* NextLine();
    char* PutTab(char* dst);
    u32 PutChar(const char* str, char* dst);
    void TerminateLine() const;
    s32 GetTotalLines() const;
    s32 GetActiveLines() const;
    s32 GetRingUsedLines() const;
    void PrintToBuffer(const char* str);
    void DrawDirectImpl() const;

private:
    char* mpTextBuffer; // Console text buffer

    u16 mWidth;  // Console width (in characters)
    u16 mHeight; // Console height (in characters)

    u16 mPrintTop; // Line where printing begins
    u16 mPrintX;   // X offset for printing

    u16 mRingTopLine; // Line where ring buffer begins
    s32 mRingNumLine; // Ring buffer capacity

    s32 mViewTopLine; // Line where console view begins
    s16 mViewX;       // Console view base X position
    s16 mViewY;       // Console view base Y position
    s16 mViewNumLine; // Maximum lines visible

    bool mIsVisible; // Whether the console is visible

    // Default console width (in characters)
    static const u32 scWidthDefault = 128;
    // Default console height (in characters)
    static const u32 scHeightDefault = 100;

    // Default console view size
    static const u32 scViewNumLinesDefault = 18;
    // Default console X origin
    static const u32 scViewXDefault = 0;
    // Default console Y origin
    static const u32 scViewYDefault = 30;

    // Display left scroll bound
    static const s32 scViewXMin = -150;
    // Display right scroll bound
    static const s32 scViewXMax = 10;

    // Display horizontal scroll speed
    static const u32 scScrollSpeedX = 6;
    // Display vertical scroll speed
    static const u32 scScrollSpeedY = 1;
};

} // namespace kiwi

#endif
