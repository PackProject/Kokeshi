#ifndef LIBKIWI_DEBUG_STACK_CHECKER_H
#define LIBKIWI_DEBUG_STACK_CHECKER_H
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_debug
//! @{

/**
 * @brief Buffer overflow checker
 * @details Declare one after your buffer to check for overflows
 */
class StackChecker {
public:
    /**
     * @brief Constructor
     */
    StackChecker() : mGuard(0) {
        // Lazy initialization
        if (sStackCheckGuard == 0) {
            Initialize();
        }

        mGuard = sStackCheckGuard;
    }

    /**
     * @brief Destructor
     */
    ~StackChecker() {
        K_ASSERT_EX(mGuard == sStackCheckGuard, "Buffer overflow detected!");
    }

private:
    static void Initialize();

private:
    // Initial guard value
    static u32 sStackCheckGuard;

    //! Guard value on the stack
    volatile u32 mGuard;
};

//! @}
} // namespace kiwi

#endif
