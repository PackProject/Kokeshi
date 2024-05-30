#ifndef LIBKIWI_UTIL_NONCOPYABLE_H
#define LIBKIWI_UTIL_NONCOPYABLE_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {

/**
 * Non-copyable mix-in
 */
class NonCopyable {
protected:
    NonCopyable() {}
    ~NonCopyable() {}

private:
    NonCopyable(const NonCopyable& other) {
        K_ASSERT(false);
    }
    NonCopyable& operator=(const NonCopyable& other) {
        K_ASSERT(false);
        return *this;
    }
};

} // namespace kiwi

#endif
