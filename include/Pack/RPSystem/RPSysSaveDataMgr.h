#ifndef RP_SYSTEM_SAVE_DATA_MGR_H
#define RP_SYSTEM_SAVE_DATA_MGR_H
#include <Pack/RPKernel/IRPSysHostIOSocket.h>
#include <Pack/RPParty/RPPartyPlayerData.h>
#include <Pack/RPParty/RPPartySystemData.h>
#include <Pack/RPSports/RPSportsPlayerData.h>
#include <Pack/RPSports/RPSportsSystemData.h>
#include <Pack/RPTypes.h>
#include <egg/core.h>

//! @addtogroup rp_system
//! @{

/**
 * @brief Save data manager
 * @wfuname
 */
class RPSysSaveDataMgr : public IRPSysHostIOSocket {
    RP_SINGLETON_DECL_EX(RPSysSaveDataMgr);

public:
    /**
     * @brief Banner file type
     */
    enum EBannerType {
        EBannerType_Real,     //!< Writes to the "real" file (`banner.bin`)
        EBannerType_Temporary //!< Writes to the temporary file
                              //!< (`/tmp/banner.bin`)
    };

public:
    /**
     * @name Access save data
     */
    /**@{*/
    /**
     * @brief Gets the Sports Pack player data corresponding to the player index
     *
     * @param i Index into the player list
     * @return Copy of the player data
     */
    RPSportsPlayerData getSportsPlayerData(s32 i) const;
    /**
     * @brief Sets the Sports Pack player data corresponding to the player index
     *
     * @param rData Player data
     * @param i Index into the player list
     */
    void setSportsPlayerData(const RPSportsPlayerData& rData, s32 i);

    /**
     * @brief Gets the Sports Pack system data
     *
     * @return Copy of the system data
     */
    RPSportsSystemData getSportsSystemData() const;
    /**
     * @brief Sets the Sports Pack system data
     *
     * @param rData System data
     */
    void setSportsSystemData(const RPSportsSystemData& rData);

    /**
     * @brief Gets the Party Pack player data corresponding to the player index
     *
     * @param i Index into the player list
     * @return Copy of the player data
     */
    RPPartyPlayerData getPartyPlayerData(s32 i) const;
    /**
     * @brief Sets the Party Pack player data corresponding to the player index
     *
     * @param rData Player data
     * @param i Index into the player list
     */
    void setPartyPlayerData(const RPPartyPlayerData& rData, s32 i);

    /**
     * @brief Gets the Party Pack system data
     *
     * @return Copy of the system data
     */
    RPPartySystemData getPartySystemData() const;
    /**@}*/

    /**
     * @name Prepare save data
     */
    /**@{*/
    /**
     * @brief Checks which of the save data files (banner file, save file) exist
     * on the NAND
     *
     * @return Success
     */
    bool existPackFileSync();
    /**
     * @brief Asynchronously checks which of the save data files (banner file,
     * save file) exist on the NAND
     *
     * @return Success
     */
    bool existPackFileAsync();
    /**
     * @brief Asynchronously checks if the NAND has enough free space to store
     * both the banner and save files
     *
     * @return Success
     */
    bool existNandMemoryAsync();

    /**
     * @brief Asynchronously creates both the banner and save files on the NAND
     *
     * @return Success
     */
    bool createPackFileAsync();
    /**
     * @brief Deletes both the banner and save files from the NAND
     *
     * @return Success
     */
    bool deletePackFileSync();
    /**
     * @brief Saves both the banner and save files to the NAND
     *
     * @return Success
     */
    bool savePackFileSync();

    /**
     * @brief Asynchronously loads the save file from the NAND
     *
     * @return Success
     */
    bool loadAsync();
    /**
     * @brief Asynchronously saves both the banner and save files to the NAND
     *
     * @return Success
     */
    bool saveAsync();
    /**@}*/

    /**
     * @name State checking
     */
    /**@{*/
    /**
     * @brief Tests whether async operations have completed
     */
    bool isAsyncFinish() const;
    /**
     * @brief Tests whether NAND operations have completed
     */
    bool isNandAccessFinish() const;

    /**
     * @brief Tests whether both the banner and save files exist on the NAND
     */
    bool isPackFileExist() const;
    /**
     * @brief Tests whether the NAND has enough free space to store both the
     * banner and save files
     */
    bool isNandMemoryExist() const;

