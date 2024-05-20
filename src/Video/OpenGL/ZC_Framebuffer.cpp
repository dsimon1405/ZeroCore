#include <ZC/Video/OpenGL/ZC_Framebuffer.h>

#include <cassert>

ZC_Framebuffer ZC_Framebuffer::CreateFramebuffer()
{
    ZC_Framebuffer fb;
    glCreateFramebuffers(1, &(fb.id));
    return fb;
}

ZC_Framebuffer::ZC_Framebuffer(ZC_Framebuffer&& fb)
    : id(fb.id)
{
    fb.id = 0;
}

ZC_Framebuffer& ZC_Framebuffer::operator = (ZC_Framebuffer&& fb)
{
    if (id != 0 ) glDeleteFramebuffers(1, &id);
    id = fb.id;
    fb.id = 0;
    return *this;
}

ZC_Framebuffer::~ZC_Framebuffer()
{
    if (id != 0) glDeleteFramebuffers(1, &id);
}

void ZC_Framebuffer::Bind() const
{
    if (activeId == id) return;
    activeId = id;
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void ZC_Framebuffer::Unbind()
{
    if (activeId == 0) return;
    activeId = 0;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ZC_Framebuffer::GLBlitNamedFramebuffer(const ZC_Framebuffer& drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask)
{
    glBlitNamedFramebuffer(id, drawFramebuffer.id, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, mask & GL_COLOR_BUFFER_BIT ? GL_LINEAR : GL_NEAREST);
}

unsigned int ZC_Framebuffer::GetId() const noexcept
{
    return id;
}

void ZC_Framebuffer::CheckCreation()
{
    switch (glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER))
    {
    case GL_FRAMEBUFFER_COMPLETE: break;
    case GL_FRAMEBUFFER_UNDEFINED: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: assert(false); break;
    case GL_FRAMEBUFFER_UNSUPPORTED: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: assert(false); break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: assert(false); break;
    case 0: assert(false); break;   //  errors: GL_INVALID_ENUM or GL_INVALID_OPERATION
    default: assert(false);     //  undefined behaviour
    }
}

void ZC_Framebuffer::GLNamedFramebufferTexture(GLenum attachment, GLuint texture)
{
    glNamedFramebufferTexture(id, attachment, texture, 0);
}

void ZC_Framebuffer::GLNamedFramebufferRenderbuffer(GLenum attachment, GLuint renderbuffer)
{
    glNamedFramebufferRenderbuffer(id, attachment, GL_RENDERBUFFER, renderbuffer);
} 