#ifndef RP_KERNEL_SYSTEM_H
#define RP_KERNEL_SYSTEM_H
#include <Pack/RPTypes.h>
#include <RPSystem/RPSysConfigData.h>
#include <RPTypes.h>
#include <egg/core.h>
#include <nw4r/ut.h>
#include <revolution/OS.h>

//! @addtogroup rp_system
//! @{

/**
 * @brief Main RP engine class
 * @wfuname
 *
 * The system class manages the low level components of the RP engine, such as
 * its many heaps, console power/reset callbacks, and TV/render modes.
 */
class RPSysSystem {
public:
// KOKESHI: We need to access the engine configuration
#ifdef __KOKESHI__
    static EGG::Heap* getRootHeapMem1() {
        return sConfigData.getRootHeapMem1();
    }
    static EGG::Heap* getRootHeapMem2() {
        return sConfigData.getRootHeapMem2();
    }
    static EGG::Heap* getSystemHeap() {
        return sConfigData.getSystemHeap();
    }
#endif

    /**
     * @brief Gets this class's singleton instance
     */
    static RPSysSystem* getInstance() {
        return spInstance;
    }

    /**
     * @brief Auto-detects the screen's aspect ratio
     */
    static void setupTVMode();
    /**
     * @brief Auto-detects the render mode and progressive setting
     */
    static void setupRenderMode();

    /**
     * @brief Initializes the RP engine and its core components
     */
    static void initialize();

    /**
     * @brief Creates this class's singleton instance
     */
    static void create();

    /**
     * @brief Gets the width of the framebuffer, in pixels
     */
    u16 getFBWidth() const;
    /**
     * @brief Gets the height of the framebuffer, in pixels
     */
    u16 getFBHeight() const;

    /**
     * @brief Loads the Pack Project build information from the disc
     */
    void createTimeStamp();
    /**
     * @brief Creates a work buffer for the effect manager
     */
    void createEffectWork();
    /**
     * @brief Creates the engine's core components using the system heap
     */
    void createFromSystemHeap();

    /**
     * @brief Gets the engine ready to calculate and draw the first frame
     */
    void setup();

    /**
     * @brief Enters the engine's main loop
     */
    void mainLoop();

    /**
     * @brief Gets the ID of the boot scene
     */
    u32 getBootScene();

    /**
     * @brief Gets the current clock time represented as a 32-bit calendar time
     */
    RPTime32 getCalendarTime32();
    /**
     * @brief Gets the current clock time represented as a 16-bit calendar time
     */
    RPTime16 getCalendarTime16();

    /**
     * @brief Splits 32-bit calendar time into parts
     * @details If no input time is specified, this function operates on the
     * current clock time.
     *
     * @param pTime32 Input time
     * @param rWDay Day of the week
     * @param rYear Year
     * @param rMon Month (one-indexed)
     * @param rMDay Day of the month
     * @param rHour Hour
     */
    void getCalendarTime(const RPTime32* pTime32, u32& rWDay, u32& rYear,
                         u32& rMon, u32& rMDay, u32& rHour);

    /**
     * @brief Splits 16-bit calendar time into parts
     * @details If no input time is specified, this function operates on the
     * current clock time.
     *
     * @param pTime16 Input time
     * @param rYear Year
     * @param rMon Month (one-indexed)
     * @param rMDay Day of the month
     */
    void getCalendarTime(const RPTime16* pTime16, u32& rYear, u32& rMon,
                         u32& rMDay);

    /**
     * @brief Shut-down the engine and exit the application
     * @details Depending on the instructions from the scene manager, this will
     * either exit to the system menu or shutdown the console.
     */
    void systemShutDown();

    /**
     * @brief Updates the system state during resource loading
     *
     * @param hide Disable "Now Loading" text
     */
    void loadFrameWork(bool hide);

    /**
     * @brief Gets the number of ticks per frame
     */
    u8 getFrameRate() const;
    /**
     * @brief Gets the modify ratio of the display
     */
    f32 getModifyRatio() const;

    /**
     * @brief Sets the background clear color of the EFB
     *
     * @param color Clear color
     */
    void setDisplayClearColor(nw4r::ut::Color color);

    /**
     * @brief Destroys the specified heap
     *
     * @param pHeap Heap to destroy
     */
    void destroyHeap(EGG::Heap* pHeap);

