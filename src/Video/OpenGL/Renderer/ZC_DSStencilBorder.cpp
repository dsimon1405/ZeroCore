#include <ZC/Video/OpenGL/Renderer/ZC_DSStencilBorder.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

ZC_DSStencilBorder::ZC_DSStencilBorder()
    : ZC_DrawerFL(GL_STENCIL_BUFFER_BIT, true, true, {})
{}

void ZC_DSStencilBorder::VCallDraw()
{
    glStencilFunc(GL_ALWAYS, 1, 255);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilMask(255);
    //  draw as usual
    this->fl.Draw();

    glStencilFunc(GL_NOTEQUAL, 1, 255);
    
    static ZC_ShProg* pShPStencilBorder = &(ZC_ShProgs::Get(ShPN_ZCR_StencilBorder)->shProg);

    pShPStencilBorder->ActivateOpenGL();    //  use stencil shader program
    this->fl.Draw(false, true, false);
}