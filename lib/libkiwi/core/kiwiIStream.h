#ifndef LIBKIWI_CORE_I_STREAM_H
#define LIBKIWI_CORE_I_STREAM_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>

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
    IStream() : mIsOpen(false), mPosition(0) {}

    /**
     * @brief Destructor
     */
    virtual ~IStream() {}

    virtual void Close() = 0;
    virtual bool IsEOF() const = 0;

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
    virtual s32 GetSizeAlign() const {
        return 1;
    }
    virtual s32 GetOffsetAlign() const {
        return 1;
    }
    virtual s32 GetBufferAlign() const {
        return 1;
    }

    bool IsSizeAlign(u32 size) const {
        return size % GetSizeAlign() == 0;
    }
    bool IsOffsetAlign(u32 offset) const {
        return offset % GetOffsetAlign() == 0;
    }
    bool IsBufferAlign(const void* ptr) const {
        return reinterpret_cast<u32>(ptr) % GetBufferAlign() == 0;
    }

protected:
    virtual void SeekImpl(ESeekDir dir, s32 offset) = 0;
    virtual s32 ReadImpl(void* dst, u32 size) = 0;
    virtual s32 WriteImpl(const void* src, u32 size) = 0;
    virtual s32 PeekImpl(void* dst, u32 size) = 0;

protected:
    bool mIsOpen;  // Stream open flag
    u32 mPosition; // Position in data
};

} // namespace kiwi

#endif
