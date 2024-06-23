#ifndef RP_SYSTEM_RENDER_MODE_H
#define RP_SYSTEM_RENDER_MODE_H
#include <Pack/RPTypes.h>
#include <revolution/GX.h>

//! @addtogroup rp_system
//! @{

/**
 * @brief Render mode utilities
 */
class RPSysRenderMode {
public:
    /**
     * @brief Render mode formats
     */
    enum EFormat {
        EFormat_NTSC,      //!< NTSC
        EFormat_NTSC_Prog, //!< NTSC, Progressive Mode

        EFormat_PAL50,    //!< PAL, 50Hz ("EuRgb50")
        EFormat_PAL60,    //!< PAL, 60Hz ("EuRgb60")
        EFormat_PAL_Prog, //!< PAL, Progressive Mode

        EFormat_Max
    };

public:
    /**
     * @brief Gets the render mode for the specified format
     *
     * @param fmt Render mode type
     */
    static GXRenderModeObj* getRenderMode(EFormat fmt) {
        return sRenderModeTable[fmt];
    }

    /**
     * @brief Adjusts a render mode to account for widescreen/50Hz formats
     *
     * @param pOriginX Pointer to render mode viXOrigin
     * @param pWidth Pointer to render mode viWidth
     * @param fmt Render mode type
     * @param wide Whether the display is widescreen
     */
    static void adjust(u16* pOriginX, u16* pWidth, EFormat fmt, bool wide);

private:
    //! Render mode configurations for each render mode format
    static GXRenderModeObj* sRenderModeTable[EFormat_Max];
};

//! @}

#endif
