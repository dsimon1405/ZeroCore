#include <ZC/Video/OpenGL/Renderer/ZC_Drawer.h>

#include <ZC/Video/OpenGL/Renderer/ZC_DSStencilBorder.h>

#include <cassert>

ZC_uptr<ZC_Drawer> ZC_Drawer::GetRendererLevelDrawer(ZC_DrawerLevel lvl)
{
    switch (lvl)
    {
    case ZC_DrawerLevels::Drawing: return { new ZC_DrawerFL<const ZC_ShProg*, const ZC_VAO*, ZC_TexturesHolder, ZC_RLDData_Uniforms_GLDraw>(0, true, true, false, {}) };
    case ZC_DrawerLevels::StencilBorder: return { new ZC_DSStencilBorder() };
    case ZC_DrawerLevels::TextScene: return
        { new ZC_DrawerFL<const ZC_ShProg*, ZC_TexturesHolder, const ZC_VAO*, ZC_RLDData_Uniforms_GLDraw>(0, true, true, false, ZC_GLBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)) };
    case ZC_DrawerLevels::TextWindowIntoScene: return { new ZC_DrawerFL<const ZC_ShProg*, ZC_TexturesHolder, const ZC_VAO*, ZC_RLDData_Uniforms_GLDraw>
        (GL_DEPTH_BUFFER_BIT, true, true, false, ZC_GLBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)) };
    case ZC_DrawerLevels::OrthoBlend: return
        { new ZC_DrawerFL<const ZC_ShProg*, ZC_TexturesHolder, const ZC_VAO*, ZC_RLDData_Uniforms_GLDraw>(0, false, true, false, ZC_GLBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)) };
    default:
    {
        if (funcUsers_GetDrawer)
        {
            auto upUsersRendererLevelDrawer = funcUsers_GetDrawer(lvl);
            if (upUsersRendererLevelDrawer) return upUsersRendererLevelDrawer;
            else assert(false);     //  users function don't provide class/struct for requested lvl
        }
        else assert(false);    //  can't find class/struct for requested lvl
    }
    }
    return nullptr;
}

void ZC_Drawer::SetUsersFunction_GetRendererLevelDrawer(ZC_Function<ZC_uptr<ZC_Drawer>(ZC_DrawerLevel)>&& func)
{
    funcUsers_GetDrawer = std::move(func);
}