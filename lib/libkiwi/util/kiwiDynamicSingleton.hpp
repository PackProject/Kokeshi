#ifndef LIBKIWI_UTIL_DYNAMIC_SINGLETON_H
#define LIBKIWI_UTIL_DYNAMIC_SINGLETON_H
#include <libkiwi/util/kiwiAutoLock.hpp>
#include <libkiwi/util/kiwiNonCopyable.hpp>
#include <revolution/OS.h>
#include <types.h>

#define K_DYNAMIC_SINGLETON_IMPL(T)                                            \
    T* kiwi::DynamicSingleton<T>::sInstance = NULL;                            \
    OSMutex kiwi::DynamicSingleton<T>::sMutex;

namespace kiwi {

/**
 * Thread safe, dynamically allocated singleton
 */
template <typename T> class DynamicSingleton : private NonCopyable {
public:
    /**
     * Gets reference to singleton object
     */
    static T& GetInstance() {
        AutoLock<OSMutex> lock(sMutex);
        K_ASSERT(sInstance != NULL);
        return *sInstance;
    }

    /**
     * Initializes singleton object
     */
    static void CreateInstance() {
        AutoLock<OSMutex> lock(sMutex);
        K_ASSERT_EX(sInstance == NULL, "Created singleton twice");
        if (sInstance == NULL) {
            sInstance = new T();
        }
    }

    /**
     * Destroys singleton object
     */
    static void DestroyInstance() {
        AutoLock<OSMutex> lock(sMutex);
        delete sInstance;
    }

private:
    // Static instance
    static T* sInstance;
    // Mutex lock for initialization
    static OSMutex sMutex;
};

} // namespace kiwi

#endif
