#ifndef RP_PARTY_GAME_MGR_H
#define RP_PARTY_GAME_MGR_H
#include "types.h"

class RPPartyGameMgr {
public:
    static RPPartyGameMgr* GetInstance() {
        return sInstance;
    }

    static void CreateInstance();
    static void DestroyInstance();

    void Reset();

private:
    static RPPartyGameMgr* sInstance;
};

#endif
