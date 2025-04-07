#include <libkiwi.h>

/**
 * @brief Helper for defining stream functions for primitive types
 */
#define PRIM_DEF(T)                                                            \
    T MemStream::Read_##T() {                                                  \
        T value = static_cast<T>(0);                                           \
                                                                               \
        s32 n = Read(&value, sizeof(T));                                       \
        K_ASSERT(n > 0);                                                       \
                                                                               \
        return value;                                                          \
    }                                                                          \
                                                                               \
    void MemStream::Write_##T(T value) {                                       \
        s32 n = Write(&value, sizeof(T));                                      \
        K_ASSERT(n > 0);                                                       \
    }                                                                          \
                                                                               \
    T MemStream::Peek_##T() {                                                  \
        T value = static_cast<T>(0);                                           \
                                                                               \
        s32 n = Peek(&value, sizeof(T));                                       \
        K_ASSERT(n > 0);                                                       \
                                                                               \
        return value;                                                          \
    }

namespace kiwi {

/**
 * @name Primitives
 * @brief Read/Write/Peek for primitive types
 */
/**@{*/
PRIM_DEF(u8);
PRIM_DEF(s8);
PRIM_DEF(u16);
PRIM_DEF(s16);
PRIM_DEF(u32);
PRIM_DEF(s32);
PRIM_DEF(u64);
PRIM_DEF(s64);
PRIM_DEF(f32);
PRIM_DEF(f64);
PRIM_DEF(bool);
/**@}*/

/**
 * @brief Opens stream to a memory buffer
 *
 * @param pBuffer Buffer
 * @param size Buffer size
 * @param owns Whether the stream owns the buffer
 */
void MemStream::Open(void* pBuffer, u32 size, bool owns) {
    // Close existing buffer
    Close();

    mpBuffer = static_cast<u8*>(pBuffer);
    mBufferSize = size;
    mOwnsBuffer = owns;

    mIsOpen = mpBuffer != nullptr;
}

/**
 * @brief Closes this stream
 */
void MemStream::Close() {
    if (!IsOpen()) {
        return;
    }

    if (mOwnsBuffer) {
        delete mpBuffer;
        mpBuffer = nullptr;
    }

    mIsOpen = false;
}

/**
 * @brief Advances this stream's position (internal implementation)
 *
 * @param dir Seek direction
 * @param offset Seek offset
 */
void MemStream::SeekImpl(ESeekDir dir, s32 offset) {
    switch (dir) {
    case ESeekDir_Begin:   mPosition = offset; break;
    case ESeekDir_Current: mPosition += offset; break;
    case ESeekDir_End:
        K_ASSERT_EX(offset < 0, "Can't seek forward from end of file");
        K_ASSERT_EX(offset > -GetSize(), "Too far backwards");
        mPosition = GetSize() + offset;
        break;
    }

    K_ASSERT_EX(mPosition < GetSize(), "Can't seek past end of file");
}

/**
 * @brief Reads data from this stream (internal implementation)
 *
 * @param pDst Destination buffer
 * @param size Number of bytes to read
 * @return Number of bytes read, or error code
 */
s32 MemStream::ReadImpl(void* pDst, u32 size) {
    K_ASSERT(pDst != nullptr);

    std::memcpy(pDst, mpBuffer + mPosition, size);
    return size;
}

/**
 * @brief Writes data to this stream (internal implementation)
 *
 * @param pSrc Source buffer
 * @param size Number of bytes to write
 * @return Number of bytes written, or error code
 */
s32 MemStream::WriteImpl(const void* pSrc, u32 size) {
    K_ASSERT(pSrc != nullptr);

    std::memcpy(mpBuffer + mPosition, pSrc, size);
    return size;
}

/**
 * @brief Reads data from this stream without advancing the stream's
 * position (internal implementation)
 *
 * @param pDst Destination buffer
 * @param size Number of bytes to read
 * @return Number of bytes read, or error code
 */
s32 MemStream::PeekImpl(void* pDst, u32 size) {
    return ReadImpl(pDst, size);
}

/**
 * @brief Reads a C-style string from this stream
 */
String MemStream::Read_string() {
    static int sTextBufferPos = 0;
    static char sTextBuffer[0x400];

    // Form string in work buffer
    while (sTextBufferPos < LENGTHOF(sTextBuffer)) {
        char ch = Read_s8();
        sTextBuffer[sTextBufferPos++] = ch;

        // Null terminator
        if (ch == '\0') {
            break;
        }

        // End-of-file
        if (IsEOF()) {
            break;
        }
    }

    // No matter what happened, null terminator should be at the end
    sTextBuffer[sTextBufferPos] = '\0';
    return String(sTextBuffer);
}

/**
 * @brief Writes a C-style string to this stream
 *
 * @param rStr String
 */
void MemStream::Write_string(const String& rStr) {
    Write(rStr.CStr(), rStr.Length());
    Write_s8(0x00);
}

/**
 * @brief Reads a C-style string from this stream without advancing the
 * stream's position
 */
String MemStream::Peek_string() {
    String str = Read_string();
    Seek(ESeekDir_Current, -(str.Length() + 1));
    return str;
}

} // namespace kiwi
