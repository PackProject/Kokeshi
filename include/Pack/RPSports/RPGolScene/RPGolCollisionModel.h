#ifndef RP_SPORTS_GOL_COLLISION_MODEL_H
#define RP_SPORTS_GOL_COLLISION_MODEL_H
#include "RPSysUnknownBase.h"
#include "RPTypes.h"

#include <nw4r/math.h>
#include <nw4r/ut.h>

struct RPGolCollisionTriangle {
    f32 mLength;  // at 0x0
    u16 mPosIdx;  // at 0x4
    u16 mDirIdx;  // at 0x6
    u16 mNrmAIdx; // at 0x8
    u16 mNrmBIdx; // at 0xA
    u16 mNrmCIdx; // at 0xC
    u16 attr;     // at 0xE
};

enum EGndAttr {
    EGndAttr_None,
    EGndAttr_Fairway,
    EGndAttr_Rough,
    EGndAttr_Bunker,
    EGndAttr_OB,
    EGndAttr_OB2,
    EGndAttr_Green,
    EGndAttr_Water,
    EGndAttr_Wood,
    EGndAttr_GreenEdge,
    EGndAttr_SideOfCup,
    EGndAttr_Cup,
    EGndAttr_RockWall,
    EGndAttr_Valley,
    EGndAttr_Target,
    EGndAttr_Tee,
    EGndAttr_SandOB,

    EGndAttr_Max
};

class RPGolCollisionModel : RPSysUnknownBase {
    RP_SINGLETON_DECL(RPGolCollisionModel);

public:
    static const char* GetCollisionDesc(EGndAttr col) {
        return sCollisionDesc[col];
    }
    static nw4r::ut::Color GetCollisionColor(EGndAttr col) {
        return sCollisionColors[col];
    }

    RPGolCollisionTriangle* GetTris() const {
        return mTriangles;
    }
    u32 GetNumTris() const {
        return mNumRPTris;
    }

private:
    RPGolCollisionModel();
    virtual ~RPGolCollisionModel();

private:
    nw4r::math::VEC3* mVertices;        // at 0x4
    nw4r::math::VEC3* mNormals;         // at 0x8
    RPGolCollisionTriangle* mTriangles; // at 0xC
    void* mOctree;                      // at 0x10
    f32 mThickness;                     // at 0x14
    nw4r::math::VEC3 mOctreeFirst;      // at 0x18
    u32 mMaskX;                         // at 0x24
    u32 mMaskY;                         // at 0x28
    u32 mMaskZ;                         // at 0x2C
    u32 mShiftX;                        // at 0x30
    u32 mShiftY;                        // at 0x34
    u32 mShiftZ;                        // at 0x38
    f32 mMaxHitboxRadius;               // at 0x3C
    char UNK_0x40[0x5C - 0x40];
    u32 mNumRPTris;    // at 0x5C
    UNKTYPE* PTR_0x60; // at 0x60

    static RPGolCollisionModel* sInstance;
    static const char* sCollisionDesc[EGndAttr_Max];
    static nw4r::ut::Color sCollisionColors[EGndAttr_Max];
};

#endif
