#include <ZC/Video/OpenGL/Renderer/ZC_RenderLevelDrawer.h>

#include <ZC/Video/OpenGL/Renderer/ZC_RLDDrawing.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RLDStencilBorder.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RLDText.h>

#include <ZC/Video/OpenGL/Renderer/ZC_RDComplex.h>

#include <cassert>

ZC_uptr<ZC_RenderLevelDrawer> ZC_RenderLevelDrawer::GetRendererLevelDrawer(ZC_DrawLevel lvl)
{
    switch (lvl)
    {
    // case ZC_DrawLevels::Drawing: return { new ZC_RDComplex<const ZC_ShProg*, const ZC_VAO*, ZC_TexturesHolder, ZC_RLDData_Uniforms_GLDraw>(ZC_OGLP_GL_DEPTH_TEST) };
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