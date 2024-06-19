#ifndef RP_SPORTS_GOL_MAP_OBJ_CUP_H
#define RP_SPORTS_GOL_MAP_OBJ_CUP_H
#include "RPGolMapObjBase.h"
#include "RPTypes.h"

class RPGolMapObjCup : public RPGolMapObjBase {
public:
    RPGolMapObjCup();
    virtual ~RPGolMapObjCup();

    bool IsCutSuccess() const {
        return mCutSuccess;
    }

    void SetCollision();

private:
    char _50[0xA0 - 0x50];
    bool mCutSuccess;
};

#endif
