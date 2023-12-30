#include <kokeshi.hpp>
#include <libkiwi.h>

namespace kiwi {

/**
 * Gets module build date
 */
String GetBuildDate() {
    return __DATE__ " " __TIME__;
}

/**
 * Gets module build pack as a string
 */
String GetBuildPack() {
    // clang-format off
    return KOKESHI_BY_PACK("Wii Sports",
                           "Wii Play",
                           "Wii Sports Resort");
    // clang-format on
}

/**
 * Gets module build target as a string
 */
String GetBuildTarget() {
#ifdef NDEBUG
    return "Release";
#else
    return "Debug";
#endif
}

} // namespace kiwi
