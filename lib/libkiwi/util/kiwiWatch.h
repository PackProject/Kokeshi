#ifndef LIBKIWI_UTIL_STOP_WATCH_H
#define LIBKIWI_UTIL_STOP_WATCH_H
#include <libkiwi/k_types.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Watch timer
 */
class Watch {
public:
    /**
     * @brief Constructor
     */
    Watch() : mStartTick(0) {}

    /**
     * @brief Start the timer
     */
    void Start() {
        mStartTick = OSGetTick();
    }

    /**
     * @brief Amount of time elapsed
     */
    s32 Elapsed() const {
        return OSGetTick() - mStartTick;
    }

private:
    // Start time
    s32 mStartTick;
};

//! @}
} // namespace kiwi

#endif
