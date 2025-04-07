#ifndef RP_SPORTS_GOL_CONFIG_H
#define RP_SPORTS_GOL_CONFIG_H
#include "RPSysUnknownBase.h"
#include "RPTypes.h"

/**
 * @brief Global settings used by the golf scene
 * @wscname
 */
class RPGolConfig : RPSysUnknownBase {
    RP_SINGLETON_DECL(RPGolConfig);

public:
    /**
     * @brief Configuration for a golf hole
     * @wscname
     */
    struct HoleInfo {
        const char* name; // at 0x0
        u32 par;          // at 0x4

        /**
         * @see EHoleResource
         * @details Only one resource can be used as they are not flags.
         */
        u32 resource; // at 0x8

        f32 FLOAT_0xC;
        f32 FLOAT_0x10;
        f32 FLOAT_0x14;
        f32 FLOAT_0x18;
        f32 FLOAT_0x1C;
        f32 FLOAT_0x20;
    };

    // @brief Optional resources that can be requested for a hole
    enum EHoleResource {
        RES_NONE,
        RES_RIVER_SFX,
        RES_SEA_SFX,
        RES_SKY_REFLECT
    };

    // @brief Golf gamemode
    enum EGameMode {
        GAMEMODE_0,
        GAMEMODE_1,
        GAMEMODE_2,

        GAMEMODE_TR_ONEPAT,
        GAMEMODE_TR_NEARPIN,
        GAMEMODE_TR_TARGET,

        GAMEMODE_FT_ONEPAT,
        GAMEMODE_FT_NEARPIN,
        GAMEMODE_FT_TARGET,

        GAMEMODE_NINEHOLE,

        GAMEMODE_BEGINNER,
        GAMEMODE_INTERMEDIATE,
        GAMEMODE_EXPERT,
    };

public:
    /**
     * @brief Number of holes played so far
     * @address 8029cb1c
     */
    static int GetNumHolesPlayed();

    /**
     * @brief Check if the current hole is the first hole of the gamemode
     * @address 8029cb74
     */
    static bool IsFirstHole();

    /**
     * @brief Player's score on specified hole
     * @address 8029cbd4
     * @param hole Hole id (Nth hole played in round, not hole N)
     * @param player Player id
     */
    static int GetNthHoleScore(u32 hole, u32 player);

    /**
     * @brief Player's stroke count on specified hole
     * @address 8029cc54
     * @param hole Hole id (Nth hole played in round, not hole N)
     * @param player Player id
     */
    static int GetNthHoleStrokes(u32 hole, u32 player);

    /**
     * @brief Player's total score so far
     * @details Caller can optionally include the current hole in the
     * calculation, which is used for the result screen as the hole is not
     * incremented after finishing the round
     * @address 8029ccbc
     * @param player Player id
     * @param includeCurr Include the current hole in the calculation (for
     * results)
     */
    static int GetTotalScore(u32 player, bool includeCurr);

    /**
     * @brief ID of the Nth hole of the round
     * @address 8029ce04
     */
    static int GetRelativeHole(int n);

    /**
     * @brief Number of holes that will be played
     * @address 8029ce58
     */
    static int GetNumHolesTotal();

    /**
     * @brief Creates the wind set based on what holes are required by the
     * gamemode
     * @address 8029cea8
     */
    void CreateWindForGamemode();

    /**
     * @brief Update player scores
     * @details Scores taken from golf player manager
     * @address 8029cf24
     */
    void UpdateScores();

    /**
     * @brief Check if the round is not over yet
     * @address 8029cff0
     */
    bool CanPlayNextHole() const;

    /**
     * @brief Current hole ID, one-indexed for UI
     * @address 8029d0b8
     */
    static int GetUIHoleNum();

    /**
     * @brief Choose random pin position
     * @address 8029d100
     */
    void ChoosePinPos();

    // FUN_8029d238

    // FUN_8029d2c4

    /**
     * @brief Check if the current gamemode is training or fitness, not standard
     * @address 8029d3ac
     */
    static bool IsNotStandardGame();

    /**
     * @brief Check if the current hole needs `sky2`, the mirrored skybox for
     * reflections
     * @address 8029d438
     */
    static bool IsSkyReflect();

    /**
     * @brief Check if the current hole uses the sea environment sfx
     * (`SE_LV_Env2_Sea`)
     * @address 8029d464
     */
    static bool IsUseSeaSfx();

    /**
     * @brief Check if the current hole uses the river environment sfx
     * (`SE_LV_Env2_River`)
     * @address 8029d490
     */
    static bool IsUseRiverSfx();

    // @address 8029d4bc
    void SetupStandardGame();
    // @address 8029d6e0
    void SetupFitnessGame();
    // @address 8029d778
    void SetupTrainingGame();
    // @address 8029d810
    void SetupGame();

    // FUN_8029d86c()

    // FUN_8029d960()

