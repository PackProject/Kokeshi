#ifndef RVL_SDK_WUD_INTERNAL_H
#define RVL_SDK_WUD_INTERNAL_H
#include <types.h>

// Public API
#include <revolution/WUD/WUD.h>

#include <revolution/BTE.h>
#include <revolution/OS.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WUD_STATE_START = 0,

    WUD_STATE_WII_FIT_OPEN = 100,
    WUD_STATE_WII_FIT_SEEK = 101,
    WUD_STATE_WII_FIT_UPDATE = 102,
    WUD_STATE_WII_FIT_CLOSE = 103,

    WUD_STATE_ERROR = 255
} WUDState;

typedef enum {
    WUD_STATE_SYNC_PREPARE_SEARCH = 1,
    WUD_STATE_SYNC_START_SEARCH = 2,
    WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT = 3,
    WUD_STATE_SYNC_CHECK_SEARCH_RESULT = 4,
    WUD_STATE_SYNC_IS_EXISTED_DEVICE = 5,
    WUD_STATE_SYNC_6 = 6,
    WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM = 7,
    WUD_STATE_SYNC_WAIT_FOR_STORING = 8,
    WUD_STATE_SYNC_WAIT_FOR_READING = 10,
    WUD_STATE_SYNC_11 = 11,
    WUD_STATE_SYNC_12 = 12,
    WUD_STATE_SYNC_13 = 13,
    WUD_STATE_SYNC_DONE = 14,
    WUD_STATE_SYNC_TRY_CONNECT = 15,
    WUD_STATE_SYNC_PREPARE_FOR_EXISTED_DEVICE = 16,
    WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE = 17,
    WUD_STATE_SYNC_REGISTER_DEVICE = 18,
    WUD_STATE_SYNC_VIRGIN_SIMPLE = 19,
    WUD_STATE_SYNC_VIRGIN_STANDARD = 20,
    WUD_STATE_SYNC_CHANGE_SIMPLE_TO_STANDARD = 21,
    WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND = 22,
    WUD_STATE_SYNC_COMPLETE = 23,
    WUD_STATE_SYNC_WAIT_FOR_INCOMING = 24,
    WUD_STATE_SYNC_SC_FLUSH = 25,
    WUD_STATE_SYNC_26 = 26,
    WUD_STATE_SYNC_WAIT_FOR_START_SEARCH = 29,
} WUDSyncState;

typedef enum {
    WUD_RESULT_SYNC_BUSY = -1,
    WUD_RESULT_SYNC_WAITING,
    WUD_RESULT_SYNC_DONE,
} WUDSyncResult;

typedef enum {
    WUD_STATE_DELETE_DISALLOW_INCOMING = 1,
    WUD_STATE_DELETE_DISCONNECT_ALL = 2,
    WUD_STATE_DELETE_CLEANUP_DATABASE = 3,
    WUD_STATE_DELETE_CLEANUP_SETTING = 5,
    WUD_STATE_DELETE_6 = 6,
    WUD_STATE_DELETE_7 = 7,
    WUD_STATE_DELETE_DONE = 8
} WUDDeleteState;

typedef enum {
    WUD_RESULT_DELETE_BUSY = -1,
    WUD_RESULT_DELETE_WAITING,
    WUD_RESULT_DELETE_COMPLETE,
} WUDDeleteResult;

typedef enum {
    WUD_STATE_LINK_KEY_READING = 1,
    WUD_STATE_LINK_KEY_WRITING = 2,
    WUD_STATE_LINK_KEY_DELETING = 3,
} WUDLinkKeyState;

typedef enum {
    WUD_STATE_STACK_GET_STORED_LINK_KEY = 1,
    WUD_STATE_STACK_CHECK_DEVICE_INFO = 2,
    WUD_STATE_STACK_DONE = 3,
    WUD_STATE_STACK_INITIALIZED = 4
} WUDStackState;

typedef enum {
    WUD_STATE_INIT_WAIT_FOR_INITIALIZATION = 1,
    WUD_STATE_INIT_GET_DEV_INFO = 2,
    WUD_STATE_INIT_DONE = 3,
    WUD_STATE_INIT_INITIALIZED = 4
} WUDInitState;

