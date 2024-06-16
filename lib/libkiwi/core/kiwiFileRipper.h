#ifndef LIBKIWI_CORE_FILE_RIPPER_H
#define LIBKIWI_CORE_FILE_RIPPER_H
#include <libkiwi/core/kiwiMemoryMgr.h>
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

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
    //! @brief Where the file contents are stored
    //! @note Leave this as nullptr to have the ripper allocate memory
    void* pDst;

    //! Where the file size is stored
    u32* pSize;

    //! Memory region to use for ripper memory allocation
    EMemory region;

    /**
     * @brief Constructor
     */
    FileRipperArg() : pDst(nullptr), pSize(nullptr), region(EMemory_MEM2) {}
};

/**
 * @brief File ripper/loader
 */
class FileRipper {
public:
    /**
     * @brief Rips a file's contents
     *
     * @param rPath Path to the file
     * @param where Storage device on which the file is located
     * @param rArg Ripping parameters
     * @return File data (owned by you!)
     */
    static void* Rip(const String& rPath, EStorage where,
                     const FileRipperArg& rArg = FileRipperArg());

    /**
     * @brief Rips a file's contents
     *
     * @param rStrm Stream to the file
     * @param rArg Ripping parameters
     * @return File data (owned by you!)
     */
    static void* Rip(FileStream& rStrm,
                     const FileRipperArg& rArg = FileRipperArg());

    /**
     * @brief Rips a file's contents and opens a stream to it
     *
     * @param rPath Path to the file
     * @param where Storage device on which the file is located
     * @return File stream
     */
    static MemStream Open(const String& rPath, EStorage where);
};

//! @}
} // namespace kiwi

#endif
