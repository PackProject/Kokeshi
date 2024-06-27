#ifndef RP_SPORTS_PLAYER_DATA_H
#define RP_SPORTS_PLAYER_DATA_H
#include <Pack/RPTypes.h>
#include <RVLFaceLib.h>
#include <egg/core.h>

//! @addtogroup rp_sports
//! @{

/**
 * @brief Wii Sports save file player data
 * @details The save file holds 100 of these (known collectively as the "player
 * list").
 */
class RPSportsPlayerData {
public:
    /**
     * @brief Sport
     * @note Unique from RPSysSceneCreator::ESportID, as this list swaps
     * the order of Boxing and Bowling.
     */
    enum ESportID {
        ESportID_Bsb, //!< Baseball
        ESportID_Tns, //!< Tennis
        ESportID_Gol, //!< Golf
        ESportID_Bow, //!< Bowling
        ESportID_Box, //!< Boxing

        ESportID_Max
    };

    /**
     * @brief Training game ID
     */
    enum EGameID {
        EGameID_1st, //!< 1st of the three training games
        EGameID_2nd, //!< 2nd of the three training games
        EGameID_3rd, //!< 3rd of the three training games

        EGameID_Max
    };

    /**
     * @brief Training game medal
     */
    enum EMedalID {
        EMedalID_None,
        EMedalID_Bronze,
        EMedalID_Silver,
        EMedalID_Gold,
        EMedalID_Platinum,
    };

    /**
     * @brief Golf difficulty
     */
    enum EGolfID {
        EGolfID_NineHole, //!< Nine-hole
        EGolfID_Beginner, //!< Beginner (1-3)
        EGolfID_Intermed, //!< Intermediate (4-6)
        EGolfID_Expert,   //!< Expert (7-9)

        EGolfID_Max
    };

    /**
     * @brief Fitness test statistic
     */
    enum EStatID {
        EStatID_Stamina,
        EStatID_Speed,
        EStatID_Balance,

        EStatID_Max
    };

    /**
     * @brief Handedness
     */
    enum EHandType {
        EHandType_Right, //!< Right-handed
        EHandType_Left,  //!< Left-handed
    };

public:
    /**
     * @brief Constructor
     */
    RPSportsPlayerData();

private:
    /**
     * @brief Player flag bit indices
     * @see mPlayerFlag
     */
    enum EPlayerFlag {
        EPlayerFlag_0,
        EPlayerFlag_Registered, //!< Registered on the player list
        EPlayerFlag_FirstPlay,  //!< Completed first round of any sport

        EPlayerFlag_Champion = 27, //!< Defeated the sport champion (+ ESportID)
    };

private:
    //! @brief Skill graph holds the previous 49 points
    //! @remark Considering the current point on the graph, that makes 50
    //! points in total.
    static const u32 scSkillGraphLength = 50 - 1;

    //! Fitness tests include three games
    static const u32 scPhysicalNumEvent = 3;

    /**
     * @name Common
     */
    /**@{*/
    //! Player flags
    u32 mPlayerFlags; // at 0x0
    //! Mii unique ID
    RFLCreateID mCreateID; // at 0x4
    //! Time player was added to the player list
    RPTime32 mDebutTime; // at 0xC

    //! @brief Handedness flags
    //! @note First five bits are for each sport's "primary" control, and the
    //! second five bits are for each sport's "secondary" control.
    u8 mHandFlags; // at 0x10

    //! Player skill levels
    f32 mSkillLevels[ESportID_Max]; // at 0x14

    //! @brief Nodes on the skill level graph
    //! @note The last point in the array is the newest point on the graph.
    //! @note Points are stored as vertical displacement from the previous
    //! point. If there is no previous point, the displacement is measured from
    //! the top of the graph.
    s16 mSkillGraphs[ESportID_Max][scSkillGraphLength]; // at 0x28

