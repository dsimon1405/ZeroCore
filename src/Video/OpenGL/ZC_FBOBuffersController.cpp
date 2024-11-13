#include <ZC/Video/OpenGL/ZC_FBOBuffersController.h>

#include <ZC/Video/OpenGL/ZC_GLDepth.h>
#include <ZC/Video/OpenGL/ZC_GLStencil.h>

#include <cassert>

ZC_FBOBuffersController::ZC_FBOBuffersController(bool useColorBuffer, bool useDepthBuffer, bool useStencilBuffer)
    : haveColorBuffer(useColorBuffer),
    haveDepthBuffer(useDepthBuffer),
    haveStencilBuffer(useStencilBuffer),
    needClearColor(useColorBuffer),
    needClearDepth(haveDepthBuffer),
    needClearStencil(haveStencilBuffer)
{
    all_buffers_controllers.emplace_back(this);
}

ZC_FBOBuffersController::ZC_FBOBuffersController(const ZC_FBOBuffersController& bc)
    : haveColorBuffer(bc.haveColorBuffer),
    haveDepthBuffer(bc.haveDepthBuffer),
    haveStencilBuffer(bc.haveStencilBuffer),
    needClearColor(bc.needClearColor),
    needClearDepth(bc.haveDepthBuffer),
    needClearStencil(bc.haveStencilBuffer),
    clearColor(bc.clearColor)
{
    all_buffers_controllers.emplace_back(this);
}

ZC_FBOBuffersController::~ZC_FBOBuffersController()
{
    std::erase(all_buffers_controllers, this);
}

ZC_FBOBuffersController* ZC_FBOBuffersController::GetActiveBufferController()
{
    return pActiveBufferController;
}

void ZC_FBOBuffersController::MakeActive()
{
    pActiveBufferController = this;
}

void ZC_FBOBuffersController::GlClear()
{
    GLenum clear_mask = 0u;
    if (needClearColor)
    {
        clear_mask = GL_COLOR_BUFFER_BIT;
        needClearColor = false;
    }
    if (needClearDepth)
    {
        clear_mask |= GL_DEPTH_BUFFER_BIT;
        needClearDepth = false;
    }
    if (needClearStencil)
    {
        clear_mask |= GL_STENCIL_BUFFER_BIT;
        needClearStencil = false;
    }
    if (clear_mask == 0u) return;
    glClear(clear_mask);
}

void ZC_FBOBuffersController::GlClear(GLbitfield clear_mask)
{
    if (clear_mask == 0u) return;
    assert((clear_mask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))) == 0u);

    GLbitfield clear_mask_actual = 0u;
    if ((clear_mask & GL_COLOR_BUFFER_BIT) && pActiveBufferController->needClearColor)
    {
        clear_mask_actual = GL_COLOR_BUFFER_BIT;
        pActiveBufferController->needClearColor = false;
    }
    if ((clear_mask & GL_DEPTH_BUFFER_BIT) && pActiveBufferController->needClearDepth)
    {
        clear_mask_actual |= GL_DEPTH_BUFFER_BIT;
        pActiveBufferController->needClearDepth = false;
    }
    if ((clear_mask & GL_STENCIL_BUFFER_BIT) && pActiveBufferController->needClearStencil)
    {
        clear_mask_actual |= GL_STENCIL_BUFFER_BIT;
        pActiveBufferController->needClearStencil = false;
    }

    if (clear_mask_actual == 0u) return;
    glClear(clear_mask_actual);
}

void ZC_FBOBuffersController::SetClearColor(const ZC_Vec4<float>& _clearColor)
{
    clearColor = _clearColor;
}

void ZC_FBOBuffersController::BufferSwaped()
{
    for (ZC_FBOBuffersController* bc : all_buffers_controllers)
    {
        if (bc->haveColorBuffer) bc->needClearColor = true;
        if (bc->haveDepthBuffer) bc->needClearDepth = true;
        if (bc->haveStencilBuffer) bc->needClearStencil = true;
    }
}

void ZC_FBOBuffersController::UpdateClearColor()
{
    static ZC_Vec4<float> currentColor;
    if (currentColor == clearColor) return;
    currentColor = clearColor;
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
}

void ZC_FBOBuffersController::UsingGLDraw()
{
    if (pActiveBufferController->haveColorBuffer) pActiveBufferController->needClearColor = true;
    if (ZC_GLDepth::IsGLDrawWtireToDepthBuffer()) pActiveBufferController->needClearDepth = true;
    if (ZC_GLStencil::IsGLDrawWtireToDepthBuffer()) pActiveBufferController->needClearStencil = true;
}
