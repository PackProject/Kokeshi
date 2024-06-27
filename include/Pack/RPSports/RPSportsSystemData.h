#ifndef RP_SPORTS_SYSTEM_DATA_H
#define RP_SPORTS_SYSTEM_DATA_H
#include <Pack/RPTypes.h>
#include <egg/core.h>
#include <revolution/WPAD.h>

//! @addtogroup rp_sports
//! @{

/**
 * @brief Wii Sports save file common data
 */
class RPSportsSystemData {
public:
    /**
     * @brief Training game ID
     */
    enum EGameTr {
        EGameTr_Box_Mituchi,  //!< Throwing Punches
        EGameTr_Box_Tamayoke, //!< Dodging
        EGameTr_Box_Sandbag,  //!< Working the Bag

        EGameTr_Gol_Target,  //!< Target Practice
        EGameTr_Gol_Nearpin, //!< Hitting the Green
        EGameTr_Gol_Onepat,  //!< Putting

        EGameTr_Bow_Gatetoshi,  //!< Spin Control
        EGameTr_Bow_Nagitaoshi, //!< Power Throws
        EGameTr_Bow_Spareget,   //!< Picking Up Spares

        EGameTr_Bsb_Renzoku,  //!< Batting Practice
        EGameTr_Bsb_Uchiwake, //!< Swing Control
        EGameTr_Bsb_Homerun,  //!< Hitting Home Runs

        EGameTr_Tns_Kabeuchi,  //!< Target Practice
        EGameTr_Tns_Nerauchii, //!< Timing Your Swing
        EGameTr_Tns_Renzoku,   //!< Returning Balls
    };

public:
    /**
     * @brief Constructor
     */
    RPSportsSystemData();

    /**
     * @brief Clears all data
     */
    void reset();

    /**
     * @brief Gets the old data for the specified player count and ID
     *
     * @param[out] pIndex Where the Mii index will be stored
     * @param[out] pAddr Where the remote address will be stored
     * @param numPlayers Player count (zero-indexed)
     * @param player Player index
     */
    void getOldData(s8* pIndex, u8* pAddr, s32 numPlayers, s32 player) const;
    /**
     * @brief Sets the old data for the specified player count and ID
     *
     * @param index Mii index (official database)
     * @param pAddr Remote address
     * @param numPlayers Player count (zero-indexed)
     * @param player Player index
     */
    void setOldData(s8 index, const u8* pAddr, s32 numPlayers, s32 player);

    /**
     * @brief Sets the total number of fitness tests taken
     *
     * @param count Number of fitness tests taken
     */
    void setPhysicalCount(u8 count);
    /**
     * @brief Gets the total number of fitness tests taken
     */
    u8 getPhysicalCount() const;

    /**
     * @brief Sets the last date on which a fitness test was taken
     *
     * @param date Date of last fitness test
     */
    void setPhysicalLastDate(RPTime16 date);
    /**
     * @brief Gets the last date on which a fitness test was taken
     */
    RPTime16 getPhysicalLastDate() const;

    /**
     * @brief Sets whether the specified training game's opening cutscene has
     * played
     *
     * @param game Training game (@ref EGameTr)
     * @param demo Whether the cutscene has played
     */
    void setGameOpenDemo(u8 game, bool demo);
    /**
     * @brief Tests whether the specified training game's opening cutscene has
     * played
     *
     * @param game Training game (@ref EGameTr)
     */
    void isGameOpenDemo(u8 game) const;

    /**
     * @brief Sets whether the specified training game is available
     *
     * @param game Training game (@ref EGameTr)
     * @param open Whether the game is available
     */
    void setGameOpen(u8 game, bool open);
    /**
     * @brief Tests whether the specified training game is available
     *
     * @param game Training game (@ref EGameTr)
     */
    void isGameOpen(u8 game) const;

    /**
     * @brief Deserializes data from a memory buffer
     *
     * @param rStrm Memory stream
     */
    void read(EGG::RamStream& rStrm);
    /**
     * @brief Serializes data to memory buffer
     *
     * @param rStrm Memory stream
     */
    void write(EGG::RamStream& rStrm) const;

private:
    /**
     * @name Old data
     * @brief Data from previous play session
     * @note Contains data for each possible amount of players.
     */
    /**@{*/
    //! Official database index
    s8 mOldMiiIndex[1 + 2 + 3 + 4]; // at 0x0
    //! Remote address
    u8 mOldRemoteAddr[1 + 2 + 3 + 4][WPAD_ADDR_LEN]; // at 0xA
    /**@}*/

    //! Total number of fitness tests taken
    u8 mPhysicalCount; // at 0x46
    //! Last fitness test taken by any Mii
    RPTime16 mPhysicalLastDate; // at 0x48

    //! @unused
    u16 SHORT_0x4A;
    //! @unused
    u8 BYTE_0x4C;

    //! @brief Training game bitflags
    //! @details The upper half holds the open demo flags, while the lower half
    //! holds the open flags.
    u32 mTrainingFlags; // at 0x50

    //! @unused
    u32 WORD_0x54;
};

//! @}

#endif
