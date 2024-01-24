#ifndef RP_SPORTS_GOL_MAP_OBJ_PIN_H
#define RP_SPORTS_GOL_MAP_OBJ_PIN_H
#include "RPGolMapObjBase.h"
#include "types_RP.h"

class RPGolMapObjPin : public RPGolMapObjBase {
public:
    RPGolMapObjPin();
    virtual ~RPGolMapObjPin();

    void SetPinTrans(const nw4r::math::VEC3& trans) {
        SetTrans(trans);
        mPinBottomObj.SetTrans(trans);
    }

private:
    char _50[0x64 - 0x50];
    RPGolMapObjBase mPinBottomObj; // at 0x64
    char _B4[0xEC - 0xB4];
};

#endif
