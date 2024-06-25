#ifndef RP_PARTY_GAME_MGR_H
#define RP_PARTY_GAME_MGR_H
#include <Pack/RPTypes.h>

class RPPartyGameMgr {
    RP_SINGLETON_DECL(RPPartyGameMgr);

public:
    void Reset();
};

#endif
