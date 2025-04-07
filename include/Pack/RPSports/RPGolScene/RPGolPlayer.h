#ifndef RP_SPORTS_GOL_PLAYER_H
#define RP_SPORTS_GOL_PLAYER_H
#include <Pack/RPTypes.h>

class RPGolPlayer {
public:
    u32 GetStrokeNum() const {
        return mStrokeNum;
    }

private:
    u8 _00[0x78];
    u32 mStrokeNum; // at 0x78
    u8 _7C[0xCC - 0x7C];
};

#endif
