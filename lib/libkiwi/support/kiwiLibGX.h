#ifndef LIBKIWI_SUPPORT_LIBGX_H
#define LIBKIWI_SUPPORT_LIBGX_H
#include <libkiwi/k_types.h>
#include <revolution/GX.h>

namespace kiwi {

/**
 * GX library wrapper/extension
 */
class LibGX {
public:
    static const GXRenderModeObj* GetDefaultRenderMode();
};

} // namespace kiwi

#endif
