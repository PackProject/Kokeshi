#ifndef RP_PARTY_BIL_BALL_MANAGER_H
#define RP_PARTY_BIL_BALL_MANAGER_H
#include "RPTypes.h"

#include <RPBilScene/RPBilBall.h>

class RPBilBallManager {
public:
    static RPBilBallManager* GetInstance() {
        return sInstance;
    }

    RPBilBall* GetBall(int i) {
        return mBalls[i];
    }

public:
    static const int BALL_MAX = 10;

private:
    char _00[0x7C];
    RPBilBall* mBalls[BALL_MAX]; // at 0x7C

    static RPBilBallManager* sInstance;
};

#endif
