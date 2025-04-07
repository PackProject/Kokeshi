#ifndef RP_SPORTS_GOL_SEQUENCE_MANAGER_H
#define RP_SPORTS_GOL_SEQUENCE_MANAGER_H
#include <Pack/RPTypes.h>

// Forward declarations
class RPGolPhase;
class RPGolSequenceTest;
class RPGolSequenceMain;
class RPGolSequenceTraining1;
class RPGolSequenceTraining2;
class RPGolSequenceTraining3;

class RPGolSequenceManager {
    RP_SINGLETON_DECL(RPGolSequenceManager);

public:
    void SetPhase(RPGolPhase* pPhase) {
        mpPhase = pPhase;
    }

    RPGolSequenceMain* GetSequenceMain() const {
        return mpSequenceMain;
    }

private:
    RPGolSequenceTest* mpSequenceTest;           // at 0x4
    RPGolSequenceMain* mpSequenceMain;           // at 0x8
    RPGolSequenceTraining1* mpSequenceTraining1; // at 0xC
    RPGolSequenceTraining2* mpSequenceTraining2; // at 0x10
    RPGolSequenceTraining3* mpSequenceTraining3; // at 0x14

    RPGolPhase* mpPhase; // at 0x18
};

#endif
