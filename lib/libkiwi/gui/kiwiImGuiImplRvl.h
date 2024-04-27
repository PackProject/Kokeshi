/**
 * References: Wii U (GX2) implementation by GaryOderNichts
 */

#ifndef LIBKIWI_GUI_IMGUI_IMPL_RVL_H
#define LIBKIWI_GUI_IMGUI_IMPL_RVL_H
#include <Pack/RPGraphics.h>
#include <libkiwi/core/kiwiSceneHookMgr.h>
#include <libkiwi/gui/kiwiImGui.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>
#include <revolution/GX.h>
#include <types.h>

namespace kiwi {

/**
 * @brief Revolution backend for ImGui
 */
class ImGuiImplRvl : public DynamicSingleton<ImGuiImplRvl>,
                     public ISceneHook,
                     public IRPGrpDrawObject {
    friend class DynamicSingleton<ImGuiImplRvl>;

public:
    ImGuiImplRvl();
    ~ImGuiImplRvl();

    virtual void UserDraw();

private:
    virtual void BeforeCalculate(RPSysScene* scene);
    virtual void AfterCalculate(RPSysScene* scene);

    void Init();
    void CreateFontTexture();
    void SetupGX() const;
    void RenderDrawData();

private:
    ImDrawVert* mpVtxBuffer;
    u32 mNumVtx;
    GXTexObj mFontTexObj;
};

} // namespace kiwi

#endif
