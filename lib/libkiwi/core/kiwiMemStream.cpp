#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Seek stream
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
 * @brief Read data from the stream
 *
 * @param dst Destination buffer
 * @param size Number of bytes to read
 * @return s32 Number of bytes read, or error code
 */
s32 MemStream::ReadImpl(void* dst, u32 size) {
    K_ASSERT(dst != NULL);

    std::memcpy(dst, mBufferData + mPosition, size);
    return size;
}

/**
 * @brief Write data to the stream
 *
 * @param src Source buffer
 * @param size Number of bytes to write
 * @return s32 Number of bytes written, or error code
 */
s32 MemStream::WriteImpl(const void* src, u32 size) {
    K_ASSERT(src != NULL);

    std::memcpy(mBufferData + mPosition, src, size);
    return size;
}

/**
 * @brief Peek data in the stream
 *
 * @param dst Destination buffer
 * @param size Number of bytes to peek
 * @return s32 Number of bytes peeked, or error code
 */
s32 MemStream::PeekImpl(void* dst, u32 size) {
    return ReadImpl(dst, size);
}

} // namespace kiwi