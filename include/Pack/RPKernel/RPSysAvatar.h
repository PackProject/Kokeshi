#ifndef RP_KERNEL_AVATAR_H
#define RP_KERNEL_AVATAR_H
#include "RPTypes.h"

#include <RPKernel/RPSysKokeshiGenInfo.h>
#include <RPKernel/RPSysKokeshiOverloadInfo.h>

class RPSysAvatar {
public:
    RPSysAvatar(const RPSysKokeshiGenInfo*);
    virtual ~RPSysAvatar();
    virtual void LoadResource(const RPSysKokeshiOverloadInfo*);
    virtual void ApplyLightTexture(u8) = 0;

private:
    RPSysKokeshiGenInfo mGenInfo;           // at 0x4
    RPSysKokeshiOverloadInfo mOverloadInfo; // at 0x60
};

#endif
