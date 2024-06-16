#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Gets the date and time at which the module was built
 */
const char* GetBuildDate() {
    return __DATE__ " " __TIME__;
}

/**
 * @brief Gets the pack for which the module was built
 */
const char* GetBuildPack() {
    // clang-format off
    return KOKESHI_BY_PACK("Wii Sports",
                           "Wii Play",
                           "Wii Sports Resort");
    // clang-format on
}

/**
 * @brief Gets the target for which the module was built
 */
const char* GetBuildTarget() {
#ifdef NDEBUG
    return "Release";
#else
    return "Debug";
#endif
}

} // namespace kiwi
