#ifndef LIBKIWI_UTIL_BUILDTARGET_H
#define LIBKIWI_UTIL_BUILDTARGET_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {

String GetBuildDate();
String GetBuildPack();
String GetBuildTarget();

} // namespace kiwi

#endif
