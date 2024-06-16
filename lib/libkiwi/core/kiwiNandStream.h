#ifndef LIBKIWI_CORE_NAND_STREAM_H
#define LIBKIWI_CORE_NAND_STREAM_H
#include <libkiwi/core/kiwiFileStream.h>
#include <libkiwi/k_types.h>
#include <revolution/NAND.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief NAND file stream
 */
class NandStream : public FileStream {
public:
    /**
     * @brief Constructor
     *
     * @param mode Open mode
     */
    explicit NandStream(EOpenMode mode) : FileStream(mode) {}

    /**
     * @brief Constructor
     *
     * @param rPath File path
     * @param mode Open mode
     */
    NandStream(const String& rPath, EOpenMode mode) : FileStream(mode) {
        Open(rPath);
    }

    /**
     * @brief Destructor
     * @details Automatically closes stream
     */
    virtual ~NandStream() {
        Close();
    }

    /**
     * @brief Opens stream to NAND file
     *
     * @param rPath File path
     * @return Success
     */
    bool Open(const String& rPath);
    /**
     * @brief Closes this stream
     */
    virtual void Close();

    /**
     * @brief Gets the size of the currently open file
     */
    virtual u32 GetSize() const;

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
        return 32;
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
        return 32;
    }

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
     * @return Number of bytes read, or DVD error code
     */
    virtual s32 ReadImpl(void* pDst, u32 size);

    /**
     * @brief Writes data to this stream (internal implementation)
     *
     * @param pSrc Source buffer
     * @param size Number of bytes to write
     * @return Number of bytes written, or DVD error code
     */
    virtual s32 WriteImpl(const void* pSrc, u32 size);

    /**
     * @brief Reads data from this stream without advancing the stream's
     * position (internal implementation)
     *
     * @param pDst Destination buffer
     * @param size Number of bytes to read
     * @return Number of bytes read, or DVD error code
     */
    virtual s32 PeekImpl(void* pDst, u32 size);

private:
    NANDFileInfo mFileInfo; // NAND handle
};

//! @}
} // namespace kiwi

#endif
