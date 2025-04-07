#ifndef LIBKIWI_UTIL_NON_COPYABLE_H
#define LIBKIWI_UTIL_NON_COPYABLE_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Non-copyable mix-in
 */
class NonCopyable {
protected:
    NonCopyable() {}
    ~NonCopyable() {}

private:
    NonCopyable(const NonCopyable& /* rOther */) {
        K_ASSERT(false);
    }
    NonCopyable& operator=(const NonCopyable& /* rOther */) {
        K_ASSERT(false);
        return *this;
    }
};

//! @}
} // namespace kiwi

#endif
