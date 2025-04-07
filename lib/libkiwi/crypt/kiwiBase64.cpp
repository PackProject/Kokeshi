#include <libkiwi.h>

namespace kiwi {
namespace {

/**
 * @brief Base64 encoding table
 */
const char scEncodeTable[64] = {
    // clang-format off
    /*         0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F */
    /* 0x00 */  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',
    /* 0x10 */  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  'a',  'b',  'c',  'd',  'e',  'f',
    /* 0x20 */  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',
    /* 0x30 */  'w',  'x',  'y',  'z',  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '+',  '/'
    // clang-format on
};

/**
 * @brief Base64 decoding table
 */
const u8 scDecodeTable[128] = {
    // clang-format off
    /*         0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F */
    /* 0x00 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /* 0x10 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /* 0x20 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   62, 0xFF, 0xFF, 0xFF,   63,
    /* 0x30 */   52,   53,   54,   55,   56,   57,   58,   59,   60,   61, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /* 0x40 */ 0xFF,    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
    /* 0x50 */   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /* 0x60 */ 0xFF,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
    /* 0x70 */   41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    // clang-format on
};

/**
 * @brief Calculates the buffer size required to encode the binary data
 *
 * @param size Binary data size
 */
u32 GetEncodeSize(u32 size) {
    return ROUND_UP((4 * size) / 3, 4);
}

/**
 * @brief Attempts to decode the specified character using the decoding table
 *
 * @param rCh Character
 * @return Success
 */
bool TryDecodeChar(u8& rCh) {
    if (rCh > LENGTHOF(scDecodeTable)) {
        return false;
    }

    rCh = scDecodeTable[rCh];
    return true;
}

} // namespace

/**
 * @brief Encodes binary data into a string of Base64 characters
 *
 * @param pData Binary data
 * @param size Data size
 */
String B64Encode(const void* pData, u32 size) {
    enum State {
        State_EncOctet1st,
        State_EncOctet2nd,
        State_EncOctet3rd,
    };

    // Reserve space to avoid reallocation
    String encode;
    encode.Reserve(GetEncodeSize(size));

    const u8* p = static_cast<const u8*>(pData);
    State state = State_EncOctet1st;

    for (; size > 0; size--, p++) {
        switch (state) {
        case State_EncOctet1st: {
            encode += scEncodeTable[(p[0] & 0b11111100) >> 2];
            state = State_EncOctet2nd;
            break;
        }

        case State_EncOctet2nd: {
            encode += scEncodeTable[(p[-1] & 0b00000011) << 4 |
                                    (p[0] & 0b11110000) >> 4];
            state = State_EncOctet3rd;
            break;
        }

        case State_EncOctet3rd: {
            encode += scEncodeTable[(p[-1] & 0b00001111) << 2 |
                                    (p[0] & 0b11000000) >> 6];
            encode += scEncodeTable[p[0] & 0b00111111];
            state = State_EncOctet1st;
            break;
        }
        }
    }

    // Need padding if source data is not block aligned
    switch (state) {
    case State_EncOctet2nd: {
        encode += scEncodeTable[(p[-1] & 0b00000011) << 4];
        encode += "==";
        break;
    }

    case State_EncOctet3rd: {
        encode += scEncodeTable[(p[-1] & 0b00001111) << 2];
        encode += '=';
        break;
    }

    case State_EncOctet1st:
    default:                {
        break;
    }
    }

    return encode;
}

/**
 * @brief Decodes a string of Base64 characters into binary data
 *
 * @param rData Base64 encoded string
 * @param pDst Buffer for decoded data
 * @param size Size of the destination buffer
 *
 * @return Whether the decoded data fit completely within the buffer
 */
bool B64Decode(const String& rData, void* pDst, u32 size) {
    enum State {
        State_DecOctet1st,
        State_DecOctet2nd,
        State_DecOctet3rd,
        State_DecOctet4th,
    };

    if (size < B64GetDecodeSize(rData)) {
        return false;
    }

    u8* p = static_cast<u8*>(pDst);
    State state = State_DecOctet1st;

    for (int i = 0; i < rData.Length(); i++) {
        K_ASSERT(i < size);

        switch (state) {
        case State_DecOctet1st: {
            state = State_DecOctet2nd;
            break;
        }

        case State_DecOctet2nd: {
            if (rData[i] != '=') {
                u8 c0 = static_cast<u8>(rData[i - 1]);
                u8 c1 = static_cast<u8>(rData[i]);

                if (!TryDecodeChar(c0) || !TryDecodeChar(c1)) {
                    return false;
                }

                *p++ = (c0 & 0b111111) << 2 | (c1 & 0b110000) >> 4;
            }

            state = State_DecOctet3rd;
            break;
        }

        case State_DecOctet3rd: {
            if (rData[i] != '=') {
                u8 c0 = static_cast<u8>(rData[i - 1]);
                u8 c1 = static_cast<u8>(rData[i]);

                if (!TryDecodeChar(c0) || !TryDecodeChar(c1)) {
                    return false;
                }

                *p++ = (c0 & 0b001111) << 4 | (c1 & 0b111100) >> 2;
            }

            state = State_DecOctet4th;
            break;
        }

        case State_DecOctet4th: {
            if (rData[i] != '=') {
                u8 c0 = static_cast<u8>(rData[i - 1]);
                u8 c1 = static_cast<u8>(rData[i]);

                if (!TryDecodeChar(c0) || !TryDecodeChar(c1)) {
                    return false;
                }

                *p++ = (c0 & 0b000011) << 6 | (c1 & 0b111111);
            }

            state = State_DecOctet1st;
            break;
        }
        }
    }

    K_ASSERT_EX(state == State_DecOctet1st, "Base64 string is missing padding");
    return true;
}

/**
 * @brief Decodes a string of Base64 characters into binary data
 * @note Caller is responsible for freeing the returned buffer
 *
 * @param rData Base64 encoded string
 * @param[out] pOutSize Decoded data size
 *
 * @return Buffer containing decoded data (null if decoding failed)
 */
void* B64Decode(const String& rData, u32* pSize) {
    u32 size = B64GetDecodeSize(rData);

    void* pDst = new u8[size];
    K_ASSERT(pDst != nullptr);

    bool success = B64Decode(rData, pDst, size);
    if (!success) {
        *pSize = 0;
        return nullptr;
    }

    if (pSize != nullptr) {
        *pSize = size;
    }

    return pDst;
}

} // namespace kiwi
