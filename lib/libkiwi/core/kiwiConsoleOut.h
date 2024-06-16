#ifndef LIBKIWI_CORE_CONSOLE_OUT_H
#define LIBKIWI_CORE_CONSOLE_OUT_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiString.h>
#include <libkiwi/util/kiwiNonCopyable.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

namespace detail {
//! @addtogroup libkiwi_core
//! @{

/**
 * @brief Console text output stream
 * @note This class cannot be instantiated. Please use the global kiwi::cout
 * instance.
 */
class ConsoleOut : private NonCopyable {
public:
    /**
     * @brief Logs output to the console
     *
     * @param rValue Value (converted to string)
     */
    template <typename T> const ConsoleOut& operator<<(const T& rValue) const {
        K_LOG(ToString(rValue));
        return *this;
    }
};

//! @}
} // namespace detail

//! Global console handle
extern const detail::ConsoleOut cout;
//! Newline character
static const char endl = '\n';

//! @}
} // namespace kiwi

#endif
