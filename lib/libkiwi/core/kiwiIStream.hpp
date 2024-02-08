#ifndef LIBKIWI_CORE_I_STREAM_H
#define LIBKIWI_CORE_I_STREAM_H
#include <libkiwi/kernel/kiwiAssert.h>
#include <libkiwi/prim/kiwiString.hpp>
#include <types.h>

/**
 * @brief Declare stream functions by type
 */
#define IO_FUNC_DECL(T)                                                        \
    T Read_##T();                                                              \
    T Write_##T(T value);                                                      \
    T Peek_##T();

namespace kiwi {

/**
 * @brief Seek origin
 */
enum ESeekDir { ESeekDir_Begin, ESeekDir_Current, ESeekDir_End };

/**
 * @brief Stream interface
 */
class IStream {
public:
    /**
     * @brief Constructor
     */
    IStream() : mIsOpen(false) {}

    /**
     * @brief Destructor
     */
    virtual ~IStream() {}

    virtual void Close() = 0;

    /**
     * @brief Check whether stream is available to use
     */
    bool IsOpen() const {
        return mIsOpen;
    }

    /**
     * Seek operation
     */
    virtual bool CanSeek() const = 0;
    void Seek(ESeekDir dir, s32 offset);

    /**
     * Read operation
     */
    virtual bool CanRead() const = 0;
    s32 Read(void* dst, u32 size);

    /**
     * Write operation
     */
    virtual bool CanWrite() const = 0;
    s32 Write(const void* src, u32 size);

    /**
     * Peek operation
     */
    s32 Peek(void* dst, u32 size);

    /**
     * Required byte-alignment
     */
    virtual s32 GetAlign() const {
        return 1;
    }
    bool IsAlign(const void* ptr) const {
        return reinterpret_cast<u32>(ptr) % GetAlign() == 0;
    }

    /**
     * Primitive types
     */
    IO_FUNC_DECL(u8);
    IO_FUNC_DECL(s8);
    IO_FUNC_DECL(u16);
    IO_FUNC_DECL(s16);
    IO_FUNC_DECL(u32);
    IO_FUNC_DECL(s32);
    IO_FUNC_DECL(u64);
    IO_FUNC_DECL(s64);
    IO_FUNC_DECL(f32);
    IO_FUNC_DECL(f64);
    IO_FUNC_DECL(bool);

    /**
     * User types
     */
    String Read_string();
    void Write_string(const String& str);
    String Peek_string();

protected:
    virtual void SeekImpl(ESeekDir dir, s32 offset) = 0;
    virtual s32 ReadImpl(void* dst, u32 size) = 0;
    virtual s32 WriteImpl(const void* src, u32 size) = 0;
    virtual s32 PeekImpl(void* dst, u32 size) = 0;

protected:
    // Stream open flag
    bool mIsOpen;
    // Position in data
    u32 mPosition;
};

} // namespace kiwi

#undef IO_FUNC_DECL

#endif
