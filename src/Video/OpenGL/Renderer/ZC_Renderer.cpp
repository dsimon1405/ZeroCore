#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Video/OpenGL/ZC_GLBlend.h>
#include <ZC_Config.h>
#ifdef ZC_IMGUI
#include <ZC_IGWindow.h>
#endif

#include <cassert>

ZC_Renderer::ZC_Renderer(ZC_Function<void()>&& _funcSwapBuffer)
    : funcSwapBuffer(std::move(_funcSwapBuffer))
{
    assert(funcSwapBuffer);
    pRenderer = this;
}

ZC_Renderer::~ZC_Renderer()
{
    pRenderer = nullptr;
}

void ZC_Renderer::AddUBOs(ZC_UBO* ubo, ZC_Function<void()>&& fUpdate)
{
    if (pRenderer) pRenderer->AddUBO(ubo, std::move(fUpdate));
}

void ZC_Renderer::EraseUBOs(ZC_UBO* pUbo)
{
    if (pRenderer) pRenderer->EraseUBO(pUbo);
}

void ZC_Renderer::Add(ZC_Render* pRender)
{
    if (!pRenderer || !pRender) return;

    auto prevIter = pRenderer->renders.before_begin();
    for (auto curIter = pRenderer->renders.begin(); curIter != pRenderer->renders.end(); )
    {
        if (**curIter < (*pRender).renderLevel)
        {
            prevIter = curIter;
            ++curIter;
            continue;
        }
        else
        {
            assert(**curIter != (*pRender).renderLevel);     //  double adding
            break;
        }
    }
    pRenderer->renders.emplace_after(prevIter, pRender);
}

void ZC_Renderer::Erase(ZC_Render* pRender)
{
    if (!pRenderer || !pRender) return;
    ZC_ForwardListErase(pRenderer->renders, pRender);
}

void ZC_Renderer::Draw()
{
    this->UpdateUBO();

    auto prevIter = renders.before_begin();
    for (auto curIter = renders.begin(); curIter != renders.end(); )
        curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

    //  my rendering finished, make openGL state default
    ZC_VAO::UnbindVertexArray();
    ZC_GLBlend::Disable();
    ZC_Framebuffer::Unbind();

#ifdef ZC_IMGUI
    ZC_IGWindow::Draw();
#endif

    funcSwapBuffer();
}
// #include <ZC/Video/ZC_Window.h>
// #include <ZC/Tools/Console/ZC_cout.h>
// void ZC_Renderer::Draw()
// {
//     this->UpdateUBO();

//     auto prevIter = renders.before_begin();
//     for (auto curIter = renders.begin(); curIter != renders.end(); )
//         curIter = (*curIter)->Draw() ? ++curIter : renders.erase_after(prevIter);

//     // uchar data[4];
//     float data;
//     float x, y;
//     ZC_Window::GetCursorPosition(x, y);
//     static float _x, _y;
//     if (x != _x || y != _y)
//     {
//         _x = x;
//         _y = y;
//         glReadPixels(x,y,1,1,  GL_DEPTH_COMPONENT,  GL_FLOAT, &data);
//         // glReadPixels(x,y,1,1,  GL_DEPTH_STENCIL,  GL_UNSIGNED_INT_24_8, &data);
//         ZC_cout(std::to_string(data));
//         // int data1 = ((*(int*)data) >> 8) & 16777215;
//         // ZC_cout(std::to_string(data1));
//         // ZC_cout(std::to_string(data[0]) + "     " + std::to_string(data[1]) + "     " + std::to_string(data[2]));
//     }

// #ifdef ZC_IMGUI
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
//     ZC_IGWindow::Draw();
// #endif
//     glBindVertexArray(0);

//     funcSwapBuffer();
// }