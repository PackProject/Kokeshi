#ifndef RP_PARTY_BIL_BALL_MANAGER_H
#define RP_PARTY_BIL_BALL_MANAGER_H
#include "RPTypes.h"
#include <RPBilScene/RPBilBall.h>

class RPBilBallManager {
    RP_SINGLETON_DECL(RPBilBallManager);

public:
    RPBilBall* GetBall(int i) {
        return mBalls[i];
    }

public:
    static const int BALL_MAX = 10;

private:
    char _04[0x7C - 0x4];
    RPBilBall* mBalls[BALL_MAX]; // at 0x7C
};

#endif
