#include <cstdio>
#include <libkiwi.h>

namespace kiwi {

K_DYNAMIC_SINGLETON_IMPL(Nw4rConsole);

namespace {

/**
 * Determines character code width from the lead byte
 */
u32 CodeWidth(const char* s) {
    return *s >= 0x81 ? 2 : 1;
}

} // namespace

/**
 * Constructor
 */
Nw4rConsole::Nw4rConsole() {
    // Initialize debug print
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
 * Destructor
 */
Nw4rConsole::~Nw4rConsole() {
    delete[] mpTextBuffer;
    mpTextBuffer = NULL;
}

/**
 * Print text to console
 *
 * @param fmt Format string
 * @param ... Format args
 */
void Nw4rConsole::Printf(const char* fmt, ...) {
    std::va_list list;
    va_start(list, fmt);
    VPrintf(fmt, list);
    va_end(list);
}

/**
 * Print text to console
 *
 * @param fmt Format string
 * @param args Format args
 */
void Nw4rConsole::VPrintf(const char* fmt, std::va_list args) {
    char msgbuf[0x400];
    std::vsnprintf(msgbuf, sizeof(msgbuf), fmt, args);
    PrintToBuffer(msgbuf);
}

/**
 * Draws console using DirectPrint (wrapper function)
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
 * Scrolls console display horizontally
 *
 * @param n Number of characters to scroll
 */
void Nw4rConsole::ScrollX(int n) {
    mViewX = Clamp<s16>(mViewX + (n * scScrollSpeedX), scViewXMin, scViewXMax);
}

/**
 * Scrolls console display vertically
 *
 * @param n Number of characters to scroll
 */
void Nw4rConsole::ScrollY(int n) {
    mViewTopLine = Clamp<int>(mViewTopLine + (n * scScrollSpeedY), mRingNumLine,
                              GetTotalLines());
}

/**
 * Gets position of specified character in the console text buffer
 *
 * @param line Character line
 * @param pos Character position
 * @return char* Character location in buffer
 */
char* Nw4rConsole::GetTextPtr(u16 line, u16 pos) const {
    return &mpTextBuffer[((mWidth + 1) * line) + pos];
}

/**
 * Advances text buffer to the next line
 *
 * @return char* Pointer to next line
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
 * Writes tab to text buffer
 *
 * @param dst Destination buffer
 * @return char* Pointer to text after inserted characters
 */
char* Nw4rConsole::PutTab(char* dst) {
    do {
        *dst++ = ' ';
        mPrintX++;
    } while (mPrintX < mWidth &&
             (mPrintX & (Nw4rDirectPrint::scTabSize - 1)) != 0);

    return dst;
}

/**
 * Writes character to buffer (multi-byte supported)
 *
 * @param str Character to write
 * @param dst Destination buffer
 * @return u32 Number of bytes written
 */
u32 Nw4rConsole::PutChar(const char* str, char* dst) {
    const u32 width = CodeWidth(str);

    // Character would overflow line
    if (mPrintX + width >= mWidth) {
        return 0;
    }

    mPrintX += width;

    for (int i = 0; i < width; i++) {
        *dst++ = *str++;
    }

    return width;
}

/**
 * Terminates the current text buffer line
 */
void Nw4rConsole::TerminateLine() const {
    *GetTextPtr(mPrintTop, mPrintX) = '\0';
}

/**
 * Gets total number of lines in the console text buffer
 */
int Nw4rConsole::GetTotalLines() const {
    AutoInterruptLock lock;
    return GetActiveLines() + mRingNumLine;
}

/**
 * Gets total number of lines printed to the console that are visible
 * on screen
 */
int Nw4rConsole::GetActiveLines() const {
    int active = GetRingUsedLines();

    if (mPrintX != 0) {
        active++;
    }

    return active;
}

/**
 * Gets total number of lines printed to the console that are part of the
 * ring buffer
 */
int Nw4rConsole::GetRingUsedLines() const {
    int used = mPrintTop - mRingTopLine;

    if (used < 0) {
        used += mHeight;
    }

    return used;
}

/**
 * Print string to console text buffer
 *
 * @param str Text string
 */
void Nw4rConsole::PrintToBuffer(const char* str) {
    AutoInterruptLock lock;

    // Write to debugger console
    OSReport(str);

    // Pointer to current buffer position
    char* dst = GetTextPtr(mPrintTop, mPrintX);

    while (*str != '\0') {
        // Console overflow
        if (mPrintTop == mHeight) {
            return;
        }

        while (*str != '\0') {
            bool newline = false;

            if (*str == '\n') {
                str++;
                dst = NextLine();
                break;
            } else if (*str == '\t') {
                str++;
                dst = PutTab(dst);
            } else {
                const u32 bytes = PutChar(str, dst);

                if (bytes != 0) {
                    str += bytes;
                    dst += bytes;
                } else {
                    newline = true;
                }
            }

            if (mPrintX >= mWidth) {
                newline = true;
            }

            if (newline) {
                if (*str == '\n') {
                    str++;
                }

                dst = NextLine();
                break;
            }

            if (*str == '\0') {
                TerminateLine();
            }
        }
    }
}

/**
 * Draw console using DirectPrint (implementation)
 */
void Nw4rConsole::DrawDirectImpl() const {
    int viewOffset = mViewTopLine - mRingNumLine;
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
