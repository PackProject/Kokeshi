#ifndef LIBKIWI_CORE_FILE_RIPPER_H
#define LIBKIWI_CORE_FILE_RIPPER_H
#include <libkiwi/core/kiwiMemoryMgr.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiSmartPtr.h>

namespace kiwi {

// Forward declarations
class FileStream;
class MemStream;

/**
 * @brief File storage devices
 */
enum EStorage {
    EStorage_DVD,
    EStorage_NAND,
};

/**
 * @brief File ripping parameters
 */
struct FileRipperArg {
    // Where the file contents are stored.
    // Leave this as NULL to have the ripper allocate memory.
    void* dst;

    // Where the file size is stored
    u32* size;

    // Memory region to use for the destination buffer (when dst is NULL)
    kiwi::EMemory region;

    /**
     * @brief Constructor
     */
    FileRipperArg() : dst(NULL), size(NULL), region(kiwi::EMemory_MEM2) {}
};

/**
 * @brief File ripper/loader
 */
class FileRipper {
public:
    /**
     * @brief Load a file
     *
     * @param path Path to the file
     * @param where Storage on which the file is located
     * @param arg Ripping parameters
     * @return File data
     */
    static void* Rip(const String& path, EStorage where,
                     const FileRipperArg& arg = FileRipperArg());

    /**
     * @brief Load a file
     *
     * @param strm Stream to file
     * @param arg Ripping parameters
     * @return File data
     */
    static void* Rip(FileStream& strm,
                     const FileRipperArg& arg = FileRipperArg());

    /**
     * @brief Load a file and open a stream
     *
     * @param path Path to the file
     * @param where Storage on which the file is located
     * @return File stream
     */
    static MemStream Open(const String& path, EStorage where);
};

} // namespace kiwi

#endif
