#ifndef RP_UTILITY_RANDOM_H
#define RP_UTILITY_RANDOM_H
#include "RPTypes.h"

class RPUtlRandom {
public:
    static void initialize();
    static u32 getU32() {
        return calc();
    }
    static u32 getU32(u32 max) {
        return max * getF32();
    }
    static f32 getF32() {
        // Limited to u16 bounds
        const u16 rnd =
            static_cast<u16>(sFltRandMax & (getU32() >> sFltSeedShift));
        // Convert to float
        const f32 rnd_f = static_cast<f32>(rnd);
        // Convert to percentage
        return rnd_f / static_cast<f32>(sFltRandMax + 1);
    }

    static u32 getSeed() {
        return sSeed;
    }
    static void setSeed(u32 s) {
        sSeed = s;
    }

private:
    RPUtlRandom() {}

    static u32 calc() {
        return (sSeed = sSeed * sSeedStep + 1);
    }

    static const u32 sSeedStep = 69069;
    static const u32 sFltRandMax = 0xFFFF;
    static const u32 sFltSeedShift = 16;
    static u32 sSeed;
};

#endif
