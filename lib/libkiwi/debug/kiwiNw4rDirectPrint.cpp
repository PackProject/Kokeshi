#include <cstdio>
#include <cstring>
#include <libkiwi.h>
#include <revolution/VI.h>

namespace kiwi {

K_DYNAMIC_SINGLETON_IMPL(Nw4rDirectPrint);

namespace {

/**
 * @brief Suspends execution until the next VI retrace
 */
void WaitVIRetrace() {
    AutoInterruptLock lock(true);

    volatile s32 count = VIGetRetraceCount();
    while (count == VIGetRetraceCount()) {
        ;
    }
}

/**
 * @brief Creates framebuffer
 *
 * @param pRmo GX render configuration
 * @return Framebuffer
 */
void* CreateFB(const GXRenderModeObj* pRmo) {
    // Calculate framebuffer size in bytes
    u32 size = ROUND_UP(pRmo->fbWidth, 16) * pRmo->xfbHeight * sizeof(u16);

    // Try using heap, but be careful to not throw a nested exception
    void* pXfb = nullptr;
    if (MemoryMgr::GetInstance().GetFreeSize(EMemory_MEM1) > size) {
        pXfb = new (32) u8[size];
    }

    // Force allocation from OS arena
    if (pXfb == nullptr) {
        K_LOG("Can't get framebuffer from heap\n");
        pXfb = static_cast<u8*>(OSGetArenaHi()) - size;
        OSSetArenaHi(pXfb);
    }

    VIConfigure(pRmo);
    VISetNextFrameBuffer(pXfb);
    return pXfb;
}

} // namespace

/**
 * @brief Constructor
 */
Nw4rDirectPrint::Nw4rDirectPrint() {
    SetColor(Color::WHITE);
    ChangeXfb(nullptr, scBufferWidthDefault, scBufferHeightDefault);
}

/**
 * @brief Destructor
 */
Nw4rDirectPrint::~Nw4rDirectPrint() {
    delete mpBuffer;
}

/**
 * @brief Sets up XFB for printing
 */
void Nw4rDirectPrint::SetupXfb() {
    const GXRenderModeObj* pRmo = nullptr;

    // Initialize direct print
    SetColor(Color::WHITE);

// TODO: WS2 framebuffer is 608x456. Why?
#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    // Try to repurpose current framebuffer
    void* pXfb = VIGetCurrentFrameBuffer();

    // Create new framebuffer if one doesn't exist
    if (pXfb == nullptr) {
        pRmo = LibGX::GetDefaultRenderMode();
        pXfb = CreateFB(pRmo);
    }
#elif defined(PACK_RESORT)
    pRmo = LibGX::GetDefaultRenderMode();
    void* pXfb = CreateFB(pRmo);
#endif

    VISetBlack(FALSE);
    VIFlush();
    WaitVIRetrace();

    if (pRmo != nullptr) {
        ChangeXfb(pXfb, pRmo->fbWidth, pRmo->xfbHeight);
    } else {
        ChangeXfb(pXfb, scBufferWidthDefault, scBufferHeightDefault);
    }
}

/**
 * @brief Changes framebuffer information
 *
 * @param pXfb Framebuffer in memory
 * @param w Framebuffer width
 * @param h Framebuffer height
 */
void Nw4rDirectPrint::ChangeXfb(void* pXfb, u16 w, u16 h) {
    mpBuffer = static_cast<u8*>(pXfb);
    mBufferWidth = w;
    mBufferHeight = h;

    mBufferRows = ROUND_UP(w, 16);
    mBufferSize = mBufferRows * h * sizeof(u16);
}

/**
 * @brief Erases framebuffer contents
 *
 * @param x X position
 * @param y Y position
 * @param w Width
 * @param h Height
 */
void Nw4rDirectPrint::EraseXfb(s32 x, s32 y, s32 w, s32 h) const {
    if (mpBuffer == nullptr) {
        return;
    }

    if (GetDotWidth() == 2) {
        x *= 2;
        w *= 2;
    }

    s32 x2 = Min<s32>(x + w, mBufferWidth);
    x = Max<s32>(x, 0);
    w = x2 - x;

    if (GetDotHeight() == 2) {
        y *= 2;
        h *= 2;
    }

    s32 y2 = Min<s32>(y + h, mBufferHeight);
    y = Max<s32>(y, 0);
    h = y2 - y;

    // Character location in framebuffer
    u16* pPixel = reinterpret_cast<u16*>(mpBuffer);
    pPixel += x;
    pPixel += y * mBufferRows;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            *pPixel++ = 0x1080;
        }

