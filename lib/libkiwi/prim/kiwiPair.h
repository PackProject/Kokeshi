#ifndef LIBKIWI_PRIM_PAIR_H
#define LIBKIWI_PRIM_PAIR_H
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_prim
//! @{

/**
 * @brief std::pair replacement
 */
template <typename TFirst, typename TSecond> struct TPair {
    /**
     * @brief Constructor
     *
     * @param rFirst First element
     * @param rSecond Second element
     */
    TPair(const TFirst& rFirst, const TSecond& rSecond)
        : first(rFirst), second(rSecond) {}

    //! First element
    TFirst first;
    //! Second element
    TSecond second;
};

namespace {
//! @addtogroup libkiwi_prim
//! @{

/**
 * @brief Pair construction helper
 *
 * @param rFirst First element
 * @param rSecond Second element
 */
template <typename TFirst, typename TSecond>
TPair<TFirst, TSecond> MakePair(const TFirst& rFirst, const TSecond& rSecond) {
    return TPair<TFirst, TSecond>(rFirst, rSecond);
}

//! @}
} // namespace
//! @}
} // namespace kiwi

#endif
