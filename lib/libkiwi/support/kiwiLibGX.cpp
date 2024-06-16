#include <libkiwi.h>
#include <revolution/VI.h>

namespace kiwi {

/**
 * @brief Gets "default" GX render mode based on VI format
 */
const GXRenderModeObj* LibGX::GetDefaultRenderMode() {
    switch (VIGetTvFormat()) {
    case VI_TV_FMT_NTSC:    return &GXNtsc480IntDf;
    case VI_TV_FMT_PAL:     return &GXPal528IntDf;
    case VI_TV_FMT_EURGB60: return &GXEurgb60Hz480IntDf;
    case VI_TV_FMT_MPAL:    return &GXMpal480IntDf;
    default:                return nullptr;
    }
}

} // namespace kiwi
