#ifndef RVL_SDK_SI_SISAMPLINGRATE_H
#define RVL_SDK_SI_SISAMPLINGRATE_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

//! @addtogroup rvl_si
//! @{
//! @file

#define SI_MAX_SAMPLE_RATE 11

void SISetSamplingRate(u32 msec);
void SIRefreshSamplingRate(void);

//! @}

#ifdef __cplusplus
}
#endif
#endif
