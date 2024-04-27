/**
 * References: Wii U (GX2) implementation by GaryOderNichts
 */

#include <Pack/RPGraphics.h>
#include <libkiwi.h>
#include <revolution/GX.h>

namespace kiwi {
namespace {

/**
 * @brief Alloc function for ImGui
 */
void* AllocMemory(u32 size, void* userData) {
#pragma unused(userData)
    return new (EMemory_MEM2) u8[size];
}

/**
 * @brief Free function for ImGui
 */
void FreeMemory(void* ptr, void* userData) {
#pragma unused(userData)
    delete static_cast<u8*>(ptr);
}

} // namespace

K_DYNAMIC_SINGLETON_IMPL(ImGuiImplRvl);

/**
 * @brief Constructor
 */
ImGuiImplRvl::ImGuiImplRvl() : ISceneHook(-1), mpVtxBuffer(NULL), mNumVtx(0) {
    // Setup ImGui library
    Init();
    CreateFontTexture();

    // Register this with the RP engine
    RPGrpRenderer::GetInstance()->AppendDrawObject(this);
}

/**
 * @brief Destructor
 */
ImGuiImplRvl::~ImGuiImplRvl() {
    delete mpVtxBuffer;
    mpVtxBuffer = NULL;
}

/**
 * @brief Update before scene
 */
void ImGuiImplRvl::BeforeCalculate(RPSysScene* scene) {
#pragma unused(scene)

    // Prepare for this frame
    imgui::NewFrame();
}

/**
 * @brief Update after scene
 */
void ImGuiImplRvl::AfterCalculate(RPSysScene* scene) {
#pragma unused(scene)

    // Create draw data for this frame
    imgui::Render();
}

/**
 * @brief Render UI elements
 */
void ImGuiImplRvl::UserDraw() {
    // Draw GUI during layout drawpass
    if (!RPGrpRenderer::IsDrawLayout()) {
        return;
    }

    // No draw data sometimes if these are called out of order
    if (imgui::GetDrawData() == NULL) {
        return;
    }

    SetupGX();
    RenderDrawData();
}

/**
 * @brief Initialize ImGui library
 */
void ImGuiImplRvl::Init() {
    // Set allocator functions
    imgui::SetAllocatorFunctions(AllocMemory, FreeMemory);

    // Initialize ImGui context
    imgui::CreateContext();
    imgui::GetIO().ConfigFlags |=
        ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableGamepad;

    // Dark theme!!! :D
    imgui::StyleColorsDark();

    // Setup display
    const GXRenderModeObj* rmode = LibGX::GetDefaultRenderMode();
    K_ASSERT(rmode != NULL);
    imgui::GetIO().DisplaySize.x = rmode->fbWidth;
    imgui::GetIO().DisplaySize.y = rmode->xfbHeight;
}

/**
 * @brief Create GX texture for font
 */
void ImGuiImplRvl::CreateFontTexture() {
    ImGuiIO& io = imgui::GetIO();

    // Get font texture data
    u8* image;
    int w, h;
    io.Fonts->GetTexDataAsRGBA32(&image, &w, &h);
    K_ASSERT_EX(w <= 1024 && h <= 1024, "Texture is too large for GX");

    // Build GX texture
    GXInitTexObj(&mFontTexObj, image, w, h, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP,
                 FALSE);
    GXLoadTexObj(&mFontTexObj, GX_TEXMAP0);

    io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(GX_TEXMAP0));
}

/**
 * @brief Setup GX render state
 */
