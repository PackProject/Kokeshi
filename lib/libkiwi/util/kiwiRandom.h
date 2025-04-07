#ifndef LIBKIWI_UTIL_RANDOM_H
#define LIBKIWI_UTIL_RANDOM_H
#include <algorithm>
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Random number generator
 */
class Random {
public:
    /**
     * @brief Constructor
     */
    Random() : mSeed(OSGetTick()) {}

    /**
     * @brief Set random seed
     *
     * @param seed New seed
     */
    void SetSeed(u32 seed) {
        mSeed = seed;
    }

    /**
     * @brief Get random seed
     */
    u32 GetSeed() const {
        return mSeed;
    }

    /**
     * @brief Get random u32 (unbounded)
     */
    u32 NextU32() {
        Update();
        return mSeed;
    }

    /**
     * @brief Get random s32 (unbounded)
     */
    s32 NextS32() {
        return static_cast<s32>(NextU32());
    }

    /**
     * @brief Get random u32 (upper bound)
     *
     * @param max Upper bound (exclusive)
     */
    u32 NextU32(u32 max) {
        return static_cast<u32>(max * NextF32());
    }

    /**
     * @brief Get random s32 (upper bound)
     *
     * @param max Upper bound (exclusive)
     */
    s32 NextS32(s32 max) {
        return static_cast<s32>(NextU32(max));
    }

    /**
     * @brief Get random u32 (lower+upper bound)
     *
     * @param min Lower bound (inclusive)
     * @param max Upper bound (exclusive)
     */
    u32 NextU32(u32 min, u32 max) {
        K_ASSERT(min < max);
        return min + NextU32(max - min + 1);
    }

    /**
     * @brief Get random s32 (lower+upper bound)
     *
     * @param min Lower bound (inclusive)
     * @param max Upper bound (exclusive)
     */
    s32 NextS32(s32 min, s32 max) {
        return static_cast<s32>(NextU32(min, max));
    }

    /**
     * @brief Get random float -> [0.0 - 1.0)
     */
    f32 NextF32() {
        // Random -> [0, 0xFFFF]
        u16 halfw = NextU32() & 0xFFFF;
        // Convert to float
        f32 dec = static_cast<f32>(halfw);
        // Convert to percentage
        return dec / static_cast<f32>(0xFFFF + 1);
    }

    /**
     * @brief Get random float (upper bound)
     *
     * @param max Upper bound (exclusive)
     */
    f32 NextF32(f32 max) {
        return NextF32() * max;
    }

    /**
     * @brief Roll random chance
     *
     * @param p Probability to succeed
     */
    bool Chance(f32 p) {
        K_ASSERT(p <= 1.0f);
        return NextF32() > p;
    }

    /**
     * @brief Roll coin-flip (50% chance)
     */
    bool CoinFlip() {
        return Chance(0.5f);
    }

    /**
     * @brief Roll random sign
     */
    f32 Sign() {
        return Chance(0.5f) ? 1.0f : -1.0f;
    }

private:
    /**
     * @brief Advance the generator
     */
    void Update() {
        mSeed = (mSeed * 0x10DCD) + 1;
    }

private:
    u32 mSeed; // Random seed
};

/**
 * @brief Global Random instance if you don't want to create one
 */
extern Random RNG;

/**
 * @brief Performs the Fisher-Yates shuffle algorithm to generate a random
 * permutation
 *
 * @tparam T Element type
 * @param pArray Input array
 * @param size Array size
 */
template <typename T> K_INLINE void Shuffle(T pArray, int size) {
    K_ASSERT(pArray != nullptr);

    Random r;
    for (int i = size - 1; i >= 1; i--) {
        int j = r.NextS32(i + 1);
        std::swap(pArray[j], pArray[i]);
    }
}

//! @}
} // namespace kiwi

#endif
