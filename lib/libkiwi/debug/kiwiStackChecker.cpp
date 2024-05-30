#include <libkiwi.h>
#include <revolution/OS.h>

namespace kiwi {

u32 StackChecker::sStackCheckGuard = 0;

/**
 * @brief Initialize guard variable
 */
void StackChecker::Initialize() {
    K_ASSERT(sStackCheckGuard == 0);
    sStackCheckGuard = OSGetTick();
}

} // namespace kiwi
