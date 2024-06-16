#ifndef LIBKIWI_UTIL_GLOBAL_INSTANCE_H
#define LIBKIWI_UTIL_GLOBAL_INSTANCE_H
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiAutoLock.h>
#include <revolution/OS.h>

#define K_GLOBAL_INSTANCE_IMPL(T)                                              \
    T* kiwi::GlobalInstance<T>::sInstance = nullptr;                           \
    OSMutex kiwi::GlobalInstance<T>::sMutex;

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Global instance holder
 */
template <typename T> class GlobalInstance {
public:
    /**
     * @brief Constructor
     *
     * @param force Overwrite the current instance
     */
    GlobalInstance(bool force = false) {
        AutoLock<OSMutex> lock(sMutex);

        // Without 'force' we cannot re-register
        if (sInstance != nullptr && !force) {
            K_ASSERT_EX(false, "Created global instance twice");
            return;
        }

        // Register this object as the new instance
        sInstance = (T*)this;
    }

    /**
     * @brief Destructor
     */
    ~GlobalInstance() {
        AutoLock<OSMutex> lock(sMutex);

        // Only unregister if this object is the global instance
        if (sInstance == (T*)this) {
            sInstance = nullptr;
        }
    }

    /**
     * @brief Gets reference to global instance
     */
    static T& Get() {
        AutoLock<OSMutex> lock(sMutex);
        K_ASSERT(sInstance != nullptr);
        return *sInstance;
    }

    /**
     * @brief Change to new global instance
     *
     * @param instance New instance
     */
    static void Set(T& instance) {
        AutoLock<OSMutex> lock(sMutex);
        sInstance = &instance;
    }

    /**
     * @brief Tests whether a global instance is registered
     */
    static bool Exists() {
        AutoLock<OSMutex> lock(sMutex);
        return sInstance != nullptr;
    }

private:
    static T* sInstance;   // Global instance
    static OSMutex sMutex; // Mutex lock for safe access
};

//! @}
} // namespace kiwi

#endif
