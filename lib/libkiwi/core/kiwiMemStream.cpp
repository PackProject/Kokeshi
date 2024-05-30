#include <libkiwi.h>

/**
 * @brief Define stream functions by type
 */
#define IO_FUNC_DEF(T)                                                         \
    T MemStream::Read_##T() {                                                  \
        T* ptr;                                                                \
        T value = static_cast<T>(0);                                           \
                                                                               \
        s32 n = Read(&value, sizeof(T));                                       \
        K_ASSERT(n > 0);                                                       \
                                                                               \
        return value;                                                          \
    }                                                                          \
                                                                               \
    void MemStream::Write_##T(T value) {                                       \
        T* ptr;                                                                \
                                                                               \
        s32 n = Write(&value, sizeof(T));                                      \
        K_ASSERT(n > 0);                                                       \
    }                                                                          \
                                                                               \
    T MemStream::Peek_##T() {                                                  \
        T* ptr;                                                                \
        T value = static_cast<T>(0);                                           \
                                                                               \
        s32 n = Peek(&value, sizeof(T));                                       \
        K_ASSERT(n > 0);                                                       \
                                                                               \
        return value;                                                          \
    }

namespace kiwi {

IO_FUNC_DEF(u8);
IO_FUNC_DEF(s8);
IO_FUNC_DEF(u16);
IO_FUNC_DEF(s16);
IO_FUNC_DEF(u32);
IO_FUNC_DEF(s32);
IO_FUNC_DEF(u64);
IO_FUNC_DEF(s64);
IO_FUNC_DEF(f32);
IO_FUNC_DEF(f64);
IO_FUNC_DEF(bool);

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

/**
 * @brief Read string from the stream
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
 * @brief Write string to the stream
 *
 * @param str String to write
 */
void MemStream::Write_string(const String& str) {
    Write(str.CStr(), str.Length());
    Write_s8(0x00);
}

/**
 * @brief Peek string in the stream
 */
String MemStream::Peek_string() {
    // Just seek back
    String str = Read_string();
    Seek(ESeekDir_Current, -str.Length());
    return str;
}

} // namespace kiwi
