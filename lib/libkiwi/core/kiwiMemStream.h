#ifndef LIBKIWI_CORE_MEM_STREAM_H
#define LIBKIWI_CORE_MEM_STREAM_H
#include <libkiwi/core/kiwiFileStream.h>
#include <types.h>

namespace kiwi {

/**
 * @brief Memory buffer stream
 */
class MemStream : public FileStream {
public:
    /**
     * @brief Constructor
     *
     * @param buffer Buffer
     * @param size Buffer size
     * @param owns Whether the stream owns the buffer
     */
    MemStream(void* buffer, u32 size, bool owns = false)
        : FileStream(EOpenMode_RW), mBufferSize(size), mOwnsBuffer(owns) {
        mBufferData = static_cast<u8*>(buffer);
    }

    /**
     * @brief Constructor (for const pointers)
     *
     * @param buffer Read-only buffer
     * @param size Buffer size
     * @param owns Whether the stream owns the buffer
     */
    MemStream(const void* buffer, u32 size, bool owns = false)
        : FileStream(EOpenMode_RW), mBufferSize(size), mOwnsBuffer(owns) {
        mBufferData = static_cast<u8*>(const_cast<void*>(buffer));
    }

    /**
     * @brief Destructor
     */
    virtual ~MemStream() {
        if (mOwnsBuffer) {
            delete mBufferData;
        }
    }

    /**
     * @brief Close stream
     */
    virtual void Close() {}

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

    virtual s32 GetAlign() const {
        return 4;
    }

private:
    virtual void SeekImpl(ESeekDir dir, s32 offset);
    virtual s32 ReadImpl(void* dst, u32 size);
    virtual s32 WriteImpl(const void* src, u32 size);
    virtual s32 PeekImpl(void* dst, u32 size);

private:
    u8* mBufferData;
    u32 mBufferSize;

    bool mOwnsBuffer;
};

} // namespace kiwi

#endif
