#include <ZC/Video/OpenGL/ZC_GLBlend.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

#include <cassert>

ZC_GLBlend::ZC_GLBlend(unsigned int _sfactor, unsigned int _dfactor, float red, float green, float blue, float alpha)
    : sfactor(_sfactor),
    dfactor(_dfactor),
    blendColor(red, green, blue, alpha)
{
    assert((sfactor == GL_ZERO || sfactor == GL_ONE || sfactor == GL_SRC_COLOR || sfactor == GL_ONE_MINUS_SRC_COLOR || sfactor == GL_DST_COLOR ||
        sfactor == GL_ONE_MINUS_DST_COLOR || sfactor == GL_SRC_ALPHA || sfactor == GL_ONE_MINUS_SRC_ALPHA || sfactor == GL_DST_ALPHA ||
        sfactor ==  GL_ONE_MINUS_DST_ALPHA || sfactor == GL_CONSTANT_COLOR || sfactor == GL_ONE_MINUS_CONSTANT_COLOR || sfactor == GL_CONSTANT_ALPHA ||
        sfactor == GL_ONE_MINUS_CONSTANT_ALPHA || sfactor == GL_SRC_ALPHA_SATURATE || sfactor == GL_SRC1_COLOR || sfactor == GL_ONE_MINUS_SRC1_COLOR ||
        sfactor == GL_SRC1_ALPHA || sfactor == GL_ONE_MINUS_SRC1_ALPHA)
        && (dfactor == GL_ZERO || dfactor == GL_ONE || dfactor == GL_SRC_COLOR || dfactor == GL_ONE_MINUS_SRC_COLOR || dfactor == GL_DST_COLOR ||
        dfactor == GL_ONE_MINUS_DST_COLOR || dfactor == GL_SRC_ALPHA || dfactor == GL_ONE_MINUS_SRC_ALPHA || dfactor == GL_DST_ALPHA ||
        dfactor ==  GL_ONE_MINUS_DST_ALPHA || dfactor == GL_CONSTANT_COLOR || dfactor == GL_ONE_MINUS_CONSTANT_COLOR || dfactor == GL_CONSTANT_ALPHA ||
        dfactor == GL_ONE_MINUS_CONSTANT_ALPHA || dfactor == GL_SRC_ALPHA_SATURATE || dfactor == GL_SRC1_COLOR || dfactor == GL_ONE_MINUS_SRC1_COLOR ||
        dfactor == GL_SRC1_ALPHA || dfactor == GL_ONE_MINUS_SRC1_ALPHA)
        && (red == -1.f || (red >= 0.f && red <= 1.f)) && (green == -1.f || (green >= 0.f && green <= 1.f))
        && (blue == -1.f || (blue >= 0.f && blue <= 1.f)) && (alpha == -1.f || (alpha >= 0.f && alpha <= 1.f)));
}

ZC_GLBlend::~ZC_GLBlend()
{
    if (pActiveGLBlend == this) pActiveGLBlend = nullptr;
}

void ZC_GLBlend::Use()
{
    if (sfactor != defaultValue)
    {
        if (!pActiveGLBlend)    //  blend disabled, enable and reset needed params
        {
            glEnable(GL_BLEND);
            glBlendFunc(sfactor, dfactor);
            if (blendColor[0] != -1.f)   //  id not default (-1.f), then need set
                glBlendColor(blendColor[0], blendColor[1], blendColor[2], blendColor[3]);
        }
        else
        {
            if (sfactor != pActiveGLBlend->sfactor || dfactor != pActiveGLBlend->dfactor)   //  check blending function, if need, reset
                glBlendFunc(sfactor, dfactor);
            if (blendColor[0] != -1.f && blendColor != pActiveGLBlend->blendColor)   //  if not default (-1.f) and not same as active, then need reset
                glBlendColor(blendColor[0], blendColor[1], blendColor[2], blendColor[3]);
        }
        pActiveGLBlend = this;
    }
    else
    {
        if (pActiveGLBlend)
         glDisable(GL_BLEND);
        pActiveGLBlend = nullptr;
    }
}

void ZC_GLBlend::Disable()
{
    pActiveGLBlend = nullptr;
}