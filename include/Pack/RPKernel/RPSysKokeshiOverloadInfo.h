#ifndef RP_KERNEL_KOKESHI_OVERLOAD_INFO_H
#define RP_KERNEL_KOKESHI_OVERLOAD_INFO_H
#include "types_RP.h"

class RPSysKokeshiOverloadInfo {
public:
    RPSysKokeshiOverloadInfo();
    virtual ~RPSysKokeshiOverloadInfo();

private:
    u16 mClothesType; // at 0x0
};

#endif
