#ifndef RP_GRAPHICS_RENDERER_H
#define RP_GRAPHICS_RENDERER_H
#include "RPTypes.h"

// Forward declarations
class RPGrpScreen;

class RPGrpRenderer {
    RP_SINGLETON_DECL(RPGrpRenderer);

public:
    enum EDrawPass {
        EDrawPass_DrawBefore,
        EDrawPass_1,
        EDrawPass_2,
        EDrawPass_3,
        EDrawPass_DrawAfter,
        EDrawPass_Draw2D,
        EDrawPass_Effect0,
        EDrawPass_Effect1,
        EDrawPass_Effect3,
        EDrawPass_Effect4,
        EDrawPass_Effect2,
        EDrawPass_Begin,
        EDrawPass_End,
        EDrawPass_Null
    };

public:
    static void Begin();
    static void End();

    void AppendDrawObject(IRPGrpDrawObject*);

    void CreateView2D(u8, RPGrpScreen*);
    void CorrectView();

    static void SetEfbClearColor(u8 r, u8 g, u8 b);

    static EDrawPass GetDrawPass() {
        return sDrawPass;
    }
    static void SetDrawPass(EDrawPass pass) {
        sDrawPass = pass;
    }

    static RPGrpScreen* GetActiveScreen() {
        return sActiveScreen;
    }

    static bool IsDrawLayout() {
        return sDrawPass == EDrawPass_Draw2D;
    }

private:
    char UNK_0x0[0xC];
    IRPGrpDrawObject* mDrawObjects; // at 0xC

    static EDrawPass sDrawPass;
    static u8 sCurrentViewID;
    static RPGrpRenderer* sInstance;
    static RPGrpScreen* sActiveScreen;
};

#endif
