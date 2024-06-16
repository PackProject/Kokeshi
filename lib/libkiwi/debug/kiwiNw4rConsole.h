#ifndef LIBKIWI_DEBUG_NW4R_CONSOLE_H
#define LIBKIWI_DEBUG_NW4R_CONSOLE_H
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>

namespace kiwi {
//! @addtogroup libkiwi_debug
//! @{

/**
 * @brief Reimplementation of NW4R's debug console
 */
class Nw4rConsole : public DynamicSingleton<Nw4rConsole> {
    friend class DynamicSingleton<Nw4rConsole>;

public:
    /**
     * @brief Toggles console visibilty
     */
    void SetVisible(bool vis) {
        mIsVisible = vis;
    }

    /**
     * @brief Scrolls display one character up
     */
    void ScrollUp() {
        ScrollY(-1);
    }
    /**
     * @brief Scrolls display one character down
     */
    void ScrollDown() {
        ScrollY(+1);
    }

    /**
     * @brief Scrolls display one character left
     */
    void ScrollLeft() {
        ScrollX(+1);
    }
    /**
     * @brief Scrolls display one character right
     */
    void ScrollRight() {
        ScrollX(-1);
    }

    /**
     * @brief Prints text to console
     *
     * @param pMsg Format string
     * @param ... Format args
     */
    void Printf(const char* pMsg, ...);

    /**
     * @brief Prints text to console
     *
     * @param pMsg Format string
     * @param args Format args
     */
    void VPrintf(const char* pMsg, std::va_list args);

    /**
     * @brief Draws console using DirectPrint
     */
    void DrawDirect() const;

private:
    /**
     * @brief Constructor
     */
    Nw4rConsole();

    /**
     * @brief Destructor
     */
    virtual ~Nw4rConsole();

    /**
     * @brief Scrolls console display horizontally
     *
     * @param n Number of characters to scroll
     */
    void ScrollX(s32 n);
    /**
     * @brief Scrolls console display vertically
     *
     * @param n Number of characters to scroll
     */
    void ScrollY(s32 n);

    /**
     * @brief Gets position of specified character in the console text buffer
     *
     * @param line Character line
     * @param pos Character position
     * @return Character location in buffer
     */
    char* GetTextPtr(u16 line, u16 pos) const;

    /**
     * @brief Advances text buffer to the next line
     *
     * @return Pointer to next line
     */
    char* NextLine();

    /**
     * @brief Writes tab to text buffer
     *
     * @param pDst Destination buffer
     * @return Pointer to text after inserted characters
     */
    char* PutTab(char* pDst);

    /**
     * @brief Writes character to buffer (multi-byte supported)
     *
     * @param pCode Character to write
     * @param pDst Destination buffer
     * @return Number of bytes written
     */
    u32 PutChar(const char* pCode, char* pDst);

    /**
     * @brief Terminates the current text buffer line
     */
    void TerminateLine() const;

    /**
     * @brief Gets total number of lines in the console text buffer
     */
    s32 GetTotalLines() const;

    /**
     * @brief Gets total number of lines printed to the console that are visible
     * on screen
     */
    s32 GetActiveLines() const;

    /**
     * @brief Gets total number of lines printed to the console that are part of
     * the ring buffer
     */
    s32 GetRingUsedLines() const;

    /**
     * @brief Prints string to console text buffer
     *
     * @param pStr Text string
     */
    void PrintToBuffer(const char* pStr);

    /**
     * @brief Draws console using DirectPrint (internal implementation)
     */
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

//! @}
} // namespace kiwi

#endif
