#include <libkiwi.h>
#include <revolution/OS.h>

namespace kiwi {

/**
 * @brief Initial guard value
 */
u32 StackChecker::sStackCheckGuard = 0;

/**
 * @brief Initialize initial guard value
 */
void StackChecker::Initialize() {
    K_ASSERT(sStackCheckGuard == 0);
    sStackCheckGuard = OSGetTick();
}

} // namespace kiwi
