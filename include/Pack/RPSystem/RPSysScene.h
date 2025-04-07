#ifndef RP_SYSTEM_SCENE_H
#define RP_SYSTEM_SCENE_H
#include <Pack/RPGraphics.h>
#include <Pack/RPTypes.h>

#include <egg/core.h>
#include <egg/math.h>

#include <nw4r/ut.h>

// Forward declarations
class RPSysCommonObject;
class RPSysEffectCreator;

//! @addtogroup rp_system
//! @{

/**
 * @brief Pack Project base scene
 * @wfuname
 *
 * @details Common between all Pack Project games.
 */
class RPSysScene : public EGG::Scene, public IRPGrpDrawObject {
public:
#if defined(PACK_RESORT)
    /**
     * @brief Scene type
     * @ws2
     */
    enum EKind{
        EKind_System = 'SYS_',   //!< RPSysScene
        EKind_Base = 'BASE',     //!< RPSportsBaseScene
        EKind_Game = 'GAME',     //!< RPSportsGameScene
        EKind_Sequence = 'SEQ_', //!< RPSportsSeqScene
    };

    /**
     * @brief Island time
     * @ws2
     */
    enum ETime{
        ETime_Day,     //!< Force daytime
        ETime_Evening, //!< Force evening
        ETime_Night,   //!< Force night
        ETime_Auto,    //!< Auto-detect from the current sequence
    };
#endif

    /**
     * @brief Unknown static scene structure
     */
    struct UnkStruct {
        u32 _00;

        /**
         * @brief Constructor
         */
        UnkStruct() : _00(0) {}
    };

public:
    /**
     * @brief Constructor
     */
    RPSysScene();
    /**
     * @brief Destructor
     */
    virtual ~RPSysScene() {} // at 0x8

    /**
     * @name Internal events
     * @brief These event functions are internal and should not be overriden.
     */
    /**@{*/
    /**
     * @brief Updates the scene state
     */
    virtual void calc(); // at 0xC
    /**
     * @brief Renders the scene state
     */
    virtual void draw(); // at 0x10

    /**
     * @brief Enters the scene
     */
    virtual void enter(); // at 0x14
    /**
     * @brief Exits the scene
     */
    virtual void exit(); // at 0x18

    /**
     * @brief Re-initializes the scene
     */
    virtual void reinit(); // at 0x1C

    /**
     * @brief Prepares for destruction of (and transition from) a child scene
     */
    virtual void incoming_childDestroy(); // at 0x20
    /**
     * @brief Prepares for creation of (and transition to) a child scene
     */
    virtual void outgoing_childCreate(); // at 0x24
    /**@}*/

    /**
     * @name User events
     * @brief These event functions are mostly empty stubs and should be
     * overriden to provide scene functionality.
     */
    /**@{*/
    /**
     * @brief Accesses an unknown, unused structure
     * @unused
     */
    virtual UnkStruct& VF_0x3C() {
        return sUnkStruct;
    } // at 0x3C

#if defined(PACK_RESORT)
    /**
     * @brief Gets the scene's class type
     * @ws2
     */
    virtual EKind getKind() {
        return EKind_System;
    }
#endif

    /**
     * @brief Pause callback
     *
     * @param enter Whether the pause menu is being entered
     */
    virtual void pauseCallBack(bool enter); // at 0x40

#if defined(PACK_RESORT)
    /**
     * @brief Gets the scene's island time of day
     * @ws2
     */
    virtual ETime getIslandTime();

    /**
     * @brief Enters the scene
     */
    virtual void Enter() {}
#endif

    /**
     * @brief Initializes the scene's state for the first time
     */
    virtual void Configure() {} // at 0x44
    /**
     * @brief Loads the scene's required assets
     */
    virtual void LoadResource() {} // at 0x48
    /**
     * @brief Re-initializes the scene's state
     */
    virtual void Reset() {} // at 0x4C
    /**
     * @brief Updates the scene's state
     */
    virtual void Calculate() {} // at 0x50
    /**
     * @brief Exits the scene
     */
    virtual void Exit() {} // at 0x54

#if defined(PACK_RESORT)
    /**
     * @brief Updates the scene's MotionPlus state
     * @ws2
     */
    virtual void CalculateMotionPlus() {}
#endif

    /**
     * @brief Runs an asynchronous task on the DVD thread
     */
    virtual void taskAsync() {} // at 0x58
    /**@}*/

    /**
     * @brief Initializes the scene's members
     */
    void init();

    /**
     * @brief Tests whether the scene is currently fully visible
     */
    bool isSceneDisplay() const;
    /**
     * @brief Tests whether the scene is currently not visible
     */
    bool isSceneBlank() const;
    /**
     * @brief Tests whether the scene is allowed to change
     */
    bool isSceneChangeEnable() const;
    /**
     * @brief Tests whether the scene is allowing NAND access
     */
    bool isNandAccessEnable() const;