    /**
     * @brief Allocates memory from the specified heap
     *
     * @param pHeap Heap to allocate from
     * @param size Amount of memory required
     * @param align Required byte-alignment
     * @return void* Allocated memory block
     */
    void* alloc(EGG::Heap* pHeap, u32 size, s32 align = 4) {
        return EGG::Heap::alloc(size, align, pHeap);
    }

    /**
     * @brief Frees memory to the specified heap
     *
     * @param pHeap Heap to free to
     * @param pBlock Memory block to free
     */
    void free(EGG::Heap* pHeap, void* pBlock) {
        return EGG::Heap::free(pBlock, pHeap);
    }

    /**
     * @brief Initializes the load count to zero frames
     */
    void startLoadCount();
    /**
     * @brief Tests whether the current load count is over five seconds (600f)
     */
    bool isLoadCountEnd() const;

    /**
     * @brief Turns on/off screen dimming
     * @details If "Screen Saver Mode" is disabled, dimming is always OFF.
     *
     * @param dim Whether to dim the screen
     */
    void setDimming(BOOL dim);

    /**
     * @brief Gets the heap for the resource manager
     */
    EGG::Heap* getResourceHeap() {
        return mpResourceHeap;
    }

    /**
     * @brief Gets the thread for asynchronous NAND operations
     */
    EGG::TaskThread* getNandThread();
    /**
     * @brief Gets the thread for asynchronous disc operations
     */
    EGG::TaskThread* getDvdThread();
    /**
     * @brief Gets the thread for asynchronous WiiConnect24 operations
     */
    EGG::TaskThread* getWc24Thread();

    /**
     * @brief Gets the task-end thread message for asynchronous NAND operations
     */
    OSMessage getNandEndMessage() const;
    /**
     * @brief Gets the task-end thread message for asynchronous disc operations
     */
    OSMessage getDvdEndMessage() const;

    /**
     * @brief Gets the Pack Project build information
     */
    const char* getTimeStampString();

    /**
     * @brief Auto-detects project locale settings
     */
    void setupLocalSettings();

    /**
     * @brief Blocks the main thread to safely change video/render mode
     * configuration
     */
    void waitRenderModeChange();

    /**
     * @brief Installs reset and shutdown callbacks to the OS
     */
    void setCallBack();

private:
    /**
     * @brief Constructor
     */
    RPSysSystem();
    /**
     * @brief Destructor
     */
    virtual ~RPSysSystem();

    /**
     * @brief OS reset callback
     */
    static void softResetCallBack();
    /**
     * @brief OS shutdown callback
     */
    static void shutdownSystemCallBack();

private:
    //! Class singleton instance
    static RPSysSystem* spInstance;
    //! Engine configuration
    static RPSysConfigData sConfigData;

    //! Effect manager work memory size
    u32 mEffectWorkSize; // at 0x4

    //! Heap used by the resource manager
    EGG::Heap* mpResourceHeap; // at 0x8
    //! Heap reserving memory in the MEM1 region
    EGG::Heap* mpReserveHeap; // at 0xC
    //! Heap used for error handling when other heaps are exhausted
    EGG::Heap* mpAssertHeap; // at 0x10
    //! @unused
    EGG::Heap* HEAP_0x14;

    //! Backup of the EGG current heap while it is changed
    EGG::Heap* mpCurrentHeap; // at 0x18
    //! Mutex to guard changing the current heap
    OSMutex mCurrentHeapMutex; // at 0x1C

    //! Thread for asynchronous NAND operations
    EGG::TaskThread* mpNandThread; // at 0x34
    //! Thread for asynchronous disc operations
    EGG::TaskThread* mpDvdThread; // at 0x38
    //! Thread for asynchronous WiiConnect24 operations
    EGG::TaskThread* mpWc24Thread; // at 0x3C

    //! Task-end thread message for asynchronous NAND operations
    u32 mNandEndMessage; // at 0x40
    //! Task-end thread message for asynchronous disc operations
    u32 mDvdEndMessage; // at 0x44
    //! Task-end thread message for asynchronous WiiConnect24 operations
    u32 mWc24EndMessage; // at 0x48

    //! Number of ticks per frame
    u8 mFrameRate; // at 0x4C
    //! Average frame duration, in seconds (assumes 60fps)
    f32 mFrameTime; // at 0x50
    //! Total number of frames spent loading resources
    u32 mLoadCount; // at 0x54

    //! Pack Project build information
    char* mpTimeStampString; // at 0x58
};

//! @}

#endif
