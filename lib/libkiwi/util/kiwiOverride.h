#ifndef LIBKIWI_UTIL_OVERRIDE_H
#define LIBKIWI_UTIL_OVERRIDE_H
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiNonCopyable.h>

namespace kiwi {

/**
 * Override class without extending the base class's layout.
 * Hacky but useful for modifying behavior in existing game classes.
 */
template <typename T> class Override : private NonCopyable {
public:
    operator T*() {
        return BasePtr();
    }

    const operator T*() const {
        return BasePtr();
    }

    T* BasePtr() {
        return reinterpret_cast<T*>(this);
    }

    const T* BasePtr() const {
        return reinterpret_cast<T*>(this);
    }

    T& BaseRef() {
        return *BasePtr();
    }

    const T& BaseRef() const {
        return *BasePtr();
    }
};

} // namespace kiwi

#endif
