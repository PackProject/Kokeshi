#include <libkiwi.h>

/**
 * @brief Define stream functions by type
 */
#define IO_FUNC_DEF(T)                                                         \
    T IStream::Read_##T() {                                                    \
        T* ptr;                                                                \
        T value = static_cast<T>(0);                                           \
                                                                               \
        /* Unaligned read */                                                   \
        if (GetAlign() == 1) {                                                 \
            Read(&value, sizeof(T));                                           \
            return value;                                                      \
        }                                                                      \
                                                                               \
        /* Aligned read requires allocation */                                 \
        ptr = new (GetAlign()) T();                                            \
        Read(ptr, sizeof(T));                                                  \
        value = *ptr;                                                          \
        delete ptr;                                                            \
                                                                               \
        return value;                                                          \
    }                                                                          \
                                                                               \
    T IStream::Write_##T(T value) {                                            \
        T* ptr;                                                                \
                                                                               \
        /* Unaligned write */                                                  \
        if (GetAlign() == 1) {                                                 \
            Write(&value, sizeof(T));                                          \
            return;                                                            \
        }                                                                      \
                                                                               \
        /* Aligned write requires allocation */                                \
        ptr = new (GetAlign()) T();                                            \
        *ptr = value;                                                          \
        Write(ptr, sizeof(T));                                                 \
        delete ptr;                                                            \
    }                                                                          \
                                                                               \
    T IStream::Peek_##T() {                                                    \
        T* ptr;                                                                \
        T value = static_cast<T>(0);                                           \
                                                                               \
        /* Unaligned peek */                                                   \
        if (GetAlign() == 1) {                                                 \
            Peek(&value, sizeof(T));                                           \
            return value;                                                      \
        }                                                                      \
                                                                               \
        /* Aligned read requires allocation */                                 \
        ptr = new (GetAlign()) T();                                            \
        Peek(ptr, sizeof(T));                                                  \
        value = *ptr;                                                          \
        delete ptr;                                                            \
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
    K_ASSERT_EX(IsAlign(dst), "Buffer must be aligned to %d bytes", GetAlign());

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
    K_ASSERT_EX(IsAlign(src), "Buffer must be aligned to %d bytes", GetAlign());

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
    K_ASSERT_EX(IsAlign(dst), "Buffer must be aligned to %d bytes", GetAlign());

    s32 n = PeekImpl(dst, size);
    return n;
}

/**
 * @brief Read string from the stream
 */
String IStream::Read_string() {
    static int sTextBufferPos = 0;
    static char sTextBuffer[0x400];

    // Form string in work buffer
    while (sTextBufferPos < LENGTHOF(sTextBuffer)) {
        char ch = Read_s8();
        sTextBuffer[sTextBufferPos++] = ch;

        if (ch == '\0') {
            break;
        }
    }

    // No matter what happened, null terminator should be at the end
    K_ASSERT_EX(sTextBuffer[sTextBufferPos] == '\0', "Buffer overflow");
}

/**
 * @brief Write string to the stream
 *
 * @param str String to write
 */
void IStream::Write_string(const String& str) {
    Write(str.CStr(), str.Length());
    Write_s8(0x00);
}

/**
 * @brief Peek string in the stream
 */
String IStream::Peek_string() {
    // Just seek back
    String str = Read_string();
    Seek(ESeekDir_Current, -str.Length());
    return str;
}

} // namespace kiwi
