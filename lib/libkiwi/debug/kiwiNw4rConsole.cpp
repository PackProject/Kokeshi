#include <cstdio>
#include <libkiwi.h>

namespace kiwi {

K_DYNAMIC_SINGLETON_IMPL(Nw4rConsole);

namespace {

/**
 * @brief Determines character code width from the lead byte
 */
u32 CodeWidth(const char* pCode) {
    return *pCode >= 0x81 ? 2 : 1;
}

} // namespace

/**
 * @brief Constructor
 */
Nw4rConsole::Nw4rConsole() {
    Nw4rDirectPrint::CreateInstance();

    mWidth = scWidthDefault;
    mHeight = scHeightDefault;
    mpTextBuffer = new char[(mWidth + 1) * mHeight];

    mIsVisible = false;

    mPrintTop = 0;
    mPrintX = 0;

    mRingTopLine = 0;
    mRingNumLine = 0;

    mViewTopLine = 0;
    mViewX = scViewXDefault;
    mViewY = scViewYDefault;
    mViewNumLine = scViewNumLinesDefault;
}

/**
 * @brief Destructor
 */
Nw4rConsole::~Nw4rConsole() {
    delete[] mpTextBuffer;
}

/**
 * @brief Prints text to console
 *
 * @param pMsg Format string
 * @param ... Format args
 */
void Nw4rConsole::Printf(const char* pMsg, ...) {
    std::va_list list;
    va_start(list, pMsg);
    VPrintf(pMsg, list);
    va_end(list);
}

/**
 * @brief Prints text to console
 *
 * @param pMsg Format string
 * @param args Format args
 */
void Nw4rConsole::VPrintf(const char* pMsg, std::va_list args) {
    char msgbuf[1024];
    std::vsnprintf(msgbuf, sizeof(msgbuf), pMsg, args);
    PrintToBuffer(msgbuf);
}

/**
 * @brief Draws console using DirectPrint
 */
void Nw4rConsole::DrawDirect() const {
    // Not visible/not setup
    if (!Nw4rDirectPrint::GetInstance().IsActive() || !mIsVisible) {
        return;
    }

    // Clear console framebuffer
    Nw4rDirectPrint::GetInstance().EraseXfb(
        mViewX - Nw4rDirectPrint::scFontCharWidth, mViewY - 3,
        mWidth * Nw4rDirectPrint::scFontCharWidth + 12,
        mViewNumLine * Nw4rDirectPrint::scFontLeading + 4);

    // Draw console
    DrawDirectImpl();

    // Keep framebuffer updated
    Nw4rDirectPrint::GetInstance().StoreCache();
}

/**
 * @brief Scrolls console display horizontally
 *
 * @param n Number of characters to scroll
 */
void Nw4rConsole::ScrollX(s32 n) {
    mViewX = Clamp<s16>(mViewX + (n * scScrollSpeedX), scViewXMin, scViewXMax);
}

/**
 * @brief Scrolls console display vertically
 *
 * @param n Number of characters to scroll
 */
void Nw4rConsole::ScrollY(s32 n) {
    mViewTopLine = Clamp<s32>(mViewTopLine + (n * scScrollSpeedY), mRingNumLine,
                              GetTotalLines());
}

/**
 * @brief Gets position of specified character in the console text buffer
 *
 * @param line Character line
 * @param pos Character position
 * @return Character location in buffer
 */
char* Nw4rConsole::GetTextPtr(u16 line, u16 pos) const {
    return &mpTextBuffer[((mWidth + 1) * line) + pos];
}

/**
 * @brief Advances text buffer to the next line
 *
 * @return Pointer to next line
 */
char* Nw4rConsole::NextLine() {
    *GetTextPtr(mPrintTop, mPrintX) = '\0';

    mPrintX = 0;
    mPrintTop++;

    if (mPrintTop == mRingTopLine) {
        mRingNumLine--;
        mRingTopLine++;

        if (mRingTopLine == mHeight) {
            mRingTopLine = 0;
        }
    }

    return GetTextPtr(mPrintTop, 0);
}

/**
 * @brief Writes tab to text buffer
 *
 * @param pDst Destination buffer
 * @return Pointer to text after inserted characters
 */
char* Nw4rConsole::PutTab(char* pDst) {
    do {
        *pDst++ = ' ';
        mPrintX++;
    } while (mPrintX < mWidth &&
             (mPrintX & (Nw4rDirectPrint::scTabSize - 1)) != 0);

    return pDst;
}

/**
 * @brief Writes character to buffer (multi-byte supported)
 *
 * @param pCode Character to write
 * @param pDst Destination buffer
 * @return Number of bytes written
 */
u32 Nw4rConsole::PutChar(const char* pCode, char* pDst) {
    u32 width = CodeWidth(pCode);

    // Character would overflow line
    if (mPrintX + width >= mWidth) {
        return 0;
    }

    mPrintX += width;

    for (int i = 0; i < width; i++) {
        *pDst++ = *pCode++;
    }

    return width;
}

/**
 * @brief Terminates the current text buffer line
 */
void Nw4rConsole::TerminateLine() const {
    *GetTextPtr(mPrintTop, mPrintX) = '\0';
}

/**
 * @brief Gets total number of lines in the console text buffer
 */
s32 Nw4rConsole::GetTotalLines() const {
    AutoInterruptLock lock;
    return GetActiveLines() + mRingNumLine;
}

/**
 * @brief Gets total number of lines printed to the console that are visible
 * on screen
 */
s32 Nw4rConsole::GetActiveLines() const {
    s32 active = GetRingUsedLines();

    if (mPrintX != 0) {
        active++;
    }

    return active;
}

/**
 * @brief Gets total number of lines printed to the console that are part of the
 * ring buffer
 */
s32 Nw4rConsole::GetRingUsedLines() const {
    s32 used = mPrintTop - mRingTopLine;

    if (used < 0) {
        used += mHeight;
    }

    return used;
}

/**
 * @brief Prints string to console text buffer
 *
 * @param pStr Text string
 */
void Nw4rConsole::PrintToBuffer(const char* pStr) {
    AutoInterruptLock lock;

    // Write to debugger console
    OSReport(pStr);

    // Pointer to current buffer position
    char* pDst = GetTextPtr(mPrintTop, mPrintX);

    while (*pStr != '\0') {
        // Console overflow
        if (mPrintTop == mHeight) {
            return;
        }

        while (*pStr != '\0') {
            bool newline = false;

            if (*pStr == '\n') {
                pStr++;
                pDst = NextLine();
                break;
            } else if (*pStr == '\t') {
                pStr++;
                pDst = PutTab(pDst);
            } else {
                u32 bytes = PutChar(pStr, pDst);

                if (bytes != 0) {
                    pStr += bytes;
                    pDst += bytes;
                } else {
                    newline = true;
                }
            }

            if (mPrintX >= mWidth) {
                newline = true;
            }

            if (newline) {
                if (*pStr == '\n') {
                    pStr++;
                }

                pDst = NextLine();
                break;
            }

            if (*pStr == '\0') {
                TerminateLine();
            }
        }
    }
}

/**
 * @brief Draws console using DirectPrint (internal implementation)
 */
void Nw4rConsole::DrawDirectImpl() const {
    s32 viewOffset = mViewTopLine - mRingNumLine;
    if (viewOffset < 0) {
        viewOffset = 0;
    } else if (viewOffset > GetActiveLines()) {
        return;
    }

    u16 line = mRingTopLine + viewOffset;
    if (line >= mHeight) {
        line -= mHeight;
    }

    u16 topLine = mPrintTop + (mPrintX != 0 ? 1 : 0);
    if (topLine == mHeight) {
        topLine = 0;
    }

    u16 printLines = 0;
    while (line != topLine) {
        Nw4rDirectPrint::GetInstance().DrawString(
            mViewX, mViewY + printLines * Nw4rDirectPrint::scFontLeading,
            "%s\n", GetTextPtr(line, 0));

        printLines++;
        line++;

        // Don't overflow console
        if (line == mHeight || printLines >= mViewNumLine) {
            break;
        }
    }
}

} // namespace kiwi
