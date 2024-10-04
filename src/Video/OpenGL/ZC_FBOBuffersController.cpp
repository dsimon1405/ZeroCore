#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>

#include <cassert>

ZC_FBOBuffersController::ZC_FBOBuffersController(bool useColorBuffer, bool useDepthBuffer, bool useStencilBuffer)
    : haveColorBuffer(useColorBuffer),
    haveDepthBuffer(useDepthBuffer),
    haveStencilBuffer(useStencilBuffer),
    needClearColor(useColorBuffer),
    needClearDepth(haveDepthBuffer),
    needClearStencil(haveStencilBuffer)
{}

void ZC_FBOBuffersController::MakeActive()
{
    pActiveBufferController = this;
}

void ZC_FBOBuffersController::GlEnable(GLenum cap)
{
    assert(!(cap & GL_DEPTH_BUFFER_BIT) || !(cap & GL_STENCIL_BUFFER_BIT));
    switch (cap)
    {
    case GL_DEPTH_TEST:
        assert(pActiveBufferController->haveDepthBuffer);
        if (pActiveBufferController->isDepthEnable) return;
        glEnable(GL_DEPTH_TEST);
        pActiveBufferController->isDepthEnable = true;
        break;
    case GL_STENCIL_TEST:
        assert(pActiveBufferController->haveStencilBuffer);
        if (pActiveBufferController->isStecncilEnable) return;
        glEnable(GL_STENCIL_TEST);
        pActiveBufferController->isStecncilEnable = true;
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
    {
        // assert(pActiveBufferController->haveDepthBuffer);
        if (!pActiveBufferController->isDepthEnable) return;
        glDisable(GL_DEPTH_TEST);
        pActiveBufferController->isDepthEnable = false;
    } break;
    case GL_STENCIL_TEST:
    {
        // assert(pActiveBufferController->haveStencilBuffer);
        if (!pActiveBufferController->isStecncilEnable) return;
        glDisable(GL_STENCIL_TEST);
        pActiveBufferController->isStecncilEnable = false;
    } break;
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
        needClearColor = false;
    }
    if (haveDepthBuffer)
    {
        clearMask |= GL_DEPTH_BUFFER_BIT;
        needClearDepth = false;
        isDepthEnable = false;
    }
    if (haveStencilBuffer)
    {
        clearMask |= GL_STENCIL_BUFFER_BIT;
        needClearStencil = false;
        isStecncilEnable = false;
    }
    glClear(clearMask);
}

void ZC_FBOBuffersController::GlClear(GLbitfield clearMask)
{
    if (clearMask == 0) return;
    assert((clearMask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))) == 0);

    if (((clearMask & GL_COLOR_BUFFER_BIT) && pActiveBufferController->needClearColor)
        || ((clearMask & GL_DEPTH_BUFFER_BIT) && pActiveBufferController->needClearDepth)
        || ((clearMask & GL_STENCIL_BUFFER_BIT) && pActiveBufferController->needClearStencil))
    {
        if (clearMask & GL_COLOR_BUFFER_BIT)
        {
            pActiveBufferController->CheckClearColor();
            pActiveBufferController->needClearColor = false;
        }
        glClear(clearMask);

        if (clearMask & GL_DEPTH_BUFFER_BIT) pActiveBufferController->needClearDepth = false;
        if (clearMask & GL_STENCIL_BUFFER_BIT) pActiveBufferController->needClearStencil = false;
    }
}

void ZC_FBOBuffersController::SetClearColor(const ZC_Vec4<float>& _clearColor)
{
    clearColor = _clearColor;
}

void ZC_FBOBuffersController::GLDepthMask(GLboolean flag)
{
    assert(!(flag == GL_TRUE) || !(flag == GL_FALSE));
    assert(pActiveBufferController->haveDepthBuffer);
    switch (flag)
    {
    case GL_TRUE:
        if (pActiveBufferController->isDepthMask_TRUE) return;
        glDepthMask(GL_TRUE);
        pActiveBufferController->isDepthMask_TRUE = true;
        break;
    case GL_FALSE:
        if (!pActiveBufferController->isDepthMask_TRUE) return;
        glDepthMask(GL_FALSE);
        pActiveBufferController->isDepthMask_TRUE = false;
        break;
    default: assert(false); break;
    }
}

void ZC_FBOBuffersController::CheckClearColor()
{
    static ZC_Vec4<float> currentColor;
    if (currentColor == clearColor) return;
    currentColor = clearColor;
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
}

void ZC_FBOBuffersController::UsingGLDraw()
{
    if (pActiveBufferController->haveColorBuffer) pActiveBufferController->needClearColor = true;
    if (pActiveBufferController->isDepthEnable) pActiveBufferController->needClearDepth = true;
    if (pActiveBufferController->isStecncilEnable) pActiveBufferController->needClearStencil = true;
}
