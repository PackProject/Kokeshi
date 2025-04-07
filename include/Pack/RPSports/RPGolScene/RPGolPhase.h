#ifndef RP_SPORTS_GOL_PHASE_H
#define RP_SPORTS_GOL_PHASE_H
#include <Pack/RPTypes.h>

class RPGolPhase {
public:
    virtual ~RPGolPhase();        // at 0x8
    virtual void Calculate() = 0; // at 0xC

    void Configure();
    void Reset();
    void Enter() const;

    bool WaitCond(bool pause, bool end = false);
    bool WaitFrame(s32 frame, bool end = false);
    bool Check();
    bool IsDoing() const;
    bool Finish();

    void CommitResult(u32 ftScore, u32 trScore, u32 trSubScore);

    void ShowGameStartIn();
    bool IsDoneGameStartIn() const;

    void ShowGameStartOut();
    bool IsDoneGameStartOut() const;

    void ShowPlayerIntro();
    bool IsDonePlayerIntro() const;

    void Wipe();
    bool IsDoneWipe() const;

    bool IsPressA() const;

private:
    s32 mStep;      // at 0x4
    s32 mOver;      // at 0x8
    bool mDoneWait; // at 0xC
    s32 mWaitTimer; // at 0x10
};

#endif
