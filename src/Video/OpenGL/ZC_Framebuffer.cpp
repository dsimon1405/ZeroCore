#include <ZC/Video/OpenGL/ZC_Framebuffer.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

ZC_Framebuffer::ZC_Framebuffer(bool create)
{
    if (create) glGenFramebuffers(1, &id);
}

ZC_Framebuffer::ZC_Framebuffer(ZC_Framebuffer&& fb)
    : id(fb.id)
{
    fb.id = 0;
}

ZC_Framebuffer& ZC_Framebuffer::operator = (ZC_Framebuffer&& fb)
{
    if (id != 0 ) glDeleteRenderbuffers(1, &id);
    id = fb.id;
    fb.id = 0;
    return *this;
}

ZC_Framebuffer::~ZC_Framebuffer()
{
    if (id != 0) glDeleteFramebuffers(1, &id);
}

void ZC_Framebuffer::Bind(unsigned int targer) const
{
    glBindFramebuffer(targer, id);
}

void ZC_Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ZC_Framebuffer::BlitTo(const ZC_Framebuffer& fbToDraw, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbToDraw.id);
    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, mask == GL_COLOR_BUFFER_BIT ? GL_LINEAR : GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int ZC_Framebuffer::GetId() const noexcept
{
    return id;
}