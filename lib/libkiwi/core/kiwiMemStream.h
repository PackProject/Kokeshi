#ifndef LIBKIWI_CORE_MEM_STREAM_H
#define LIBKIWI_CORE_MEM_STREAM_H
#include <libkiwi/core/kiwiFileRipper.h>
#include <libkiwi/core/kiwiFileStream.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiWorkBuffer.h>

/**
 * @brief Declare stream functions by type
 */
#define IO_FUNC_DECL(T)                                                        \
    T Read_##T();                                                              \
    void Write_##T(T value);                                                   \
    T Peek_##T();

namespace kiwi {

/**
 * @brief Memory buffer stream
 */
class MemStream : public FileStream {
public:
    /**
     * @brief Constructor
     */
    MemStream() : FileStream(EOpenMode_RW) {
        Open(NULL, 0);
    }

    /**
     * @brief Constructor
     *
     * @param buffer Buffer
     * @param size Buffer size
     * @param owns Whether the stream owns the buffer
     */
    MemStream(void* buffer, u32 size, bool owns = false)
        : FileStream(EOpenMode_RW) {
        Open(buffer, size, owns);
    }

    /**
     * @brief Constructor (for const pointers)
     *
     * @param buffer Read-only buffer
     * @param size Buffer size
     * @param owns Whether the stream owns the buffer
     */
    MemStream(const void* buffer, u32 size, bool owns = false)
        : FileStream(EOpenMode_Read) {
        Open(const_cast<void*>(buffer), size, owns);
    }

    /**
     * @brief Constructor (for work buffer)
     *
     * @param buffer Work buffer
     */
    MemStream(const WorkBuffer& buffer) : FileStream(EOpenMode_RW) {
        Open(buffer.Contents(), buffer.Size(), false);
    }

    /**
     * @brief Destructor
     */
    virtual ~MemStream() {
        Close();
    }

    /**
     * @brief Close stream
     */
    virtual void Close() {
        if (mOwnsBuffer) {
            delete mBufferData;
        }
    }

    virtual u32 GetSize() const {
        return mBufferSize;
    }

    virtual bool CanSeek() const {
        return true;
    }
    virtual bool CanRead() const {
        return true;
    }
    virtual bool CanWrite() const {
        return true;
    }
    virtual bool CanPeek() const {
        return true;
    }

    /**
     * Required byte-alignment
     */
    virtual s32 GetSizeAlign() const {
        return 1;
    }
    virtual s32 GetOffsetAlign() const {
        return 1;
    }
    virtual s32 GetBufferAlign() const {
        return 4;
    }

    /**
     * @brief Open a new buffer
     *
     * @param buffer Buffer
     * @param size Buffer size
     * @param owns Whether the stream owns the buffer
     */
    void Open(void* buffer, u32 size, bool owns = false) {
        if (IsOpen()) {
            Close();
        }

        mBufferData = static_cast<u8*>(buffer);
        mBufferSize = size;
        mOwnsBuffer = owns;

        mIsOpen = mBufferData != NULL;
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

private:
    virtual void SeekImpl(ESeekDir dir, s32 offset);
    virtual s32 ReadImpl(void* dst, u32 size);
    virtual s32 WriteImpl(const void* src, u32 size);
    virtual s32 PeekImpl(void* dst, u32 size);

private:
    u8* mBufferData;  // Memory buffer
    u32 mBufferSize;  // Buffer size
    bool mOwnsBuffer; // Whether the stream owns the buffer
};

} // namespace kiwi

#undef IO_FUNC_DECL

#endif
