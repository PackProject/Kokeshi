#include <Pack/RPGraphics.h>
#include <Pack/RPKernel.h>

#include <libkiwi.h>

namespace kiwi {

/**
 * @brief Setup scene
 */
void IScene::Configure() {
#if defined(PACK_SPORTS) || defined(PACK_PLAY)
    // Setup model scene
    RP_GET_INSTANCE(RPGrpModelResManager)->CreateResourceList(0x400);
    RP_GET_INSTANCE(RPGrpModelManager)
        ->CreateModelScene(0, 0, 1, nullptr, nullptr);

    // Setup renderer view
    RPGrpScreen* pScreen = new RPGrpScreen();
    K_ASSERT(pScreen != nullptr);
    pScreen->SetCanvasMode(RPGrpScreen::CANVASMODE_CC);

    RPGrpRenderer::GetCurrent()->CreateView2D(1, pScreen);
    RPGrpRenderer::GetCurrent()->CorrectView();
#endif

    // User state function
    OnConfigure();
}

/**
 * @brief Load required assets
 */
void IScene::LoadResource() {
    // User state function
    OnLoadResource();
}

/**
 * @brief Reload scene
 */
void IScene::Reset() {
    // User state function
    OnReset();
}

/**
 * @brief Scene logic
 */
void IScene::Calculate() {
    // User state function
    OnCalculate();
}

#if defined(PACK_RESORT)
/**
 * @brief Scene MotionPlus logic
 */
void IScene::CalculateMPlus() {
    // User state function
    OnCalculateMPlus();
}
#endif

/**
 * @brief Exit scene
 */
void IScene::Exit() {
    // User state function
    OnExit();
}

/**
 * @brief User-level draw
 */
void IScene::UserDraw() {
    // User state function
    OnUserDraw();
}

/**
 * @brief Debug-level draw
 */
void IScene::DebugDraw() {
    // User state function
    OnDebugDraw();
}

} // namespace kiwi