typedef enum {
    WUD_STATE_SHUTDOWN_STORE_SETTINGS = 1,
    WUD_STATE_SHUTDOWN_FLUSH_SETTINGS = 2,
    WUD_STATE_SHUTDOWN_DONE = 3
} WUDShutdownState;

typedef enum {
    WUD_VSE_INITIATE_PAIRING = 8,
    WUD_VSE_DELETE_ALL_KEYS,
    WUD_VSE_SI_PORT_STATUS,
    WUD_VSE_WATCH_DOG_RESET_HW = 16,
} WUDVendorSpecificEvent;

typedef struct WUDDevInfoList {
    WUDDevInfo* devInfo;         // at 0x0
    struct WUDDevInfoList* prev; // at 0x4
    struct WUDDevInfoList* next; // at 0x8
} WUDDevInfoList;

typedef struct WUDDiscResp {
    BD_ADDR devAddr;  // at 0x0
    char devName[64]; // at 0x6
    u8 UNK_0x46[0xBA];
    tBTA_SERVICE_MASK services; // at 0x100
    u8 UNK_0x104[0x4];
} WUDDiscResp;

typedef struct WUDCB {
    WUDSyncDeviceCallback syncStdCB;   // at 0x0
    WUDSyncDeviceCallback syncSmpCB;   // at 0x4
    WUDClearDeviceCallback clearDevCB; // at 0x8

    u8 syncState;     // at 0xC
    u8 deleteState;   // at 0xD
    u8 linkKeyState;  // at 0xE
    u8 stackState;    // at 0xF
    u8 initState;     // at 0x10
    u8 shutdownState; // at 0x11

    u8 devNums;    // at 0x12
    u8 devSmpNums; // at 0x13

    WUDDevInfoList* smpListHead;                       // at 0x14
    WUDDevInfoList* smpListTail;                       // at 0x18
    WUDDevInfoList smpList[WUD_MAX_DEV_ENTRY_FOR_SMP]; // at 0x1C

    WUDDevInfoList* stdListHead;                       // at 0x64
    WUDDevInfoList* stdListTail;                       // at 0x68
    WUDDevInfoList stdList[WUD_MAX_DEV_ENTRY_FOR_STD]; // at 0x6C

    WUDDevInfo stdDevs[WUD_MAX_DEV_ENTRY_FOR_STD]; // at 0xE4
    WUDDevInfo smpDevs[WUD_MAX_DEV_ENTRY_FOR_SMP]; // at 0x4A4

    u8 connectedNum; // at 0x6E4
    u8 linkedNum;    // at 0x6E5
    u8 syncedNum;    // at 0x6E6

    u8 syncSkipChecks; // at 0x6E7
    s8 syncLoopNum;    // at 0x6E8
    u8 syncType;       // at 0x6E9

    u8 connectable;  // at 0x6EA
    u8 discoverable; // at 0x6EB

    WUDHidReceiveCallback hidRecvCB; // at 0x6EC
    WUDHidConnectCallback hidConnCB; // at 0x6F0

    WUDAllocFunc allocFunc; // at 0x6F4
    WUDFreeFunc freeFunc;   // at 0x6F8

    BD_ADDR pairAddr; // at 0x6FC
    BD_ADDR hostAddr; // at 0x702

    s8 libStatus; // at 0x708

    u8 serialPortStatus; // at 0x709
    UINT8 pmID;          // at 0x70A
    s8 syncRssi;         // at 0x70B
    OSAlarm alarm;       // at 0x710
    u32 hhFlags;         // at 0x740

    u16 bufferStatus0; // at 0x744
    u16 bufferStatus1; // at 0x746

    s8 initWaitDeviceUpFrames; // at 0x748
    s8 waitStartSearchFrames;  // at 0x749
    s16 waitIncomingFrames;    // at 0x74A
} WUDCB;

extern WUDCB _wcb;

#ifdef __cplusplus
}
#endif
#endif
