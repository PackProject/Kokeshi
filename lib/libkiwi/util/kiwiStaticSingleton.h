#ifndef LIBKIWI_UTIL_STATIC_SINGLETON_H
#define LIBKIWI_UTIL_STATIC_SINGLETON_H
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiNonCopyable.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Thread safe, statically allocated singleton
 */
template <typename T> class StaticSingleton : private NonCopyable {
public:
    /**
     * @brief Gets reference to singleton object
     */
    static T& GetInstance() {
        static T sInstance;
        return sInstance;
    }
};

//! @}
} // namespace kiwi

#endif
