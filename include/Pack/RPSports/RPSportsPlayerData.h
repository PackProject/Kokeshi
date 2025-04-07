#ifndef RP_SPORTS_PLAYER_DATA_H
#define RP_SPORTS_PLAYER_DATA_H
#include <Pack/RPTypes.h>

#include <egg/core.h>

#include <RVLFaceLib.h>

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
     * @brief Golf course
     */
    enum ECourseID {
        ECourseID_NineHole, //!< Nine-hole
        ECourseID_Beginner, //!< Beginner (1-3)
        ECourseID_Intermed, //!< Intermediate (4-6)
        ECourseID_Expert,   //!< Expert (7-9)

        ECourseID_Max
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

    /**
     * @brief Clears all data
     */
    void reset();

    /**
     * @name Common
     */
    /**@{*/
    /**
     * @brief Gets the hand type used for the specified sport's primary control
     *
     * @param sport Sport ID
     */
    EHandType getHandPrimary(ESportID sport) const;
    /**
     * @brief Gets the hand type used for the specified sport's secondary
     * control
     *
     * @param sport Sport ID
     */
    EHandType getHandSecondary(ESportID sport) const;

    /**
     * @brief Sets the hand type used for the specified sport's primary control
     *
     * @param sport Sport ID
     * @param hand Hand type
     */
    void setHandPrimary(ESportID sport, EHandType hand);
    /**
     * @brief Gets the hand type used for the specified sport's secondary
     * control
     *
     * @param sport Sport ID
     * @param hand Hand type
     */
    void setHandSecondary(ESportID sport, EHandType hand);

    /**
     * @brief Gets the skill level associated with this sport
     *
     * @param sport Sport ID
     */
    f32 getSkillLevel(ESportID sport) const;
    /**
     * @brief Sets the skill level associated with this sport
     *
     * @param sport Sport ID
     * @param skill Skill level
     */
    void setSkillLevel(ESportID sport, f32 skill);

    /**
     * @brief Gets the specified point from the specified sport's skill graph
     *
     * @param sport Sport ID
     * @param point Point
     */
    s16 getSkillGraphPoint(ESportID sport, u32 point) const;
    /**
     * @brief Sets the specified point from the specified sport's skill graph
     *
     * @param value Point value
     * @param sport Sport ID
     * @param point Point
     */
    void setSkillGraphPoint(s16 value, ESportID sport, u32 point);

    /**
     * @brief Gets the first date when the standard mode of the specified sport
     * was played
     *
     * @param sport Sport ID
     */
    RPTime16 getStandardFirstDate(ESportID sport) const;
    /**
     * @brief Sets the first date when the standard mode of the specified sport
     * was played
     *
     * @param date First play date
     * @param sport Sport ID
     */
    void setStandardFirstDate(RPTime16 date, ESportID sport);

    /**
     * @brief Gets the first date when the specified training game was played
     *
     * @param sport Sport ID
     * @param game Training game
     */
    RPTime16 getTrainingFirstDate(ESportID sport, EGameID game) const;
    /**
     * @brief Sets the first date when the specified training game was played
     *
     * @param date First play date
     * @param sport Sport ID
     * @param game Training game
     */
    void setTrainingFirstDate(RPTime16 date, ESportID sport, EGameID game);

    /**
     * @brief Gets the number of times the standard mode of the specified sport
     * was played
     *
     * @param sport Sport ID
     */
    u16 getStandardPlayCount(ESportID sport) const;
    /**
     * @brief Sets the number of times the standard mode of the specified sport
     * was played
     *
     * @param count Play count
     * @param sport Sport ID
     */
    void setStandardPlayCount(u16 count, ESportID sport);

    /**
     * @brief Gets the number of times the specified training game was played
     *
     * @param sport Sport ID
     * @param game Training game
     */
    u16 getTrainingPlayCount(ESportID sport, EGameID game) const;
    /**
     * @brief Sets the number of times the specified training game was played
     *
     * @param count Play count
     * @param sport Sport ID
     * @param game Training game
     */
    void setTrainingPlayCount(u16 count, ESportID sport, EGameID game);

    /**
     * @brief Gets the medal earned in the specified training game
     *
     * @param sport Sport ID
     * @param game Training game
     * @return @ref EMedalID
     */
    u8 getTrainingMedal(ESportID sport, EGameID game) const;
    /**
     * @brief Sets the medal earned in the specified training game
     *
     * @param medal Medal type (@ref EMedalID)
     * @param sport Sport ID
     * @param game Training game
     */
    void setTrainingMedal(u8 medal, ESportID sport, EGameID game);

    /**
     * @brief Tests whether this player has played any sport
     */
    bool isFirstPlay() const;
    /**
     * @brief Marks that this player has played a sport
     */
    void setFirstPlay();

    /**
     * @brief Tests whether this player has defeated the specified sport's
     * champion
     *
     * @param sport Sport ID
     */
    bool isDefeatChampion(ESportID sport) const;
    /**
     * @brief Marks that this player has defeated the specified sport's champion
     *
     * @param sport Sport ID
     */
    bool setDefeatChampion(ESportID sport);
    /**@}*/

    /**
     * @name Tennis
     */
    /**@{*/
    /**
     * @brief Gets the difficulty of the Tennis CPU players
     */
    f32 getTnsDifficulty() const;
    /**
     * @brief Sets the difficulty of the Tennis CPU players
     *
     * @param difficulty CPU difficulty
     */
    void setTnsDifficulty(f32 difficulty);

    /**
     * @brief Gets the Tennis success factor
     */
    f32 getTnsSuccess() const;
    /**
     * @brief Sets the Tennis success factor
     *
     * @param success Success factor
     */
    void setTnsSuccess(f32 success);

    /**
     * @brief Gets the best score in the specified Tennis training game
     *
     * @param game Training game
     */
    u8 getTnsTrainingBest(EGameID game) const;
    /**
     * @brief Sets the best score in the specified Tennis training game
     *
     * @param score Best score
     * @param game Training game
     */
    void setTnsTrainingBest(u8 score, EGameID game);
    /**@}*/

    /**
     * @name Baseball
     */
    /**@{*/
    /**
     * @brief Gets the best home run count record in Hitting Home Runs
     */
    u8 getBsbHomerunRunsBest() const;
    /**
     * @brief Sets the best home run count record in Hitting Home Runs
     *
     * @param runs Best home run count
     */
    void setBsbHomerunRunsBest(u8 runs);

    /**
     * @brief Gets the best total distance record in Hitting Home Runs
     */
    u16 getBsbHomerunDistBest() const;
    /**
     * @brief Sets the best total distance record in Hitting Home Runs
     *
     * @param dist Best total distance
     */
    void setBsbHomerunDistBest(u16 dist);

    /**
     * @brief Gets the best score in Swing Control
     */
    u8 getBsbUchiwakeBest() const;
    /**
     * @brief Sets the best score in Swing Control
     *
     * @param score Best score
     */
    void setBsbUchiwakeBest(u8 score);

    /**
     * @brief Gets the best score in Batting Practice
     */
    u8 getBsbRenzokuBest() const;
    /**
     * @brief Sets the best score in Batting Practice
     *
     * @param score Best score
     */
    void setBsbRenzokuBest(u8 score);
    /**@}*/

    /**
     * @name Bowling
     */
    /**@{*/
    /**
     * @brief Gets the best score in Bowling
     */
    u16 getBowStandardBest() const;
    /**
     * @brief Sets the best score in Bowling
     *
     * @param score Best score
     */
    void setBowStandardBest(u16 score);

    /**
     * @brief Gets the number of perfect games bowled
     */
    u8 getBowPerfectCount() const;
    /**
     * @brief Sets the number of perfect games bowled
     *
     * @param count Perfect game count
     */
    void setBowPerfectCount(u8 count);

    /**
     * @brief Gets the best score in the specified Bowling training game
     *
     * @param game Training game
     */
    u16 getBowTrainingBest(EGameID game) const;
    /**
     * @brief Sets the best score in the specified Bowling training game
     *
     * @param score Best score
     * @param game Training game
     */
    void setBowTrainingBest(u16 score, EGameID game);
    /**@}*/

    /**
     * @name Golf
     */
    /**@{*/
    /**
     * @brief Gets the best Golf score on the specified course
     *
     * @param course Golf course
     */
    s8 getGolStandardBest(ECourseID course) const;
    /**
     * @brief Sets the best Golf score on the specified course
     *
     * @param score Best score
     * @param course Golf course
     */
    void setGolStandardBest(s8 score, ECourseID course);

    /**
     * @brief Gets the amount of hole-in-ones hit
     */
    u8 getGolAceCount() const;
    /**
     * @brief Sets the amount of hole-in-ones hit
     *
     * @param count Hole-in-one count
     */
    void setGolAceCount(u8 count);

    /**
     * @brief Gets the best score in the specified Golf training game
     *
     * @param game Training game
     */
    u16 getGolTrainingBest(EGameID game) const;
    /**
     * @brief Set the best score in the specified Golf training game
     *
     * @param score Best score
     * @param game Training game
     */
    void setGolTrainingBest(u16 score, EGameID game);
    /**@}*/

    /**
     * @name Boxing
     */
    /**@{*/
    /**
     * @brief Gets the best score in the specified Boxing training game
     *
     * @param game Training game
     */
    u16 getBoxTrainingBest(EGameID game) const;
    /**
     * @brief Set the best score in the specified Boxing training game
     *
     * @param score Best score
     * @param game Training game
     */
    void setBoxTrainingBest(u16 score, EGameID game);
    /**@}*/

    /**
     * @name Wii Fitness
     */
    /**@{*/
    /**@}*/

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
    //! Fitness test history includes the most recent test and the previous 90
    static const u32 scPhysicalHistoryLength = 90 + 1;

    /**
     * @name Common data
     */
    /**@{*/
    //! Player flags
    EGG::TBitFlag<u32> mPlayerFlags; // at 0x0
    //! Mii unique ID
    RFLCreateID mCreateID; // at 0x4
    //! Time player was added to the player list
    RPTime32 mDebutTime; // at 0xC

    //! @brief Handedness flags
    //! @note First five bits are for each sport's "primary" control, and the
    //! second five bits are for each sport's "secondary" control.
    EGG::TBitFlag<u8> mHandFlags; // at 0x10

    //! Player skill levels
    f32 mSkillLevels[ESportID_Max]; // at 0x14

    //! @brief Nodes on the skill level graph
    //! @note The last point in the array is the newest point on the graph.
    //! @note Points are stored as vertical displacement from the previous
    //! point. If there is no previous point, the displacement is measured from
    //! the top of the graph.
    s16 mSkillGraphPoints[ESportID_Max][scSkillGraphLength]; // at 0x28

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
     * @name Tennis data
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
     * @name Baseball data
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
     * @name Bowling data
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
     * @name Golf data
     */
    /**@{*/
    //! Best scores on each difficulty
    s8 mGolStandardBests[ECourseID_Max]; // at 0x290
    //! Number of aces hit
    u8 mGolAceCount; // at 0x294
    //! Best scores in Golf training games
    u16 mGolTrainingRecords[EGameID_Max]; // at 0x296
    /**@}*/

    /**
     * @name Boxing data
     */
    /**@{*/
    //! Best scores in Boxing training games
    u16 mBoxTrainingBests[EGameID_Max]; // at 0x29C
    /**@}*/

    /**
     * @name Wii Fitness data
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
    u16 mPhysicalStatSumBests[ESportID_Max][EGameID_Max]; // at 0x2B0

    //! Date of the last completed fitness test
    RPTime16 mPhysicalEndLastDate; // at 0x2CE
    //! Fitness stat sums of the most recent fitness test, and the previous 90
    u16 mPhysicalStatSumHistory[scPhysicalHistoryLength];
    //! Dates of the most recent fitness test, and the previous 90
    RPTime16 mPhysicalDateHistory[scPhysicalHistoryLength];

    //! Date of the last started fitness test
    RPTime16 mPhysicalBeginLastDate; // at 0x43C

    //! @unused
    u16 SHORTS_0x43E[15];
    /**@}*/
};

//! @}

#endif
