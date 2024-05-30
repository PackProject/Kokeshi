#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Open stream to NAND file
 *
 * @param path File path
 * @return Success
 */
bool NandStream::Open(const String& path) {
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
    result = NANDOpen(path, &mFileInfo, type);

    // Doesn't exist, but we should create it
    if (result == NAND_RESULT_NOEXISTS &&
        (mOpenMode == EOpenMode_Write || mOpenMode == EOpenMode_RW)) {
        // Release descriptor
        Close();

        // Try to create file
        if (NANDCreate(path, NAND_PERM_RWALL, 0) != NAND_RESULT_OK) {
            Close();
            return false;
        }

        // Need to open again
        if (NANDOpen(path, &mFileInfo, type) != NAND_RESULT_OK) {
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
 * @brief Close stream
 */
void NandStream::Close() {
    NANDClose(&mFileInfo);
    mIsOpen = false;
}

/**
 * @brief Get file byte size
 */
u32 NandStream::GetSize() const {
    // Parameter must be non-const according to decomp
    NANDFileInfo* info = const_cast<NANDFileInfo*>(&mFileInfo);

    u32 size = 0;
    s32 result = NANDGetLength(info, &size);

    return result == NAND_RESULT_OK ? size : 0;
}

/**
 * @brief Seek stream
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
 * @brief Read data from the stream
 *
 * @param dst Destination buffer
 * @param size Number of bytes to read
 * @return s32 Number of bytes read, or error code
 */
s32 NandStream::ReadImpl(void* dst, u32 size) {
    K_ASSERT(dst != NULL);
    return NANDRead(&mFileInfo, dst, size);
}

/**
 * @brief Write data to the stream
 *
 * @param src Source buffer
 * @param size Number of bytes to write
 * @return s32 Number of bytes written, or error code
 */
s32 NandStream::WriteImpl(const void* src, u32 size) {
    K_ASSERT(src != NULL);
    return NANDWrite(&mFileInfo, src, size);
}

/**
 * @brief Peek data in the stream
 *
 * @param dst Destination buffer
 * @param size Number of bytes to peek
 * @return s32 Number of bytes peeked, or error code
 */
s32 NandStream::PeekImpl(void* dst, u32 size) {
    K_ASSERT(dst != NULL);

    s32 n = ReadImpl(dst, size);
    if (n > 0) {
        s32 result = NANDSeek(&mFileInfo, -n, NAND_SEEK_CUR);
        K_WARN_EX(result < 0, "Seek back failed (%d)\n", result);
    }

    return n;
}

} // namespace kiwi
