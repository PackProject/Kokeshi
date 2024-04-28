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
        K_LOG_EX("Can't find DVD file: %s\n", path);
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
    switch (dir) {
    case ESeekDir_Begin:   mPosition = offset; break;
    case ESeekDir_Current: mPosition += offset; break;
    case ESeekDir_End:
        K_ASSERT_EX(offset < 0, "Can't seek forward from end of file");
        K_ASSERT_EX(offset > -GetSize(), "Too far backwards");
        mPosition = GetSize() + offset;
        break;
    }

    K_ASSERT_EX(mPosition < GetSize(), "Can't seek past end of file");
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
    return DVDReadPrio(&mFileInfo, dst, size, mPosition, DVD_PRIO_MEDIUM);
}

/**
 * @brief Write data to the stream
 *
 * @param src Source buffer
 * @param size Number of bytes to write
 * @return s32 Number of bytes written, or error code
 */
s32 DvdStream::WriteImpl(const void* src, u32 size) {
    K_ASSERT_EX(false, "Can't write to the DVD");
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
    s32 n = ReadImpl(dst, size);
    mPosition -= n;
    return n;
}

} // namespace kiwi
