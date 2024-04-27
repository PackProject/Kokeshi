#ifndef LIBKIWI_UTIL_AUTOLOCK_H
#define LIBKIWI_UTIL_AUTOLOCK_H
#include <revolution/OS.h>
#include <types.h>

namespace kiwi {

/**
 * Generic scoped lock
 */
template <typename T> class AutoLock : private NonCopyable {
public:
    AutoLock(T& object) : mObject(object) {
        Lock();
    }
    ~AutoLock() {
        Unlock();
    }

    // Implement these for custom types
    void Lock();
    void Unlock();

private:
    // Locked object
    T& mObject;
};

/**
 * OSMutex specializations
 */
template <> inline void AutoLock<OSMutex>::Lock() {
    if (OSGetCurrentThread() != NULL) {
        OSLockMutex(&mObject);
    }
}
template <> inline void AutoLock<OSMutex>::Unlock() {
    if (OSGetCurrentThread() != NULL) {
        OSUnlockMutex(&mObject);
    }
}

typedef AutoLock<OSMutex> AutoMutexLock;

/**
 * OS interrupt scoped lock
 */
class AutoInterruptLock : private NonCopyable {
public:
    AutoInterruptLock(bool enable = false)
        : mEnabled(enable ? OSEnableInterrupts() : OSDisableInterrupts()) {}
    ~AutoInterruptLock() {
        OSRestoreInterrupts(mEnabled);
    }

private:
    // Interrupt status
    BOOL mEnabled;
};

} // namespace kiwi

#endif
