#include <ZC/Video/OpenGL/Renderer/ZC_DSStencilBorder.h>

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

ZC_DSStencilBorder::ZC_DSStencilBorder()
    : ZC_DrawerFL(GL_STENCIL_BUFFER_BIT, ZC_GLDepth(true), ZC_GLStencil(true), ZC_GLBlend(false), ZC_GLCullFace(true))
{}

void ZC_DSStencilBorder::VCallDraw()
{
    ZC_GLStencil::GLStencilFuncSeparate(GL_FRONT_AND_BACK, GL_ALWAYS, 1u, 255u);
    ZC_GLStencil::GLStencilOpSeparate(GL_FRONT_AND_BACK, GL_REPLACE, GL_REPLACE, GL_REPLACE);
    ZC_GLStencil::GLStencilMaskSeparate(GL_FRONT_AND_BACK, 255u);
    // glStencilFunc(GL_ALWAYS, 1, 255);
    // glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    // glStencilMask(255);

    this->fl.Draw();    //  draw as usual

    ZC_GLStencil::GLStencilFuncSeparate(GL_FRONT_AND_BACK, GL_NOTEQUAL, 1u, 255u);
    // glStencilFunc(GL_NOTEQUAL, 1, 255);
    
    static ZC_ShProg* pShPStencilBorder = &(ZC_ShProgs::Get(ShPN_ZCR_StencilBorder)->shProg);

    pShPStencilBorder->ActivateOpenGL();    //  use stencil shader program
    this->fl.Draw(false, true, false);
}