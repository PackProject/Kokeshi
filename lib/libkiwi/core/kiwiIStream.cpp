#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Advances this stream's position
 *
 * @param dir Seek direction
 * @param offset Seek offset
 */
void IStream::Seek(ESeekDir dir, s32 offset) {
    K_ASSERT_EX(IsOpen(), "Stream is not available");
    K_ASSERT_EX(CanSeek(), "Stream does not support seeking");

    K_ASSERT_EX(IsOffsetAlign(offset),
                "This stream type requires offsets aligned to %d bytes",
                GetOffsetAlign());

    SeekImpl(dir, offset);
}

/**
 * @brief Reads data from this stream
 *
 * @param pDst Destination buffer
 * @param size Number of bytes to read
 * @return Number of bytes read, or DVD error code
 */
s32 IStream::Read(void* pDst, u32 size) {
    K_ASSERT(pDst != nullptr);
    K_ASSERT_EX(IsOpen(), "Stream is not available");
    K_ASSERT_EX(CanRead(), "Stream does not support reading");

    K_ASSERT_EX(IsBufferAlign(pDst), "Buffer must be aligned to %d bytes",
                GetBufferAlign());

    K_ASSERT_EX(IsSizeAlign(size),
                "This stream type requires sizes aligned to %d bytes",
                GetSizeAlign());

    s32 n = ReadImpl(pDst, size);
    if (n > 0) {
        mPosition += n;
    }

    return n;
}

/**
 * @brief Writes data to this stream
 *
 * @param pSrc Source buffer
 * @param size Number of bytes to write
 * @return Number of bytes written, or DVD error code
 */
s32 IStream::Write(const void* pSrc, u32 size) {
    K_ASSERT(pSrc != nullptr);
    K_ASSERT_EX(IsOpen(), "Stream is not available");
    K_ASSERT_EX(CanWrite(), "Stream does not support writing");

    K_ASSERT_EX(IsBufferAlign(pSrc), "Buffer must be aligned to %d bytes",
                GetBufferAlign());

    K_ASSERT_EX(IsSizeAlign(size),
                "This stream type requires sizes aligned to %d bytes",
                GetSizeAlign());

    s32 n = WriteImpl(pSrc, size);
    if (n > 0) {
        mPosition += n;
    }

    return n;
}

/**
 * @brief Reads data from this stream without advancing the stream's
 * position (internal implementation)
 *
 * @param pDst Destination buffer
 * @param size Number of bytes to read
 * @return Number of bytes read, or DVD error code
 */
s32 IStream::Peek(void* pDst, u32 size) {
    K_ASSERT(pDst != nullptr);
    K_ASSERT_EX(IsOpen(), "Stream is not available");
    K_ASSERT_EX(CanRead() && CanSeek(), "Stream does not support peeking");

    K_ASSERT_EX(IsBufferAlign(pDst), "Buffer must be aligned to %d bytes",
                GetBufferAlign());

    K_ASSERT_EX(IsSizeAlign(size),
                "This stream type requires sizes aligned to %d bytes",
                GetSizeAlign());

    return PeekImpl(pDst, size);
}

} // namespace kiwi
