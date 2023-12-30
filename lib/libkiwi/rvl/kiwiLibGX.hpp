#ifndef LIBKIWI_RVL_LIBGX_HPP
#define LIBKIWI_RVL_LIBGX_HPP
#include <revolution/GX.h>
#include <types.h>

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
