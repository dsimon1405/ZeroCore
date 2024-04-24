#include <ZC/Video/OpenGL/Renderer/ZC_RenderLevelDrawer.h>

#include <ZC/Video/OpenGL/Renderer/ZC_RLDDrawing.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RLDStencilBorder.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RLDText.h>

#include <cassert>

ZC_uptr<ZC_RenderLevelDrawer> ZC_RenderLevelDrawer::GetRendererLevelDrawer(ZC_DrawLevel lvl)
{
    switch (lvl)
    {
    case ZC_DrawLevels::Drawing: return { new ZC_RLDDrawing() };
    case ZC_DrawLevels::TextScene: return { new ZC_RLDTextScene() };
    case ZC_DrawLevels::StencilBorder: return { new ZC_RLDStencilBorder() };
    case ZC_DrawLevels::OrthoBlend: return { new ZC_RLDOrthoBlend() };
    case ZC_DrawLevels::TextWindowIntoScene: return { new ZC_RLDTextWindowIntoScene() };
    default:
    {
        if (funcUsers_GetRendererLevelDrawer)
        {
            auto upUsersRendererLevelDrawer = funcUsers_GetRendererLevelDrawer(lvl);
            if (upUsersRendererLevelDrawer) return upUsersRendererLevelDrawer;
            else assert(false);     //  users function don't provide class/struct for requested lvl
        }
        else assert(false);    //  can't find class/struct for requested lvl
    }
    }
    return nullptr;
}

void ZC_RenderLevelDrawer::SetUsersFunction_GetRendererLevelDrawer(ZC_Function<ZC_uptr<ZC_RenderLevelDrawer>(ZC_DrawLevel)>&& func)
{
    funcUsers_GetRendererLevelDrawer = std::move(func);
}


//  ZC_TexturesHolder

bool ZC_TexturesHolder::operator == (const ZC_TexturesHolder& th) const noexcept
{
    return pTexture == th.pTexture;
}

void ZC_TexturesHolder::ActivateOpenGL() const
{
    for (uint i = 0; i < static_cast<uint>(texturesCount); i++) pTexture->ActiveTexture(i);
}


//  ZC_Uniforms_GLDraw

bool ZC_RLDData_Uniforms_GLDraw::operator == (const ZC_RLDData_Uniforms_GLDraw& unifAndGLDraw) const noexcept
{
    return pUniforms == unifAndGLDraw.pUniforms && pGLDraw == unifAndGLDraw.pGLDraw;
}

void ZC_RLDData_Uniforms_GLDraw::Draw() const
{
    if (pUniforms) pUniforms->Activate();
    pGLDraw->Draw();
}