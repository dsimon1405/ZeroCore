#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

#include <cassert>

ZC_FBOBuffersController::ZC_FBOBuffersController(bool useColorBuffer, bool useDepthBuffer, bool useStencilBuffer)
    : haveColorBuffer(useColorBuffer),
    haveDepthBuffer(useDepthBuffer),
    haveStencilBuffer(useStencilBuffer),
    needClearDepth(haveDepthBuffer),
    needClearStencil(haveStencilBuffer)
{}

void ZC_FBOBuffersController::GlEnable(GLenum cap)
{
    assert(!(cap & GL_DEPTH_BUFFER_BIT) || !(cap & GL_STENCIL_BUFFER_BIT));
    switch (cap)
    {
    case GL_DEPTH_TEST:
        assert(haveDepthBuffer);
        if (isDepthEnable) return;
        glEnable(GL_DEPTH_TEST);
        isDepthEnable = true;
        needClearDepth = true;
        break;
    case GL_STENCIL_TEST:
        assert(haveStencilBuffer);
        if (isStecncilEnable) return;
        glEnable(GL_STENCIL_TEST);
        isStecncilEnable = true;
        needClearStencil = true;
        break;
    default: assert(false); break;
    }
}

void ZC_FBOBuffersController::GlDisable(GLenum cap)
{
    assert(!(cap & GL_DEPTH_BUFFER_BIT) || !(cap & GL_STENCIL_BUFFER_BIT));
    switch (cap)
    {
    case GL_DEPTH_TEST:
        assert(haveDepthBuffer);
        glDisable(GL_DEPTH_TEST);
        isDepthEnable = false;
        break;
    case GL_STENCIL_TEST:
        assert(haveStencilBuffer);
        glDisable(GL_STENCIL_TEST);
        isStecncilEnable = false;
        break;
    default: assert(false); break;
    }
}

void ZC_FBOBuffersController::GlClear()
{
    GLenum clearMask = 0;
    if (haveColorBuffer)
    {
        clearMask = GL_COLOR_BUFFER_BIT;
        CheckClearColor();
    }
    if (needClearDepth) clearMask |= GL_DEPTH_BUFFER_BIT;
    if (needClearStencil) clearMask |= GL_STENCIL_BUFFER_BIT;
    glClear(clearMask);
    needClearDepth = isDepthEnable;
    needClearStencil = isStecncilEnable;
}

void ZC_FBOBuffersController::GlClear(GLenum gl_clear_buffer_bit)
{
    assert((gl_clear_buffer_bit & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))) == 0);
    if (((gl_clear_buffer_bit & GL_DEPTH_BUFFER_BIT) && needClearDepth) || ((gl_clear_buffer_bit & GL_STENCIL_BUFFER_BIT) && needClearStencil)
        || (gl_clear_buffer_bit & GL_COLOR_BUFFER_BIT))
    {
        if (gl_clear_buffer_bit & GL_COLOR_BUFFER_BIT) CheckClearColor();
        glClear(gl_clear_buffer_bit);
        if ((gl_clear_buffer_bit & GL_DEPTH_BUFFER_BIT) && !isDepthEnable) needClearDepth = false;
        if ((gl_clear_buffer_bit & GL_STENCIL_BUFFER_BIT) && !isStecncilEnable) needClearStencil = false;
    }
}

void ZC_FBOBuffersController::SetClearColor(const ZC_Vec4<float>& _clearColor)
{
    clearColor = _clearColor;
}

void ZC_FBOBuffersController::CheckClearColor()
{
    static ZC_Vec4<float> currentColor;
    if (currentColor == clearColor) return;
    currentColor = clearColor;
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
}