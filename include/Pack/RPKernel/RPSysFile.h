#ifndef RP_KERNEL_FILE_H
#define RP_KERNEL_FILE_H
#include <Pack/RPTypes.h>

#include <egg/core.h>

#include <nw4r/ut.h>

/**
 * @brief Cached resource file
 * @wfuname
 */
class RPSysFile : public EGG::Disposer {
public:
    /**
     * @brief Constructor
     *
     * @param pPath File path (absolute)
     * @param size File data size
     * @param pData File data pointer
     */
    RPSysFile(const char* pPath, s32 size, const void* pData);
    /**
     * @brief Destructor
     * @details The file is automatically removed from the resource cache.
     */
    virtual ~RPSysFile();

public:
    //! Link node used for cache lists
    nw4r::ut::Link node; // at 0x10

private:
    //! File path
    char mPath[128]; // at 0x18
    //! File data size
    s32 mSize; // at 0x98
    //! File data contents
    const u8* mpData; // at 0x9C
};

#endif