        pPixel += mBufferRows - w;
    }
}

/**
 * @brief Keeps main framebuffer copy updated via cache blocks
 */
void Nw4rDirectPrint::StoreCache() const {
    DCStoreRange(mpBuffer, mBufferSize);
}

/**
 * @brief Draws string to framebuffer
 *
 * @param x Text X position
 * @param y Text Y position
 * @param pMsg Format string
 * @param ... Format arguments
 */
void Nw4rDirectPrint::DrawString(s32 x, s32 y, const char* pMsg, ...) const {
    if (mpBuffer == nullptr) {
        return;
    }

    char msgbuf[1024];
    std::va_list list;

    va_start(list, pMsg);
    if (std::vsnprintf(msgbuf, sizeof(msgbuf), pMsg, list) <= 0) {
        return;
    }
    va_end(list);

    DrawStringImpl(x, y, msgbuf);
}

/**
 * @brief Sets framebuffer color
 *
 * @param rgb Color (RGB)
 */
void Nw4rDirectPrint::SetColor(Color rgb) {
    // Framebuffer uses YUV format, so we convert the color
    mBufferColor = rgb.Yuv();
}

/**
 * @brief Gets framebuffer dot/unit width (in pixels)
 */
s32 Nw4rDirectPrint::GetDotWidth() const {
    return mBufferWidth < 400 ? 1 : 2;
}

/**
 * @brief Gets framebuffer dot/unit height (in pixels)
 */
s32 Nw4rDirectPrint::GetDotHeight() const {
    return mBufferHeight < 300 ? 1 : 2;
}

/**
 * @brief Draws string to framebuffer (internal implementation)
 *
 * @param x Text X position
 * @param y Text Y position
 * @param pMsg Text string
 */
void Nw4rDirectPrint::DrawStringImpl(s32 x, s32 y, const char* pMsg) const {
    if (mpBuffer == nullptr) {
        return;
    }

    // Base X position
    s32 x1 = x;
    // Framebuffer width in units
    s32 fbWidth = mBufferWidth / GetDotWidth();

    while (*pMsg != '\0') {
        // Attempt to draw line (whatever is allowed by framebuffer width)
        s32 width = (fbWidth - x) / scFontCharWidth;
        pMsg = DrawStringLine(x, y, pMsg, width);
        y += scFontLeading;

        // Entire line was drawn
        if (*pMsg == '\n') {
            // New line, reset X position
            pMsg++;
            x = x1;
        }
        // Line was partially drawn (width overflow)
        else if (*pMsg != '\0') {
            // Attempt to find next line
            pMsg++;
            pMsg = std::strchr(pMsg, '\n');

            // No next line
            if (pMsg == nullptr) {
                return;
            }

            // New line, reset X position
            pMsg++;
            x = x1;
        }
    }
}

/**
 * @brief Draws line of string to framebuffer
 *
 * @param x String X position
 * @param y String Y position
 * @param pMsg Text string
 * @param maxlen Max line width
 * @return char* String contents after what was drawn
 */
