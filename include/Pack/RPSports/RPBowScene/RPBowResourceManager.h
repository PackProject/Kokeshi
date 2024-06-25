#ifndef RP_SPORTS_BOW_RESOURCE_MANAGER_H
#define RP_SPORTS_BOW_RESOURCE_MANAGER_H
#include "IRPSysHostIOSocket.h"
#include "RPTypes.h"

/**
 * @brief Bowling scene resource manager
 * @customname
 */
class RPBowResourceManager : public IRPSysHostIOSocket {
    RP_SINGLETON_DECL(RPBowResourceManager);

public:
    /**
     * @brief Sets up all bowling resources
     * @address 80320e00
     */
    void LoadResource();
    /**
     * @brief Get specified file from the scene common archive
     * @address 80320d9c
     */
    void* GetFileFromCmnArchive(const char* path);

private:
    // @brief EXBIN globals (`BowExcelGlobal.bin`)
    void* mExcelGlobal; // at 0x0
    // @brief Scene common archive
    void* mCmnArchive; // at 0x4
    // @brief Scene local archive
    void* mLocalArchive; // at 0x8
};

#endif