    //! First date playing the standard mode of any sport
    RPTime16 mStandardFirstDates[ESportID_Max]; // at 0x212
    //! First date playing the training games of any sport
    RPTime16 mTrainingFirstDates[ESportID_Max][EGameID_Max]; // at 0x21C

    //! @brief Play count of all sports
    //! @note For Golf, this counts the number of holes played
    u16 mPlayCounts[ESportID_Max]; // at 0x23A
    //! Play count of all training games
    u16 mTrainingPlayCounts[ESportID_Max][EGameID_Max]; // at 0x244

    //! Medals in all training games
    u8 mTrainingMedals[ESportID_Max][EGameID_Max]; // at 0x262
    /**@}*/

    /**
     * @name Tennis
     */
    /**@{*/
    //! @unused
    u16 SHORT_0x272;

    //! CPU difficulty
    f32 mTnsDifficulty; // at 0x274

    //! @brief Player success coefficient
    //! @details Increases more with convincing victories, and allows CPU
    //! difficulty to scale faster.
    f32 mTnsSuccess; // at 0x278

    //! Tennis training best scores
    u8 mTnsTrainingBests[EGameID_Max]; // at 0x27C
    /**@}*/

    /**
     * @name Baseball
     */
    /**@{*/
    //! Best home run count in Hitting Home Runs
    u8 mBsbHomerunRunsBest; // at 0x27F
    //! Best total distance in Hitting Home Runs
    u16 mBsbHomerunDistBest; // at 0x280

    //! Swing Control best score
    u8 mBsbUchiwakeBest; // at 0x282
    //! Batting Practice best score
    u8 mBsbRenzokuBest; // at 0x283
    /**@}*/

    /**
     * @name Bowling
     */
    /**@{*/
    //! @unused
    u16 SHORT_0x284;

    //! Bowling best score
    u16 mBowStandardBest; // at 0x286
    //! Number of perfect games bowled
    u8 mBowPerfectCount; // at 0x288
    //! Best scores in Bowling training games
    u16 mBowTrainingBests[EGameID_Max]; // at 0x28A
    /**@}*/

    /**
     * @name Golf
     */
    /**@{*/
    //! Best scores on each difficulty
    s8 mGolStandardBests[EGolfID_Max]; // at 0x290
    //! Number of aces hit
    u8 mGolAceCount; // at 0x294
    //! Best scores in Golf training games
    u16 mGolTrainingRecords[EGameID_Max]; // at 0x296
    /**@}*/

    /**
     * @name Boxing
     */
    /**@{*/
    //! Best scores in Boxing training games
    u16 mBoxTrainingBests[EGameID_Max]; // at 0x29C
    /**@}*/

    /**
     * @name Wii Fitness
     */
    /**@{*/
    //! Date of the first completed fitness test
    RPTime16 mPhysicalEndFirstDate; // at 0x2A2
    //! Number of completed fitness tests
    u16 mPhysicalEndCount; // at 0x2A4

    //! Last fitness test stat ratings
    u16 mPhysicalLastStats[EStatID_Max]; // at 0x2A6
    //! Last fitness test games
    u8 mPhysicalLastEvents[scPhysicalNumEvent]; // at 0x2AC

    //! Best fitness stat sum in all training games
    u16 mPhysicalBestStatSums[ESportID_Max][EGameID_Max]; // at 0x2B0

    //! Date of the last completed fitness test
    RPTime16 mPhysicalEndLastDate; // at 0x2CE
    //! Fitness stat sums of the most recent fitness test, and the previous 90
    u16 mPhysicalHistoryStatSums[90 + 1];
    //! Dates of the most recent fitness test, and the previous 90
    RPTime16 mPhysicalHistoryDates[90 + 1];

    //! Date of the last started fitness test
    RPTime16 mPhysicalBeginLastDate; // at 0x43C

    //! @unused
    u16 SHORTS_0x43E[15];
    /**@}*/
};

//! @}

#endif
