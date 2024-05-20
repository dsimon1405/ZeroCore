#include <ZC/Video/OpenGL/ZC_Renderbuffer.h>

ZC_Renderbuffer ZC_Renderbuffer::GLNamedRenderbufferStorage(GLenum internalformat, GLsizei width, GLsizei height)
{
    ZC_Renderbuffer rb;
    glCreateRenderbuffers(1, &(rb.id));
    glNamedRenderbufferStorage(rb.id, internalformat, width, height);
    return rb;
}

ZC_Renderbuffer ZC_Renderbuffer::GLNamedRenderbufferStorageMultisample(GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
    ZC_Renderbuffer rb;
    glCreateRenderbuffers(1, &(rb.id));
    glNamedRenderbufferStorageMultisample(rb.id, samples, internalformat, width, height);
    return rb;
}

ZC_Renderbuffer::ZC_Renderbuffer(ZC_Renderbuffer&& rb)
    : id(rb.id)
{
    rb.id = 0;
}

ZC_Renderbuffer& ZC_Renderbuffer::operator = (ZC_Renderbuffer&& rb)
{
    if (id != 0 ) glDeleteRenderbuffers(1, &id);
    id = rb.id;
    rb.id = 0;
    return *this;
}

ZC_Renderbuffer::~ZC_Renderbuffer()
{
    if (id != 0) glDeleteRenderbuffers(1, &id);
}

GLuint ZC_Renderbuffer::GetId() const noexcept
{
    return id;
}