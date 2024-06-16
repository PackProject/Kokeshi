#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Rips a file's contents
 *
 * @param rPath Path to the file
 * @param where Storage device on which the file is located
 * @param rArg Ripping parameters
 * @return File data (owned by you!)
 */
void* FileRipper::Rip(const String& rPath, EStorage where,
                      const FileRipperArg& rArg) {
    switch (where) {
    case EStorage_DVD: {
        DvdStream strm(rPath);
        return Rip(strm, rArg);
    }
    case EStorage_NAND: {
        NandStream strm(rPath, EOpenMode_Read);
        return Rip(strm, rArg);
    }
    default: K_ASSERT_EX(false, "Invalid storage type"); return nullptr;
    }
}

/**
 * @brief Rips a file's contents
 *
 * @param rStrm Stream to the file
 * @param rArg Ripping parameters
 * @return File data (owned by you!)
 */
void* FileRipper::Rip(FileStream& rStrm, const FileRipperArg& rArg) {
    // Bad stream
    if (!rStrm.IsOpen()) {
        return nullptr;
    }

    // Storage device may require byte-aligned size
    u32 fileSize = rStrm.GetSize();
    u32 bufferSize = ROUND_UP(fileSize, rStrm.GetSizeAlign());

    // User may have specified a destination buffer
    u8* pBuffer = static_cast<u8*>(rArg.pDst);

    // Ripper is responsible for allocating read buffer
    if (pBuffer == nullptr) {
        pBuffer = new (rStrm.GetBufferAlign(), rArg.region) u8[bufferSize];
    }

    K_ASSERT(pBuffer != nullptr);
    K_ASSERT_EX(rStrm.IsBufferAlign(pBuffer),
                "Stream requires buffer aligned to %d bytes",
                rStrm.GetBufferAlign());

    // Try to read the entire file
    s32 n = rStrm.Read(pBuffer, bufferSize);
    K_ASSERT(n == fileSize || n == bufferSize);

    // Report file size
    if (rArg.pSize != nullptr) {
        *rArg.pSize = fileSize;
    }

    return pBuffer;
}

/**
 * @brief Rips a file's contents and opens a stream to it
 *
 * @param rPath Path to the file
 * @param where Storage device on which the file is located
 * @return File stream
 */
MemStream FileRipper::Open(const String& rPath, EStorage where) {
    FileRipperArg arg;

    u32 size;
    arg.pSize = &size;

    // Try to read the entire file
    void* pFile = Rip(rPath, where, arg);

    // Couldn't find file
    if (pFile == nullptr) {
        return MemStream();
    }

    // Stream takes ownership of the buffer
    return MemStream(pFile, size, true);
}

} // namespace kiwi
