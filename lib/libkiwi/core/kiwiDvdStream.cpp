#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Open stream to DVD file
 *
 * @param path File path
 * @return Success
 */
bool DvdStream::Open(const char* path) {
    K_ASSERT(path != NULL);

    // Close existing file
    if (mIsOpen) {
        Close();
    }

    // Try to locate file on disc
    s32 entrynum = DVDConvertPathToEntrynum(path);
    if (entrynum < 0) {
        K_LOG_EX("Unable to find DVD file: %s", path);
        return false;
    }

    // Get handle to file
    if (!DVDFastOpen(entrynum, &mFileInfo)) {
        return false;
    }

    mIsOpen = true;
    return true;
}

/**
 * @brief Close stream
 */
void DvdStream::Close() {
    DVDClose(&mFileInfo);
    mIsOpen = false;
}

/**
 * @brief Get file byte size
 */
u32 DvdStream::GetSize() const {
    return mFileInfo.size;
}

/**
 * @brief Seek stream
 *
 * @param dir Seek direction
 * @param offset Seek offset
 */
void DvdStream::SeekImpl(ESeekDir dir, s32 offset) {
    K_ASSERT_EX(offset < GetSize(), "Cannot seek past end of file");

    // Seek is "buffered" here, and applied later through DVDReadPrio.
    mSeekDir = dir;
    mSeekOffset = offset;
}

/**
 * @brief Read data from the stream
 *
 * @param dst Destination buffer
 * @param size Number of bytes to read
 * @return s32 Number of bytes read, or error code
 */
s32 DvdStream::ReadImpl(void* dst, u32 size) {
    K_ASSERT(dst != NULL);
    return DVDReadPrio(&mFileInfo, dst, size, mSeekOffset, DVD_PRIO_MEDIUM);
}

/**
 * @brief Write data to the stream
 *
 * @param src Source buffer
 * @param size Number of bytes to write
 * @return s32 Number of bytes written, or error code
 */
s32 DvdStream::WriteImpl(const void* src, u32 size) {
    K_ASSERT_EX(false, "Not supported");
    return 0;
}

/**
 * @brief Peek data in the stream
 *
 * @param dst Destination buffer
 * @param size Number of bytes to peek
 * @return s32 Number of bytes peeked, or error code
 */
s32 DvdStream::PeekImpl(void* dst, u32 size) {
    K_ASSERT_EX(false, "Not yet implemented");
    return 0;
}

} // namespace kiwi
