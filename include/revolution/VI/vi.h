#ifndef RVL_SDK_VI_H
#define RVL_SDK_VI_H
#include <revolution/GX.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

//! @addtogroup rvl_vi
//! @{

//! Merge format/scan mode to one value for TV mode @see GXRenderModeObj
#define VI_TVMODE(format, mode) (((format) << 2) + (mode))
//! Get TV format from TV mode
#define VI_TVMODE_GETFMT(info) ((info) >> 2)
//! Get TV scan mode from TV mode
#define VI_TVMODE_GETSCAN(info) ((info) & 0b00000011)

typedef enum {
    VI_TV_FMT_NTSC,
    VI_TV_FMT_PAL,
    VI_TV_FMT_MPAL,
    VI_TV_FMT_DEBUG,
    VI_TV_FMT_DEBUG_PAL,
    VI_TV_FMT_EURGB60,
    VI_TV_FMT_6,
    VI_TV_FMT_7,
    VI_TV_FMT_8,
} VITvFormat;

typedef enum {
    VI_SCAN_MODE_INT,
    VI_SCAN_MODE_DS,
    VI_SCAN_MODE_PROG
} VIScanMode;

typedef enum {
    VI_TVMODE_NTSC_INT = VI_TVMODE(VI_TV_FMT_NTSC, VI_SCAN_MODE_INT),
    VI_TVMODE_NTSC_DS = VI_TVMODE(VI_TV_FMT_NTSC, VI_SCAN_MODE_DS),
    VI_TVMODE_NTSC_PROG = VI_TVMODE(VI_TV_FMT_NTSC, VI_SCAN_MODE_PROG),

    VI_TVMODE_PAL_INT = VI_TVMODE(VI_TV_FMT_PAL, VI_SCAN_MODE_INT),
    VI_TVMODE_PAL_DS = VI_TVMODE(VI_TV_FMT_PAL, VI_SCAN_MODE_DS),

    VI_TVMODE_EURGB60_INT = VI_TVMODE(VI_TV_FMT_EURGB60, VI_SCAN_MODE_INT),
    VI_TVMODE_EURGB60_DS = VI_TVMODE(VI_TV_FMT_EURGB60, VI_SCAN_MODE_DS),
    VI_TVMODE_EURGB60_PROG = VI_TVMODE(VI_TV_FMT_EURGB60, VI_SCAN_MODE_PROG),

    VI_TVMODE_MPAL_INT = VI_TVMODE(VI_TV_FMT_MPAL, VI_SCAN_MODE_INT),
    VI_TVMODE_MPAL_DS = VI_TVMODE(VI_TV_FMT_MPAL, VI_SCAN_MODE_DS),
    VI_TVMODE_MPAL_PROG = VI_TVMODE(VI_TV_FMT_MPAL, VI_SCAN_MODE_PROG),

    VI_TVMODE_DEBUG_INT = VI_TVMODE(VI_TV_FMT_DEBUG, VI_SCAN_MODE_INT),
    VI_TVMODE_DEBUG_PAL_INT = VI_TVMODE(VI_TV_FMT_DEBUG_PAL, VI_SCAN_MODE_INT),
    VI_TVMODE_DEBUG_PAL_DS = VI_TVMODE(VI_TV_FMT_DEBUG_PAL, VI_SCAN_MODE_DS)
} VITvMode;

typedef enum {
    VI_XFB_MODE_SF,
    VI_XFB_MODE_DF,
} VIXfbMode;

typedef void (*VIRetraceCallback)(u32 retraceCount);
typedef void (*VIPositionCallback)(s16 displayX, s16 displayY);

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback callback);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback);

void VIInit(void);
void VIWaitForRetrace(void);

void VIConfigure(const GXRenderModeObj* rmo);
void VIConfigurePan(u16 x, u16 y, u16 w, u16 h);
void VIFlush(void);

void VISetNextFrameBuffer(void* fb);
void* VIGetCurrentFrameBuffer(void);

void VISetBlack(BOOL black);
s32 VIGetRetraceCount(void);

u32 VIGetTvFormat(void);
u32 VIGetScanMode(void);

//! @}

#ifdef __cplusplus
}
#endif
#endif
