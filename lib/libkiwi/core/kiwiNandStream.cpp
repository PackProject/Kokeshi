#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Opens stream to NAND file
 *
 * @param rPath File path
 * @return Success
 */
bool NandStream::Open(const String& rPath) {
    NANDAccessType type;
    s32 result;

    // Close existing file
    if (mIsOpen) {
        Close();
    }

    // Convert open mode for NAND
    switch (mOpenMode) {
    case EOpenMode_Read:  type = NAND_ACCESS_READ; break;
    case EOpenMode_Write: type = NAND_ACCESS_WRITE; break;
    case EOpenMode_RW:    type = NAND_ACCESS_RW; break;
    default:              K_ASSERT(false); break;
    }

    // Attempt to open
    result = NANDOpen(rPath, &mFileInfo, type);

    // Doesn't exist, but we should create it
    if (result == NAND_RESULT_NOEXISTS &&
        (mOpenMode == EOpenMode_Write || mOpenMode == EOpenMode_RW)) {
        // Release descriptor
        Close();

        // Try to create file
        if (NANDCreate(rPath, NAND_PERM_RWALL, 0) != NAND_RESULT_OK) {
            Close();
            return false;
        }

        // Need to open again
        if (NANDOpen(rPath, &mFileInfo, type) != NAND_RESULT_OK) {
            Close();
            return false;
        }

        mIsOpen = true;
        return true;
    }

    // Some other error has occurred
    if (result != NAND_RESULT_OK) {
        Close();
        return false;
    }

    mIsOpen = true;
    return true;
}

/**
 * @brief Closes this stream
 */
void NandStream::Close() {
    if (!IsOpen()) {
        return;
    }

    NANDClose(&mFileInfo);
    mIsOpen = false;
}

/**
 * @brief Gets the size of the currently open file
 */
u32 NandStream::GetSize() const {
    // Parameter must be non-const according to decomp
    NANDFileInfo* pInfo = const_cast<NANDFileInfo*>(&mFileInfo);

    u32 size = 0;
    s32 result = NANDGetLength(pInfo, &size);

    return result == NAND_RESULT_OK ? size : 0;
}

/**
 * @brief Advances this stream's position (internal implementation)
 *
 * @param dir Seek direction
 * @param offset Seek offset
 */
void NandStream::SeekImpl(ESeekDir dir, s32 offset) {
    NANDSeekMode mode;
    s32 result;

    // Convert seekdir for NAND
    switch (dir) {
    case ESeekDir_Begin:   mode = NAND_SEEK_BEG; break;
    case ESeekDir_Current: mode = NAND_SEEK_CUR; break;
    case ESeekDir_End:     mode = NAND_SEEK_END; break;
    default:               K_ASSERT(false); break;
    }

    result = NANDSeek(&mFileInfo, offset, mode);
    K_WARN_EX(result != NAND_RESULT_OK, "NANDSeek failed (%d)\n", result);
}

/**
 * @brief Reads data from this stream (internal implementation)
 *
 * @param pDst Destination buffer
 * @param size Number of bytes to read
 * @return Number of bytes read, or NAND error code
 */
s32 NandStream::ReadImpl(void* pDst, u32 size) {
    K_ASSERT(pDst != nullptr);
    return NANDRead(&mFileInfo, pDst, size);
}

/**
 * @brief Writes data to this stream (internal implementation)
 *
 * @param pSrc Source buffer
 * @param size Number of bytes to write
 * @return Number of bytes written, or NAND error code
 */
s32 NandStream::WriteImpl(const void* pSrc, u32 size) {
    K_ASSERT(pSrc != nullptr);
    return NANDWrite(&mFileInfo, pSrc, size);
}

/**
 * @brief Reads data from this stream without advancing the stream's
 * position (internal implementation)
 *
 * @param pDst Destination buffer
 * @param size Number of bytes to read
 * @return Number of bytes read, or NAND error code
 */
s32 NandStream::PeekImpl(void* pDst, u32 size) {
    K_ASSERT(pDst != nullptr);

    s32 n = ReadImpl(pDst, size);
    if (n > 0) {
        s32 result = NANDSeek(&mFileInfo, -n, NAND_SEEK_CUR);
        K_WARN_EX(result < 0, "Seek back failed (%d)\n", result);
    }

    return n;
}

} // namespace kiwi
