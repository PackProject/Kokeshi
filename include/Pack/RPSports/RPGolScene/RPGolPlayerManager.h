#ifndef RP_SPORTS_GOL_PLAYER_MANAGER_H
#define RP_SPORTS_GOL_PLAYER_MANAGER_H
#include <Pack/RPSports/RPGolScene/RPGolPlayer.h>
#include <Pack/RPTypes.h>

class RPGolPlayerManager {
    RP_SINGLETON_DECL(RPGolPlayerManager);

public:
    RPGolPlayer& GetPlayer(u32 i) {
        return mPlayers[i];
    }

private:
    static const int PLAYER_MAX = 4;

private:
    RPGolPlayer mPlayers[PLAYER_MAX];
};

#endif