    // FUN_8029d970()

    /**
     * @brief CARC name of the current hole
     * @address 8029d978
     */
    static const char* GetCurrentHoleName();

    // FUN_8029d994()

    /**
     * @brief Create wind set from settings
     * @address 8029cea8
     */
    static void CreateWind(int startHole, int endHole, int minWind,
                           int maxWind);

    /**
     * @brief Create a random, exclusive sequence between 0 and `max`
     * @address 8029dea8
     * @param max Sequence max value
     * @param array Output array
     */
    static void CreateRandomSeq(int max, int* array);

    /**
     * @brief Lookup hole number by name
     * @address 8029e084
     * @param name Hole name (ex: "fc1")
     */
    static int GetHoleNumFromName(const char* name);

    /**
     * @brief Gets the explicit "goal" position for auto-aiming.
     * @details If the position is (0,0,0), this falls back to the field
     * manager.
     */
    static const nw4r::math::VEC3& GetGoalPos();

    /**
     * @brief Adjusts the minimap zoom level
     *
     * @param[in,out] rZoom Zoom level
     */
    static void AdjustMapZoom(f32& zoom);

    /**
     * @brief Adjusts the minimap position
     *
     * @param[in,out] rPos Minimap position
     */
    static void AdjustMapPos(nw4r::math::VEC2& pos);

    /**
     * @brief Gets the number of holes played in the round
     */
    static s32 GetHolesPlayed();

    /**
     * @brief Tests whether the current hole is the first hole of the current
     * round
     */
    static bool IsStartHole();

    /**
     * @brief Gets the stroke score on the specified hole, by the specified
     * player
     *
     * @param hole Hole ID (relative)
     * @param player Player ID
     */
    static s32 GetNthScore(u32 hole, u32 player);

    /**
     * @brief Gets the stroke count on the specified hole, by the specified
     * player
     *
     * @param hole Hole ID (relative)
     * @param player Player ID
     */
    static u32 GetNthTotal(u32 hole, u32 player);

    /**
     * @brief Gets the total stroke score by the specified player
     *
     * @param player Player ID
     * @param finish Count the current hole (for a finished round)
     */
    static s32 GetRoundScore(u32 player, bool finish);

    /**
     * @brief Gets the absolute hole index based on the specified relative index
     *
     * @param no Hole index (relative)
     */
    static u32 GetAbsoluteHole(u32 no);

    /**
     * @brief Gets the number of holes in the round
     */
    static u32 GetRoundLength();

    /**
     * @brief Gets the wind speed/direction of the current hole
     *
     * @param[out] pDir Wind direction
     * @param[out] pSpd Wind speed
     */
    void GetWind(u32* pDir, u32* pSpd) const;

    /**
     * @brief Gets the stroke par of the specified hole
     *
     * @param hole Hole ID
     */
    static u32 GetHolePar(u32 hole);
    /**
     * @brief Gets the stroke par of the current hole
     */
    static u32 GetPar();

    /**
     * @brief Generates a random permutation of integer values
     *
     * @param max Maximum value (exclusive)
     * @param[out] pArray Permutation
     */
    static void Random(s32 max, s32* pArray);

    /**
     * @brief Gets the hole ID corresponding to the specified resource filename
     *
     * @param pName Hole resource filename
     */
    static u32 GetHoleNo(const char* pName);

    /**
     * @brief Creates a set of wind conditions
     *
     * @param start Start hole
     * @param end End hole
     * @param min Minimum wind speed (inclusive)
     * @param max Maximum wind speed (inclusive)
     */
    static void DecideWindImpl(u32 start, u32 end, u32 min, u32 max);

    /**
     * @brief Gets the resource filename of the current hole
     */
    static const char* GetHoleName();

    /**
     * @brief Tests whether the current hole requires the river environment
     * sounds
     */
    static bool IsHoleFlagRiverSE();
    /**
     * @brief Tests whether the current hole requires the sea environment sounds
     */
    static bool IsHoleFlagSeaSE();
    /**
     * @brief Tests whether the current hole requires the reflected sky model
     */
    static bool IsHoleFlagSkyReflect();

    /**
     * @brief Tests whether expert mode (UI hidden) is enabled
     */
    static bool IsExpertMode();

private:
    // @brief Max player count
    static const u32 PLAYER_MAX = 4;
    // @brief Golf course length
    static const u32 HOLE_MAX = 9;

    u32 mStrokeTotal[HOLE_MAX][PLAYER_MAX]; // at 0x4
    u32 mGameMode;                          // at 0x94
    u32 mCurrentHole;                       // at 0x98
    char UNK_0x9C[0x1C74 - 0x9C];

    /**
     * @brief Hole info for the 9 holes plus a few unused courses
     * @address 803c7aa0
     */
    static HoleInfo sHoleInfo[];

    static bool sIsExpertMode;
};

#endif
