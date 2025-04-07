#ifndef LIBKIWI_CORE_I_STREAM_H
#define LIBKIWI_CORE_I_STREAM_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>
#include <libkiwi/util/kiwiPtrUtil.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief Stream seek origin
 */
enum ESeekDir {
    ESeekDir_Begin,   //!< Seek relative to the beginning of the stream
    ESeekDir_Current, //!< Seek relative to the stream's current position
    ESeekDir_End      //!< Seek relative to the end of the stream
};

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
    virtual ~IStream() {
        // Warn for derived types :D
        K_WARN_EX(IsOpen(), "Your class forgot to close the stream!\n");
    }

    /**
     * @brief Closes this stream
     */
    virtual void Close() = 0;

    /**
     * @brief Tests whether the stream has hit the end-of-file
     */
    virtual bool IsEOF() const = 0;

    /**
     * @brief Check whether stream is available to use
     */
    bool IsOpen() const {
        return mIsOpen;
    }

    /**
     * @brief Tests whether this stream type supports seeking
     */
    virtual bool CanSeek() const = 0;
    /**
     * @brief Tests whether this stream type supports reading
     */
    virtual bool CanRead() const = 0;
    /**
     * @brief Tests whether this stream type supports writing
     */
    virtual bool CanWrite() const = 0;

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
        return 1;
    }

    /**
     * @brief Tests whether a given size fits the alignment required by this
     * stream type
     *
     * @param size Specified size
     */
    bool IsSizeAlign(u32 size) const {
        return size % GetSizeAlign() == 0;
    }

    /**
     * @brief Tests whether a given offset fits the alignment required by this
     * stream type
     *
     * @param offset Specified offset
     */
    bool IsOffsetAlign(u32 offset) const {
        return offset % GetOffsetAlign() == 0;
    }

    /**
     * @brief Tests whether a given buffer pointer fits the alignment required
     * by this stream type
     *
     * @param pBuffer Specified buffer pointer
     */
    bool IsBufferAlign(const void* pBuffer) const {
        return PtrUtil::IsAlignedPointer(pBuffer, GetBufferAlign());
    }

    /**
     * @brief Advances this stream's position
     *
     * @param dir Seek direction
     * @param offset Seek offset
     */
    void Seek(ESeekDir dir, s32 offset);

    /**
     * @brief Reads data from this stream
     *
     * @param pDst Destination buffer
     * @param size Number of bytes to read
     * @return Number of bytes read, or DVD error code
     */
    s32 Read(void* pDst, u32 size);

    /**
     * @brief Writes data to this stream
     *
     * @param pSrc Source buffer
     * @param size Number of bytes to write
     * @return Number of bytes written, or DVD error code
     */
    s32 Write(const void* pSrc, u32 size);

    /**
     * @brief Reads data from this stream without advancing the stream's
     * position (internal implementation)
     *
     * @param pDst Destination buffer
     * @param size Number of bytes to read
     * @return Number of bytes read, or DVD error code
     */
    s32 Peek(void* pDst, u32 size);

protected:
    /**
     * @brief Advances this stream's position (internal implementation)
     *
     * @param dir Seek direction
     * @param offset Seek offset
     */
    virtual void SeekImpl(ESeekDir dir, s32 offset) = 0;

    /**
     * @brief Reads data from this stream (internal implementation)
     *
     * @param pDst Destination buffer
     * @param size Number of bytes to read
     * @return Number of bytes read, or error code
     */
    virtual s32 ReadImpl(void* pDst, u32 size) = 0;

    /**
     * @brief Writes data to this stream (internal implementation)
     *
     * @param pSrc Source buffer
     * @param size Number of bytes to write
     * @return Number of bytes written, or error code
     */
    virtual s32 WriteImpl(const void* pSrc, u32 size) = 0;

    /**
     * @brief Reads data from this stream without advancing the stream's
     * position (internal implementation)
     *
     * @param pDst Destination buffer
     * @param size Number of bytes to read
     * @return Number of bytes read, or error code
     */
    virtual s32 PeekImpl(void* pDst, u32 size) = 0;

protected:
    bool mIsOpen;  //!< Stream open flag
    u32 mPosition; //!< Position in data
};

//! @}
} // namespace kiwi

#endif
