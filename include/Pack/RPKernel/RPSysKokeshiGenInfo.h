#ifndef RP_KERNEL_KOKESHI_GEN_INFO_H
#define RP_KERNEL_KOKESHI_GEN_INFO_H
#include "RPTypes.h"
#include <RPKernel/RPSysKokeshiLocation.h>
#include <RPKernel/RPSysKokeshiManager.h>

#include <RVLFaceLib.h>

class RPSysKokeshiGenInfo {
private:
    RPSysKokeshiManager::GenType mGenType; // at 0x0
    u8 mPlayerNo;                          // at 0x4
    RPSysKokeshiLocation mLocation;        // at 0x8
    u16 mBodyType;                         // at 0x18
    u32 mHandType;                         // at 0x1C
    bool BOOL_0x20;
    u32 WORD_0x24;
    u32 WORD_0x28;
    u16 SHORT_0x2C;
    u16 SHORT_0x2E;
    RFLResolution mResolution; // at 0x30
    RFLExpression mExpression; // at 0x34
    u32 WORD_0x38;
    u32 WORD_0x3C;
    char _40[4];
    u32 WORD_0x44;
    u32 WORD_0x48;
    u32 WORD_0x4C;
    char _50[4];
    u16 SHORT_0x54;

public:
    RPSysKokeshiGenInfo();
    virtual ~RPSysKokeshiGenInfo();
};

#endif
