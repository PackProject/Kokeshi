#ifndef LIBKIWI_CORE_MEM_STREAM_H
#define LIBKIWI_CORE_MEM_STREAM_H
#include <libkiwi/core/kiwiFileStream.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiWorkBuffer.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief Memory buffer stream
 */
class MemStream : public FileStream {
public:
    /**
     * @brief Constructor
     */
    MemStream() : FileStream(EOpenMode_RW) {
        Open(nullptr, 0);
    }

    /**
     * @brief Constructor
     *
     * @param pBuffer Buffer
     * @param size Buffer size
     * @param owns Whether the stream owns the buffer
     */
    MemStream(void* pBuffer, u32 size, bool owns = false)
        : FileStream(EOpenMode_RW) {
        Open(pBuffer, size, owns);
    }

    /**
     * @brief Constructor
     * @details For const pointer
     *
     * @param pBuffer Read-only buffer
     * @param size Buffer size
     * @param owns Whether the stream owns the buffer
     */
    MemStream(const void* pBuffer, u32 size, bool owns = false)
        : FileStream(EOpenMode_Read) {
        Open(const_cast<void*>(pBuffer), size, owns);
    }

    /**
     * @brief Constructor
     * @details For work buffer
     *
     * @param rBuffer Work buffer
     */
    explicit MemStream(const WorkBuffer& rBuffer) : FileStream(EOpenMode_RW) {
        Open(rBuffer.Contents(), rBuffer.Size(), false);
    }

    /**
     * @brief Destructor
     * @details Automatically closes stream
     */
    virtual ~MemStream() {
        Close();
    }

    /**
     * @brief Opens stream to a memory buffer
     *
     * @param pBuffer Buffer
     * @param size Buffer size
     * @param owns Whether the stream owns the buffer
     */
    void Open(void* pBuffer, u32 size, bool owns = false);
    /**
     * @brief Closes this stream
     */
    virtual void Close() {
        if (IsOpen() && mOwnsBuffer) {
            delete mpBuffer;
        }
    }

    /**
     * @brief Gets the size of the currently open buffer
     */
    virtual u32 GetSize() const {
        return mBufferSize;
    }

    /**
     * @brief Tests whether this stream type supports seeking
     */
    virtual bool CanSeek() const {
        return true;
    }
    /**
     * @brief Tests whether this stream type supports reading
     */
    virtual bool CanRead() const {
        return true;
    }
    /**
     * @brief Tests whether this stream type supports writing
     */
    virtual bool CanWrite() const {
        return true;
    }

    /**
     * @brief Gets the size alignment required by this stream type
     */
    virtual s32 GetSizeAlign() const {
        return 1;
    }
    /**
     * @brief Gets the offset alignment required by this stream type
     */
    virtual s32 GetOffsetAlign() const {
        return 1;
    }
    /**
     * @brief Gets the buffer alignment required by this stream type
     */
    virtual s32 GetBufferAlign() const {
        return 4;
    }

/**
 * @brief Helper for declaring stream functions for primitive types
 */
#define PRIM_DECL(T)                                                           \
    T Read_##T();                                                              \
    void Write_##T(T value);                                                   \
    T Peek_##T();

    /**
     * @name Primitives
     * @brief Read/Write/Peek for primitive types
     */
    /**@{*/
    PRIM_DECL(u8);
    PRIM_DECL(s8);
    PRIM_DECL(u16);
    PRIM_DECL(s16);
    PRIM_DECL(u32);
    PRIM_DECL(s32);
    PRIM_DECL(u64);
    PRIM_DECL(s64);
    PRIM_DECL(f32);
    PRIM_DECL(f64);
    PRIM_DECL(bool);
    /**@}*/

#undef PRIM_DECL

    /**
     * @brief Reads a C-style string from this stream
     * @note String size limited to 0x400 (1024) bytes
     */
    String Read_string();
    /**
     * @brief Writes a C-style string to this stream
     *
     * @param rStr String
     */
    void Write_string(const String& rStr);
    /**
     * @brief Reads a C-style string from this stream without advancing the
     * stream's position
     * @note String size limited to 0x400 (1024) bytes
     */
    String Peek_string();

private:
    /**
     * @brief Advances this stream's position (internal implementation)
     *
     * @param dir Seek direction
     * @param offset Seek offset
     */
    virtual void SeekImpl(ESeekDir dir, s32 offset);

    /**
     * @brief Reads data from this stream (internal implementation)
     *
     * @param pDst Destination buffer
     * @param size Number of bytes to read
     * @return Number of bytes read, or error code
     */
    virtual s32 ReadImpl(void* pDst, u32 size);

    /**
     * @brief Writes data to this stream (internal implementation)
     *
     * @param pSrc Source buffer
     * @param size Number of bytes to write
     * @return Number of bytes written, or error code
     */
    virtual s32 WriteImpl(const void* pSrc, u32 size);

    /**
     * @brief Reads data from this stream without advancing the stream's
     * position (internal implementation)
     *
     * @param pDst Destination buffer
     * @param size Number of bytes to read
     * @return Number of bytes read, or error code
     */
    virtual s32 PeekImpl(void* pDst, u32 size);

private:
    u8* mpBuffer;     // Memory buffer
    u32 mBufferSize;  // Buffer size
    bool mOwnsBuffer; // Whether the stream owns the buffer
};

//! @}
} // namespace kiwi

#endif