const char* Nw4rDirectPrint::DrawStringLine(s32 x, s32 y, const char* pMsg,
                                            s32 maxlen) const {
    if (mpBuffer == nullptr || maxlen <= 0) {
        return nullptr;
    }

    s32 count = 0;

    while (*pMsg != '\0') {
        char c = *pMsg;

        // Line or string has ended, stop drawing
        if (c == '\n' || c == '\0') {
            return pMsg;
        }

        // Convert to font code
        s32 code = scAscii2Font[c & (LENGTHOF(scAscii2Font) - 1)];

        // Tab character
        if (code == 0xFD) {
            s32 tabSize = scTabSize - (count & (scTabSize - 1));
            x += tabSize * scFontCharWidth;
            count += tabSize;
        } else {
            // Non-tab character
            if (code != 0xFF) {
                DrawStringChar(x, y, code);
            }

            // 0xFF is treated as whitespace
            x += scFontCharWidth;
            count++;
        }

        // Stop at max line width
        if (count >= maxlen) {
            // Skip over newline if it comes next.
            // It wouldn't take up view space anyways so it does not affect the
            // maxlen.
            if (pMsg[1] == '\n') {
                pMsg++;
            }

            return pMsg;
        }

        pMsg++;
    }

    return pMsg;
}

/**
 * @brief Draws character to framebuffer
 *
 * @param x Character X position
 * @param y Character Y position
 * @param code Character code
 */
void Nw4rDirectPrint::DrawStringChar(s32 x, s32 y, s32 code) const {
    if (mpBuffer == nullptr) {
        return;
    }

    static const u32 scTwiceBit[] = {0b0000, 0b0011, 0b1100, 0b1111};

    // Convert to font-relative code
    s32 ncode = code >= 100 ? code - 100 : code;

    s32 fontW = ncode % 5 * scFontCharWidth;
    s32 fontH = ncode / 5 * scFontCharHeight;
    const u32* pFontLine =
        code < 100 ? &scFontData[fontH] : &scFontData2[fontH];

    s32 dotW = GetDotWidth();
    s32 dotH = GetDotHeight();

    // Character location in framebuffer
    u16* pPixel = reinterpret_cast<u16*>(mpBuffer);
    pPixel += x * dotW;
    pPixel += y * mBufferRows * dotH;

    if (x < 0 || y < 0 || mBufferWidth <= dotW * (x + scFontCharWidth) ||
        mBufferHeight <= dotH * (y + scFontCharHeight)) {
        return;
    }

    for (int countY = 0; countY < scFontCharHeight; countY++) {
        u32 fontBits = *pFontLine++ << fontW;

        // I couldn't tell you what this does...
        if (dotW == 1) {
            fontBits = (fontBits & 0xFC000000) >> 1;
        } else {
            fontBits = ((scTwiceBit[fontBits >> 30] << 8) |
                        (scTwiceBit[(fontBits >> 28) & 3] << 4) |
                        (scTwiceBit[(fontBits >> 26) & 3]))
                       << 19;
        }

        for (int countX = 0; countX < dotW * scFontCharWidth;
             countX += 2, fontBits <<= 2) {
            u16 color;

            color = (fontBits & 0x40000000) ? (mBufferColor.r << 8) : 0x00;
            color |= (((fontBits & 0x40000000) ? mBufferColor.g / 2 : 0x40) +
                      ((fontBits & 0x80000000) ? mBufferColor.g / 4 : 0x20) +
                      ((fontBits & 0x20000000) ? mBufferColor.g / 4 : 0x20));
            *pPixel = color;
            if (dotH > 1) {
                pPixel[mBufferRows] = color;
            }

            pPixel++;

            color = (fontBits & 0x20000000) ? (mBufferColor.r << 8) : 0x00;
            color |= (((fontBits & 0x20000000) ? mBufferColor.b / 2 : 0x40) +
                      ((fontBits & 0x10000000) ? mBufferColor.b / 4 : 0x20) +
                      ((fontBits & 0x40000000) ? mBufferColor.b / 4 : 0x20));
            *pPixel = color;
            if (dotH > 1) {
                pPixel[mBufferRows] = color;
            }

            pPixel++;
        }

        pPixel += (mBufferRows * dotH) - (dotW * scFontCharWidth);
    }
}

