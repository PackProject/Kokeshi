#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Seek around through the stream
 *
 * @param dir Seek origin
 * @param offset Seek amount
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
 * @brief Read data from the stream
 *
 * @param dst Destination buffer
 * @param size Number of bytes to read
 * @return s32 Number of bytes read, or error code
 */
s32 IStream::Read(void* dst, u32 size) {
    K_ASSERT(dst != NULL);
    K_ASSERT_EX(IsOpen(), "Stream is not available");
    K_ASSERT_EX(CanRead(), "Stream does not support reading");

    K_ASSERT_EX(IsBufferAlign(dst), "Buffer must be aligned to %d bytes",
                GetBufferAlign());

    K_ASSERT_EX(IsSizeAlign(size),
                "This stream type requires sizes aligned to %d bytes",
                GetSizeAlign());

    s32 n = ReadImpl(dst, size);
    if (n > 0) {
        mPosition += n;
    }

    return n;
}

/**
 * @brief Write data to the stream
 *
 * @param src Source buffer
 * @param size Number of bytes to write
 * @return s32 Number of bytes written, or error code
 */
s32 IStream::Write(const void* src, u32 size) {
    K_ASSERT(src != NULL);
    K_ASSERT_EX(IsOpen(), "Stream is not available");
    K_ASSERT_EX(CanWrite(), "Stream does not support writing");

    K_ASSERT_EX(IsBufferAlign(src), "Buffer must be aligned to %d bytes",
                GetBufferAlign());

    K_ASSERT_EX(IsSizeAlign(size),
                "This stream type requires sizes aligned to %d bytes",
                GetSizeAlign());

    s32 n = WriteImpl(src, size);
    if (n > 0) {
        mPosition += n;
    }

    return n;
}

/**
 * @brief Peek data in the stream
 *
 * @param dst Destination buffer
 * @param size Number of bytes to peek
 * @return s32 Number of bytes peeked, or error code
 */
s32 IStream::Peek(void* dst, u32 size) {
    K_ASSERT(dst != NULL);
    K_ASSERT_EX(IsOpen(), "Stream is not available");
    K_ASSERT_EX(CanRead() && CanSeek(), "Stream does not support peeking");

    K_ASSERT_EX(IsBufferAlign(dst), "Buffer must be aligned to %d bytes",
                GetBufferAlign());

    K_ASSERT_EX(IsSizeAlign(size),
                "This stream type requires sizes aligned to %d bytes",
                GetSizeAlign());

    return PeekImpl(dst, size);
}

} // namespace kiwi