void ImGuiImplRvl::SetupGX() const {
    const ImDrawData* data = imgui::GetDrawData();
    K_ASSERT(data != NULL);
    K_ASSERT(data->Valid);

    // Alpha-blending enabled, no face culling, no depth testing
    GXSetBlendMode(GX_BM_BLEND, GX_BL_INVDSTALPHA, GX_BL_DSTALPHA, GX_LO_SET);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(FALSE, GX_NEVER, FALSE);

    // Setup viewport
    GXSetViewport(0.0f, 0.0f, data->DisplaySize.x * data->FramebufferScale.x,
                  data->DisplaySize.y * data->FramebufferScale.y, 0.0f, 1.0f);

    // Setup orthographic projection
    f32 t = data->DisplayPos.y;
    f32 b = data->DisplayPos.y + data->DisplaySize.y;
    f32 l = data->DisplayPos.x;
    f32 r = data->DisplayPos.x + data->DisplaySize.x;
    nw4r::math::MTX34 proj;
    C_MTXOrtho(proj, t, b, l, r, 0.0f, 1.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
}

/**
 * @brief Render GUI vertex data
 */
void ImGuiImplRvl::RenderDrawData() {
    const ImDrawData* data = imgui::GetDrawData();
    K_ASSERT(data != NULL);
    K_ASSERT(data->Valid);

    // // Reallocate buffers if needed
    // if (mNumVtx < data->TotalVtxCount) {
    //     delete mpVtxBuffer;
    //     mpVtxBuffer =
    //         new (32, EMemory_MEM2) ImDrawVert[mNumVtx = data->TotalVtxCount];
    // }

    // // Create contiguous buffers (for FIFO)
    // for (int i = 0, ofs = 0; i < data->CmdListsCount; i++) {
    //     const ImDrawList* list = data->CmdLists[i];
    //     K_ASSERT(list != NULL);

    //     // Vertex data
    //     std::memcpy(mpVtxBuffer + ofs, list->VtxBuffer.Data,
    //                 list->VtxBuffer.Size * sizeof(ImDrawVert));
    //     ofs += list->VtxBuffer.Size;
    // }

    // Prepare for setting up vertex format
    GXInvalidateVtxCache();
    GXClearVtxDesc();

    // Describe ImDrawVert format
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    // Data is indexed by ImDrawIdx
    K_STATIC_ASSERT_EX(sizeof(ImDrawIdx) == sizeof(u16),
                       "ImGui can support 32-bit index, but GX can't");
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);

    // Render command lists
    for (int i = 0; i < data->CmdListsCount; i++) {
        const ImDrawList* list = data->CmdLists[i];
        K_ASSERT(list != NULL);

        // Process commands in this list
        for (int j = 0; j < list->CmdBuffer.Size; j++) {
            const ImDrawCmd& cmd = list->CmdBuffer[j];

            // References for shorthand
            const ImVec4& clip = cmd.ClipRect;
            const ImVec2& dispPos = data->DisplayPos;
            const ImVec2& dispSize = data->DisplaySize;
            const ImVec2& fbScale = data->FramebufferScale;

            // Call the function instead of rendering
            if (cmd.UserCallback != NULL) {
                cmd.UserCallback(list, &cmd);
                continue;
            }

            // Nothing to render
            if (cmd.ElemCount == 0) {
                continue;
            }

            // Project clipping into framebuffer space
            ImVec2 cmin((clip.x - dispPos.x) * fbScale.x,
                        (clip.y - dispPos.y) * fbScale.y);

            ImVec2 cmax((clip.z - dispPos.x) * fbScale.x,
                        (clip.w - dispPos.y) * fbScale.y);

            // Invalid clipping
            if (cmax.x <= cmin.x || cmax.y <= cmin.y) {
                continue;
            }

            // Invalid draws
            if (cmin.x < 0.0f || cmin.y < 0.0f ||
                cmax.x > dispSize.x * fbScale.x ||
                cmax.y > dispSize.y * fbScale.y) {
                continue;
            }

            // Apply clipping
            GXSetScissor(static_cast<u32>(cmin.x), static_cast<u32>(cmin.y),
                         static_cast<u32>(cmax.x - cmin.x),
                         static_cast<u32>(cmax.y - cmin.y));

            // Provide indexed vertex data
            GXSetArray(GX_VA_POS, &list->VtxBuffer.Data->pos,
                       sizeof(ImDrawVert));
            GXSetArray(GX_VA_CLR0, &list->VtxBuffer.Data->col,
                       sizeof(ImDrawVert));
            GXSetArray(GX_VA_TEX0, &list->VtxBuffer.Data->uv,
                       sizeof(ImDrawVert));

            // Send indexes to the FIFO
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, cmd.ElemCount);
            {
                for (int i = 0; i < cmd.ElemCount; i++) {
                    ImDrawIdx j = list->IdxBuffer[cmd.IdxOffset];

                    GXPosition1x16(j);
                    GXColor1x16(j);
                    GXTexCoord1x16(j);
                }
            }
            GXEnd();
        }
    }
}

} // namespace kiwi
