#include <ZC/Video/OpenGL/Renderer/ZC_RBufferCleaner.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

#include <cassert>

void ZC_RBufferCleaner::GlEnable(GLenum cap)
{
    switch (cap)
    {
    case GL_DEPTH_TEST:
        glEnable(GL_DEPTH_TEST);
        isDepthEnable = true;
        needClearDepth = true;
        break;
    case GL_STENCIL_TEST:
        glEnable(GL_STENCIL_TEST);
        isStecncilEnable = true;
        needClearStencil = true;
        break;
    default: assert(false); break;
    }
}

void ZC_RBufferCleaner::GlDisable(GLenum cap)
{
    switch (cap)
    {
    case GL_DEPTH_TEST:
        glDisable(GL_DEPTH_TEST);
        isDepthEnable = false;
        break;
    case GL_STENCIL_TEST:
        glDisable(GL_STENCIL_TEST);
        isStecncilEnable = false;
        break;
    default: assert(false); break;
    }
}

void ZC_RBufferCleaner::GlClear()
{
    GLenum clearMask = GL_COLOR_BUFFER_BIT;
    if (needClearDepth) clearMask |= GL_DEPTH_BUFFER_BIT;
    if (needClearStencil) clearMask |= GL_STENCIL_BUFFER_BIT;
    glClear(clearMask);
    needClearDepth = isDepthEnable;
    needClearStencil = isStecncilEnable;
}

void ZC_RBufferCleaner::GlClear(GLenum gl_clear_buffer_bit)
{
    assert((gl_clear_buffer_bit & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))) == 0);
    glClear(GL_COLOR_BUFFER_BIT);
}