/**
 * @brief Font data pt. 1
 */
const u32 Nw4rDirectPrint::scFontData[] = {
    0x70871C30, 0x8988A250, 0x88808290, 0x88830C90, 0x888402F8, 0x88882210,
    0x71CF9C10, 0xF9CF9C70, 0x8208A288, 0xF200A288, 0x0BC11C78, 0x0A222208,
    0x8A222208, 0x71C21C70, 0x23C738F8, 0x5228A480, 0x8A282280, 0x8BC822F0,
    0xFA282280, 0x8A28A480, 0x8BC738F8, 0xF9C89C08, 0x82288808, 0x82088808,
    0xF2EF8808, 0x82288888, 0x82288888, 0x81C89C70, 0x8A08A270, 0x920DA288,
    0xA20AB288, 0xC20AAA88, 0xA208A688, 0x9208A288, 0x8BE8A270, 0xF1CF1CF8,
    0x8A28A220, 0x8A28A020, 0xF22F1C20, 0x82AA0220, 0x82492220, 0x81A89C20,
    0x8A28A288, 0x8A28A288, 0x8A289488, 0x8A2A8850, 0x894A9420, 0x894AA220,
    0x70852220, 0xF8011000, 0x08020800, 0x10840400, 0x20040470, 0x40840400,
    0x80020800, 0xF8011000, 0x70800000, 0x88822200, 0x08820400, 0x108F8800,
    0x20821000, 0x00022200, 0x20800020, 0x00000000};

/**
 * @brief Font data pt. 2
 */
const u32 Nw4rDirectPrint::scFontData2[] = {
    0x51421820, 0x53E7A420, 0x014A2C40, 0x01471000, 0x0142AA00, 0x03EAA400,
    0x01471A78, 0x00000000, 0x50008010, 0x20010820, 0xF8020040, 0x20420820,
    0x50441010, 0x00880000, 0x00070E00, 0x01088840, 0x78898820, 0x004A8810,
    0x788A8810, 0x01098808, 0x00040E04, 0x70800620, 0x11400820, 0x12200820,
    0x10001020, 0x10000820, 0x100F8820, 0x70000620, 0x60070000, 0x110F82A0,
    0x12AA8AE0, 0x084F92A0, 0x100FBE1C, 0x10089008, 0x60070808, 0x00000000,
    0x02000200, 0x7A078270, 0x8BC81E88, 0x8A2822F8, 0x9A282280, 0x6BC79E78,
    0x30000000, 0x48080810, 0x41E80000, 0x422F1830, 0xFBE88810, 0x40288890,
    0x43C89C60, 0x81000000, 0x81000000, 0x990F3C70, 0xA10AA288, 0xE10AA288,
    0xA10AA288, 0x98CAA270, 0x00000000, 0x00000020, 0xF1EF1E20, 0x8A28A0F8,
    0x8A281C20, 0xF1E80220, 0x80283C38, 0x00000000, 0x00000000, 0x8A28B688,
    0x8A2A8888, 0x8A2A8878, 0x894A8808, 0x788536F0, 0x00000000, 0x00000000,
    0xF8000000, 0x10000000, 0x20000000, 0x40000000, 0xF8000000};

/**
 * @brief Converts from ASCII index to font data index
 */
const u8 Nw4rDirectPrint::scAscii2Font[128] = {
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0xFD, 0xFE, 0x7A,
    0x7A, 0x7A, 0x7A, 0x7A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x29, 0x64, 0x65,
    0x66, 0x2B, 0x67, 0x68, 0x25, 0x26, 0x69, 0x2A, 0x6A, 0x27, 0x2C, 0x6B,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x24, 0x6C,
    0x6D, 0x6E, 0x6F, 0x28, 0x70, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
    0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x71, 0x72, 0x73, 0x74, 0x75,
    0xFF, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93,
    0x94, 0x95, 0x96, 0x76, 0x77, 0x78, 0x79, 0x7A};

} // namespace kiwi