    /**
     * @brief Tests whether the save file is broken
     * @details The save file is broken if the file magic is broken or the
     * checksum is incorrect.
     */
    bool isSaveFileBroken() const;
    /**
     * @brief Tests whether any unrecoverable error occurred while loading the
     * save file
     * @typo
     */
    bool isErrorOccured() const;
    /**@}*/

    /**
     * @brief Toggles the ability to save both the banner and save files to the
     * NAND
     *
     * @param disable Whether to disable saving
     */
    void setSaveDisable(bool disable);

    /**
     * @name NAND
     */
    /**@{*/
    /**
     * @brief Gets the result of the last NAND memory check
     */
    u32 getNandCheckAnswer() const;
    /**
     * @brief Gets the result of the last NAND operation
     */
    s32 getErrorCode() const;

    /**
     * @brief Gets the number of free NAND blocks needed to store both the
     * banner and save files
     */
    u32 getUserBlockSize() const;
    /**
     * @brief Gets the number of free NAND inodes needed to store both the
     * banner and save files
     */
    u32 getSaveFileNum() const;

    /**
     * @brief Marks the current async NAND operations as completed
     */
    void finishNandAccess();
    /**@}*/

    /**
     * @brief Updates the save manager state
     *
     * @param checkContinue Whether to prompt the user to continue without
     * saving
     */
    void update(bool checkContinue);

    /**
     * @brief Initializes the banner file binary
     */
    void initBanner();

private:
    /**
     * @brief Bitflag indices
     */
    enum EFlag {
        EFlag_NandMemoryExist, //!< Enough NAND memory exists for both the
                               //!< banner file and save file
        EFlag_BannerFileExist, //!< The banner file exists on the NAND
        EFlag_SaveFileExist,   //!< The save file exists on the NAND
        EFlag_3,
        EFlag_DisableSave, //!< Saving is disabled
        EFlag_5,
        EFlag_6,
        EFlag_7,
        EFlag_NandAccess,     //!< NAND operation is happening on another thread
        EFlag_AsyncSave,      //!< Save file is being written on another thread
        EFlag_ContinueReject, //!< "Continue without saving?" prompt is
                              //!< currently displayed
        EFlag_11,
        EFlag_NandError, //!< Unrecoverable NAND error
        EFlag_12,
    };

private:
    /**
     * @brief Prompts the user to continue without saving
     */
    void checkContinueReject();

    /**
     * @brief Checks if the NAND has enough free space to store both the banner
     * and save files
     */
    void existNandMemorySync();

    /**
     * @brief Creates the banner file on the NAND
     * @details For safety purposes, the banner file is first created in the
     * temporary directory, before being moved out.
     */
    void createBannerFile();

    /**
     * @brief Saves the save file to the NAND
     * @details If saving is disabled, this function does nothing.
     */
    void saveSync();
    /**
     * @brief Loads the save file from the NAND
     */
    void loadSync();

    /**
     * @brief Saves the banner file to the NAND
     *
     * @param type Which type of banner file to save
     */
    void saveBannerFile(EBannerType type);

    /**
     * @name Thread functions
     */
    /**@{*/
    /**
     * @brief Checks which of the save data files (banner file, save file) exist
     * on the NAND
     */
    static void existPackFileFunc();
    /**
     * @brief Checks if the NAND has enough free space to store both the banner
     * and save files
     */
    static void existNandMemoryFunc();

    /**
     * @brief Creates both the banner and save files on the NAND
     */
    static void createPackFileFunc();

    /**
     * @brief Loads the save file from the NAND
     */
    static void loadDataFunc();
    /**
     * @brief Saves both the banner and save files to the NAND
     */
    static void saveDataFunc();
    /**@}*/

private:
    //! Status flags
    EGG::TBitFlag<u32> mFlags; // at 0x8

    //! Last NAND operation result
    s32 mNandErrorCode; // at 0xC
    //! @unused
    u32 WORD_0x10;
    //! NANDCheck result
    u32 mNandCheckAnswer; // at 0x14

    //! Save file (`RPSports.dat`) size
    u32 mSaveFileSize; // at 0x18
    //! Save banner (`banner.bin`) size
    u32 mBannerFileSize; // at 0x1C
    //! @brief Size of both the save file and banner together
    //! @note Size is aligned to the nearest NAND cluster (0x4000 bytes).
    u32 mTotalFileSize; // at 0x20

    //! Banner file binary
    void* mpBannerFile; // at 0x24
    //! Save file binary
    void* mpSaveFile; // at 0x28

    //! Game save data
    RPSysSaveData* mpSaveData; // at 0x2C
};

//! @}

#endif
