#ifndef LIBKIWI_UTIL_BUILDTARGET_H
#define LIBKIWI_UTIL_BUILDTARGET_H
#include <libkiwi/kernel/kiwiAssert.h>
#include <types.h>

namespace kiwi {

String GetBuildDate();
String GetBuildPack();
String GetBuildTarget();

} // namespace kiwi

#endif
