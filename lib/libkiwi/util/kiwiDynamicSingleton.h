#ifndef LIBKIWI_UTIL_DYNAMIC_SINGLETON_H
#define LIBKIWI_UTIL_DYNAMIC_SINGLETON_H
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiAutoLock.h>
#include <libkiwi/util/kiwiNonCopyable.h>
#include <revolution/OS.h>

#define K_DYNAMIC_SINGLETON_IMPL(T)                                            \
    T* kiwi::DynamicSingleton<T>::sInstance = nullptr;                         \
    OSMutex kiwi::DynamicSingleton<T>::sMutex;

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Thread safe, dynamically allocated singleton
 */
template <typename T> class DynamicSingleton : private NonCopyable {
public:
    /**
     * @brief Gets reference to singleton object
     */
    static T& GetInstance() {
        AutoLock<OSMutex> lock(sMutex);
        K_ASSERT(sInstance != nullptr);
        return *sInstance;
    }

    /**
     * @brief Initializes singleton object
     */
    static void CreateInstance() {
        AutoLock<OSMutex> lock(sMutex);
        K_ASSERT_EX(sInstance == nullptr, "Created singleton twice");
        if (sInstance == nullptr) {
            sInstance = new T();
        }
    }

    /**
     * @brief Destroys singleton object
     */
    static void DestroyInstance() {
        AutoLock<OSMutex> lock(sMutex);
        delete sInstance;
    }

private:
    static T* sInstance;   // Static instance
    static OSMutex sMutex; // Mutex lock for safe access
};

//! @}
} // namespace kiwi

#endif
