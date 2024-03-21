#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RendererLevelDrawer.h>
#include <ZC_Config.h>
#ifdef ZC_IMGUI
#include <ZC_IGWindow.h>
#endif

#include <cassert>

ZC_Renderer::ZC_Renderer(ZC_Function<void()>&& _funcSwapBuffer)
    : funcSwapBuffer(std::move(_funcSwapBuffer))
{
    pCurrentRenderer = this;
}

ZC_Renderer::~ZC_Renderer()
{
    pCurrentRenderer = nullptr;
}

void ZC_Renderer::Add(ZC_RSController* pRSController)
{
    if (!pCurrentRenderer) return;
    auto rendLvlDrawersIter = pCurrentRenderer->rendererLevelDrawers.find(pRSController->rendererLevel);
    if (rendLvlDrawersIter == pCurrentRenderer->rendererLevelDrawers.end()) //  add new level in rendererLevelDrawers and save iterator on it
        rendLvlDrawersIter = pCurrentRenderer->rendererLevelDrawers.emplace(pRSController->rendererLevel, ZC_RendererLevelDrawer::GetRendererLevelDrawer(pRSController->rendererLevel)).first;

    rendLvlDrawersIter->second->Add(pRSController);
}
    
void ZC_Renderer::Erase(ZC_RSController* pRSController)
{
    if (!pCurrentRenderer) return;
    auto rendLvlDrawersIter = pCurrentRenderer->rendererLevelDrawers.find(pRSController->rendererLevel);
    assert(rendLvlDrawersIter != pCurrentRenderer->rendererLevelDrawers.end());   //  can't find Lvl
    //  erase from renderer level, if returns true, renderer level empty and must be erased from rendererLevelDrawers.
    if (rendLvlDrawersIter->second->Erase(pRSController)) pCurrentRenderer->rendererLevelDrawers.erase(rendLvlDrawersIter);
}

void ZC_Renderer::Configure(bool useDepthTest)
{
    if (useDepthTest) bufferCleaner.GlEnable(GL_DEPTH_TEST);
}

void ZC_Renderer::Draw()
{
    bufferCleaner.GlClear();
    ZC_UBOs::Update();
    for (auto& upRendererLevelDrawerPair : rendererLevelDrawers) upRendererLevelDrawerPair.second->Draw(bufferCleaner);
    glBindVertexArray(0);
#ifdef ZC_IMGUI
    ZC_IGWindow::Draw();
#endif
    funcSwapBuffer();
}

// void ZC_Renderer::DrawTextWidndow(std::map<ZC_ShProg*, std::forward_list<ZC_RendererSet*>>& mapShPRS)
// {
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     for (auto pairShPRS : mapShPRS)
//     {
//         pairShPRS.first->UseProgram();
//         for (auto pRendSet : pairShPRS.second) pRendSet->Draw(ZC_RL_TextWindow);
//     }
//     glDisable(GL_BLEND);
// }


