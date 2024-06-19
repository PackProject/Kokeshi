#ifndef RP_SYSTEM_BOOT_SCENE_H
#define RP_SYSTEM_BOOT_SCENE_H
#include "RPSysScene.h"
#include "RPTypes.h"

/**
 * @brief Pack Project boot scene
 * @note Stub class for SceneCreator
 */
class RPSysBootScene : public RPSysScene {
public:
    enum EState {
        EState_Init,
        EState_FadeIn,
        EState_Wait,
        EState_FadeOut,
    };

public:
    RPSysBootScene() {}
    // @address 803c1fe0
    virtual ~RPSysBootScene();

private:
    char UNK_0x44[0x50 - 0x44];
    EState mState; // at 0x50
    char UNK_0x54[0x58 - 0x54];
};

#endif
