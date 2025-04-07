#ifndef RP_SPORTS_SEQUENCE_MGR_H
#define RP_SPORTS_SEQUENCE_MGR_H
#include "RPTypes.h"

/**
 * @brief Manages sequences, submodes of sports, and their interactions
 * @wfuname
 */
class RPSportsSequenceMgr {
    RP_SINGLETON_DECL(RPSportsSequenceMgr);

public:
    /**
     * @brief Sport ID
     */
    enum ESport {
        SPORT_BASEBALL,
        SPORT_TENNIS,
        SPORT_GOLF,
        SPORT_BOXING,
        SPORT_BOWLING,

        SPORT_MAX
    };

    /**
     * @brief Training game ID
     */
    enum ETrainingGame {
        TRAININGGAME_1ST,
        TRAININGGAME_2ND,
        TRAININGGAME_3RD,
    };

    /**
     * @brief Menu ID
     */
    enum EMenu { MENU_TRAINING, MENU_PHYSICAL, MENU_STANDARD };

    /**
     * @brief Golf gamemode
     */
    enum EGolfMode {
        GOLFMODE_NINEHOLE,
        GOLFMODE_BEGINNER,
        GOLFMODE_INTERMEDIATE,
        GOLFMODE_EXPERT
    };

public:
    ESport GetSport() const {
        return mSport;
    }
    EMenu GetLastMenu() const {
        return mLastMenu;
    }
    ETrainingGame GetTrainingGame() const {
        return mTrainingGame;
    }

    /**
     * @brief Reset data
     * @address 801e72d0
     */
    void Reset();

    /**
     * @brief Player has finished a training game this session
     * @note Value set seems to be unused
     * @address 801e73b0
     */
    void SetFinishedTraining();

    /**
     * @brief Get fitness event info
     * @param[out] sport Event sport
     * @param[out] game Event training game
     * @param event Event index
     * @address 801e73bc
     */
    void GetEventInfo(u32* sport, u32* game, u32 event);

    /**
     * @brief Get fitness event score
     * @address 801e73d8
     */
    u16 GetEventScore(u32 event);
    /**
     * @brief Set fitness event score
     * @address 801e73e8
     */
    void SetEventScore(u32 event, u16 score);

    /**
     * @brief Convert event sport/training game into 8-bit ID
     * @details (u8)(sport * 4 - sport + game)
     * @note Seems to be purposed for save data
     * @address 801e73f8
     */
    u8 MakeShortID(u32 sport, u32 game);

    /**
     * @brief Set event sports/games
     * @param[in] sports List of sports
     * @param[in] games List of training games
     * @address 801e740c
     */
    void SetAllEventInfo(const u32* sports, const u32* games);

    /**
     * @brief Fitness event callback
     * @details Controls whether to preview next event/go to results
     * @address 801e7440
     */
    void OnFinishEvent() const;

    /**
     * @brief Set current fitness event score (1-1000)
     * @address 801e746c
     */
    void SetCurrentEventScore(u16 score);

    /**
     * @brief Get golf score by player/hole
     * @address 801e7480
     */
    u32 GetGolfScore(u32 hole, u32 player) const;
    /**
     * @brief Set golf score by player/hole
     * @address 801e7498
     */
    void SetGolfScore(u32 stroke, u32 hole, u32 player);

private:
    // @brief Max player count
    static const u32 PLAYER_MAX = 4;
    // @brief Golf course length
    static const u32 HOLE_MAX = 9;
    // @brief Max fitness events
    static const u32 EVENT_MAX = 3;

    // @brief Current sport
    ESport mSport; // at 0x4
    // @brief Previous menu
    EMenu mLastMenu; // at 0x8
    // @brief Current training game
    ETrainingGame mTrainingGame; // at 0xC

    // @brief Final hole in golf game
    // @bug Accidentally(?) updated in RPGolConfig::CanPlayNextHole,
    // so it actually serves no purpose
    u32 mGolfFinalHole; // at 0x10
    // @brief Golf scores
    u32 mGolfScores[HOLE_MAX][PLAYER_MAX]; // at 0x14

    // @brief Scores for each Wii Fitness event (1-1000)
    u16 mEventScores[EVENT_MAX]; // at 0xA4

    // @brief Current golf gamemode
    EGolfMode mGolfMode; // at 0xAC

    // @brief Number of fitness events played
    u32 mNumFitnessEvents; // at 0xB0
    // @brief Sports played in each fitness event
    ESport mEventSports[EVENT_MAX]; // at 0xB4
    // @brief Training games played in each fitness event
    u32 mEventGames[EVENT_MAX]; // at 0xB8

    // @brief Player has finished a training game
    // @note Seems to be unused
    bool mHasFinishedTraining; // at 0xCC
};

#endif
