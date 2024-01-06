#ifndef RVL_FACE_LIBRARY_CONTROLLER_H
#define RVL_FACE_LIBRARY_CONTROLLER_H
#include <RVLFaceLib/RFL_Types.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

RFLErrcode RFLLoadControllerAsync(s32 chan);
BOOL RFLIsAvailableControllerData(s32 chan, u16 index);

#ifdef __cplusplus
}
#endif
#endif
