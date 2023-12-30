#ifndef RP_KERNEL_KOKESHI_LOCATION_H
#define RP_KERNEL_KOKESHI_LOCATION_H
#include "types_RP.h"

#include <RVLFaceLib.h>

class RPSysKokeshiLocation {
private:
    RFLDataSource mDataSource; // at 0x0
    u16 mIndex;                // at 0x4
    RFLMiddleDB* mMiddleDB;    // at 0x8

public:
    RPSysKokeshiLocation();
    virtual ~RPSysKokeshiLocation();
};

#endif
