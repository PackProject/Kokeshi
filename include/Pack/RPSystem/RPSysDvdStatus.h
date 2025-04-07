#ifndef RP_SYSTEM_DVD_STATUS_H
#define RP_SYSTEM_DVD_STATUS_H
#include <Pack/RPTypes.h>

#include <egg/core.h>

//! @addtogroup rp_system
//! @{

/**
 * @brief Disc status
 * @wfuname
 *
 * @history Likely predates EGG::DvdStatus.
 */
class RPSysDvdStatus {
    RP_SINGLETON_DECL_EX(RPSysDvdStatus);

public:
    /**
     * @brief Result of the last access to the DVD drive
     */
    enum EErrorStatus {
        EErrorStatus_Success = -2,
        EErrorStatus_Busy,      //!< Drive is busy
        EErrorStatus_NoDisk,    //!< Disk is ejected or is wrong game
        EErrorStatus_DiskError, //!< Cannot read from the disk
        EErrorStatus_Fatal      //!< Fatal error
    };

public:
    /**
     * @brief Draws an error message if an error has occurred
     */
    void draw();

    /**
     * @brief Updates state using the DVD library
     * @return Whether the DVD status is OK
     */
    bool update();

    /**
     * @brief Tests whether the status is in an error state
     * @details @ref EErrorStatus_Success and @ref EErrorStatus_Busy are *not*
     * considered error states.
     * @typo
     */
    bool isErrorOccured();

private:
    //! DVD error status
    EErrorStatus mStatus; // at 0x8
    //! Error message text
    const char* mpMessageWork; // at 0xC
    //! Fader for the black error background
    EGG::ColorFader* mpFader; // at 0x10
};

//! @}

#endif
