#ifndef RVL_SDK_WUD_HID_HOST_H
#define RVL_SDK_WUD_HID_HOST_H
#include <types.h>

#include <revolution/BTE.h>
#ifdef __cplusplus
extern "C" {
#endif

void WUDiHidHostEventCallback(tBTA_HH_EVT event, tBTA_HH* p_data);

#ifdef __cplusplus
}
#endif
#endif
