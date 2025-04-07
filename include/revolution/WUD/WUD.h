#ifndef RVL_SDK_WUD_H
#define RVL_SDK_WUD_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define WUD_MAX_DEV_ENTRY_FOR_STD 10
#define WUD_MAX_DEV_ENTRY_FOR_SMP 6

#define WUD_MAX_DEV_ENTRY 16
#define WUD_DEV_HANDLE_INVALID (-1)

// Forward declarations
typedef struct WUDDevInfo WUDDevInfo;

typedef enum {
    WUD_CHAN0,
    WUD_CHAN1,
    WUD_CHAN2,
    WUD_CHAN3,

    WUD_MAX_CHANNELS,
    WUD_CHAN_INVALID = -1
} WUDChannel;

typedef enum {
    WUD_LIB_STATUS_0,
    WUD_LIB_STATUS_1,
    WUD_LIB_STATUS_2,
    WUD_LIB_STATUS_3,
    WUD_LIB_STATUS_4,
} WUDLibStatus;

typedef enum {
    WUD_SYNC_TYPE_STANDARD,
    WUD_SYNC_TYPE_SIMPLE,
} WUDSyncType;

typedef void* (*WUDAllocFunc)(u32 size);
typedef BOOL (*WUDFreeFunc)(void* pBlock);

typedef void (*WUDSyncDeviceCallback)(s32 result, s32 num);
typedef void (*WUDClearDeviceCallback)(s8 result);

typedef void (*WUDHidConnectCallback)(WUDDevInfo* pInfo, u8 isOpenEvent);
typedef void (*WUDHidReceiveCallback)(UINT8 devHandle, UINT8* pReport,
                                      UINT16 len);

typedef struct WUDSmallDevInfo {
    char devName[20]; // at 0x0
    char at_0x14[1];
    char UNK_0x15[0xB];
    LINK_KEY linkKey; // at 0x20
    char UNK_0x30[0x10];
} WUDSmallDevInfo;

typedef struct WUDDevInfo {
    WUDSmallDevInfo small; // at 0x0
    BD_ADDR devAddr;       // at 0x40
    LINK_KEY linkKey;      // at 0x46
    UINT8 devHandle;       // at 0x56
    UINT8 subclass;        // at 0x57
    UINT8 appID;           // at 0x58
    u8 status;             // at 0x59
    s8 UNK_0x5A;           // at 0x5A
    u8 UNK_0x5B;           // at 0x5B
    u8 UNK_0x5C;           // at 0x5C
    u8 unk_0x5d[1];
    tBTA_HH_ATTR_MASK hhAttrMask; // at 0x5E
} WUDDevInfo;

void WUDiMoveBottomSmpDevInfoPtr(WUDDevInfo* pInfo);
void WUDiMoveTopStdDevInfoPtr(WUDDevInfo* pInfo);
void WUDiMoveBottomStdDevInfoPtr(WUDDevInfo* pInfo);
BOOL WUDIsBusy(void);

#ifdef __cplusplus
}
#endif
#endif