    /**
     * @brief Sets the current frame of all faders
     *
     * @param frame Frame count
     */
    void setFadeFrame(u16 frame);

    /**
     * @brief Gets the fader color for scene changes
     */
    nw4r::ut::Color getFadeColor() const;
    /**
     * @brief Sets the fader color for scene changes
     *
     * @param color Fade color
     */
    void setFadeColor(nw4r::ut::Color color);
    /**
     * @brief Set the scene's fader color
     *
     * @param color Fade color
     */
    void setSceneFadeColor(nw4r::ut::Color color);

    /**
     * @brief Begins fading in the scene's fader
     */
    void startFadeIn();
    /**
     * @brief Begins fading out the scene's fader
     */
    void startFadeOut();

    /**
     * @brief Turns on/off screen dimming
     * @note If "Screen Saver Mode" is disabled, dimming is always OFF.
     *
     * @param dim Whether to dim the screen
     */
    void setDimming(BOOL dim);

    /**
     * @brief Sets the ID of the previously created scene
     *
     * @param id Previous scene ID
     */
    void setCreatorSceneID(s32 id);
    /**
     * @brief Gets the ID of the previously created scene
     */
    s32 getCreatorSceneID() const;

    /**
     * @brief Sets the fast entry scene flag
     * @remark Doing this skips loading assets, running asynchronous tasks, and
     * fading in.
     */
    void setEnterFast();

    /**
     * @name RPSysLoadScene
     * @brief These functions are defined in `RPSysLoadScene.cpp`.
     */
    /**@{*/
    /**
     * @brief Prints the Pack Project build information in the specified color
     *
     * @param color Text color
     */
    void printTimeStamp(nw4r::ut::Color color) const;

    /**
     * @brief Enables the "Now Loading" text when loading the scene's assets
     */
    void enableLoadMessage();
    /**
     * @brief Loads the scene's assets without displaying any visual indicators
     * @note If the flag for the message is enabled, this function will do
     * nothing.
     */
    void load();
    /**
     * @brief Loads the scene's assets and attempts to display "Now Loading"
     * message
     * @note If the flag for the message is disabled, this function will do
     * nothing.
     */
    void loadDebug();
    /**
     * @brief Somehow involved in updating state during asset loading
     * @stubbed
     */
    void loadUpdate();

    /**
     * @brief Allows asynchronous tasks to run upon entering the scene
     */
    void setTaskAsync();
    /**
     * @brief Tests whether the scene's asynchronous tasks have finished running
     */
    bool isTaskAsyncFinish() const;

    /**
     * @brief Thread function used to call @ref taskAsync on the DVD thread
     *
     * @param pArg Thread function argument (the scene)
     */
    static void taskAsyncFunc(void* pArg);
    /**
     * @brief Thread function used to call @ref LoadResource on the DVD thread
     * @remark This function also opens the common sound archive if the scene
     * requires it.
     * @param pArg Thread function argument (the scene)
     */
    static void loadResourceFunc(void* pArg);

    /**
     * @brief Attempts to run asynchronous tasks on the DVD thread
     * @note If the DVD thread is busy, or setTaskAsync was never called, this
     * function will do nothing.
     */
    void initTaskAsync();

    /**
     * @brief Updates the status of the DVD thread
     */
    void updateDvdEndMessage();
    /**
     * @brief Updates the status of the NAND thread
     */
    void updateNandEndMessage();
    /**@}*/

private:
    /**
     * @brief Bitflag indices
     */
    enum EFlag {
        EFlag_LoadMessage,   //!< Show "Now Loading" text
        EFlag_TaskAsyncSet,  //!< Asynchronous task has been started
        EFlag_WaitTaskAsync, //!< Waiting on asynchronous task
        EFlag_EnterFast,     //!< Skip loading resources, fading in, etc.
    };

private:
#ifndef PACK_RESORT
    //! @brief Scene renderer
    //! @remark Upon entering the scene, this becomes the global renderer.
    RPGrpRenderer* mpRenderer; // at 0x2C
    //! Common draw functionality
    RPSysCommonObject* mpCommonObject; // at 0x30
    //! Scene effect creator
    RPSysEffectCreator* mpEffectCreator; // at 0x34
    //! @brief Scene heap
    //! @unused
    EGG::Heap* mpSceneHeap; // at 0x38
    //! Scene flags
    EGG::TBitFlag<u32> mFlags; // at 0x3C
    //! ID of the previously created scene
    s32 mCreatorSceneID; // at 0x40
#else
    //! @todo Document these fields
    char dummy[0x80 - 0x38]; // 0x38
#endif

    //! @brief Unknown structure
    //! @unused
    static UnkStruct sUnkStruct;
};

//! @}

#endif
