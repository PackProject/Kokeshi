#ifndef LIBKIWI_PRIM_PAIR_H
#define LIBKIWI_PRIM_PAIR_H
#include <libkiwi/k_types.h>

namespace kiwi {

/**
 * @brief std::pair replacement
 */
template <typename T1, typename T2> class TPair {
public:
    TPair(const T1& t1, const T2& t2) : first(t1), second(t2) {}

public:
    T1 first;
    T2 second;
};

} // namespace kiwi

#endif
