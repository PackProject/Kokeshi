#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Load a file
 *
 * @param path Path to the file
 * @param where Storage on which the file is located
 * @param arg Ripping parameters
 * @return File data
 */
void* FileRipper::Rip(const String& path, EStorage where,
                      const FileRipperArg& arg) {
    switch (where) {
    case EStorage_DVD: {
        DvdStream strm(path);
        return Rip(strm, arg);
    }
    case EStorage_NAND: {
        NandStream strm(path, EOpenMode_Read);
        return Rip(strm, arg);
    }
    default: K_ASSERT_EX(false, "Invalid storage type"); return NULL;
    }
}
/**
 * @brief Load a file
 *
 * @param strm Stream to file
 * @param arg Ripping parameters
 * @return File data
 */
void* FileRipper::Rip(FileStream& strm, const FileRipperArg& arg) {
    // Bad stream
    if (!strm.IsOpen()) {
        return NULL;
    }

    // Storage device may require byte-aligned size
    u32 fileSize = strm.GetSize();
    u32 bufferSize = ROUND_UP(fileSize, strm.GetSizeAlign());

    // User may have specified a destination buffer
    u8* buffer = static_cast<u8*>(arg.dst);

    // Ripper is responsible for allocating read buffer
    if (buffer == NULL) {
        buffer = new (strm.GetBufferAlign(), arg.region) u8[bufferSize];
    }

    K_ASSERT(buffer != NULL);
    K_ASSERT_EX(strm.IsBufferAlign(buffer),
                "Stream requires buffer aligned to %d bytes",
                strm.GetBufferAlign());

    // Try to read the entire file
    s32 n = strm.Read(buffer, bufferSize);
    K_ASSERT(n == fileSize || n == bufferSize);

    // Report file size
    if (arg.size != NULL) {
        *arg.size = fileSize;
    }

    return buffer;
}

/**
 * @brief Load a file and open a stream
 *
 * @param path Path to the file
 * @param where Storage on which the file is located
 * @return File stream
 */
MemStream FileRipper::Open(const String& path, EStorage where) {
    void* file;
    u32 size;

    // Try to read the entire file
    FileRipperArg arg;
    arg.size = &size;
    file = Rip(path, where, arg);

    // Couldn't find file
    if (file == NULL) {
        return MemStream();
    }

    // Stream takes ownership of the buffer
    return MemStream(file, size, true);
}

} // namespace kiwi
