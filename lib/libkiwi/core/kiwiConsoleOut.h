#ifndef LIBKIWI_CORE_CONSOLE_OUT_H
#define LIBKIWI_CORE_CONSOLE_OUT_H
#include <libkiwi/prim/kiwiString.h>
#include <libkiwi/util/kiwiNonCopyable.h>
#include <types.h>

namespace kiwi {
namespace detail {

/**
 * @brief Console output stream
 */
class ConsoleOut : private NonCopyable {
public:
    ConsoleOut() {}
    ~ConsoleOut() {}

    /**
     * @brief Output pipe operator
     *
     * @param t Value (attempted to convert to string)
     * @return ConsoleOut reference
     */
    template <typename T> const ConsoleOut& operator<<(const T& t) const {
        K_LOG(ToString(t));
        return *this;
    }
};

} // namespace detail

/**
 * @brief Global console handle
 */
extern const detail::ConsoleOut cout;

/**
 * @brief Newline character
 */
const char endl = '\n';

} // namespace kiwi

#endif
