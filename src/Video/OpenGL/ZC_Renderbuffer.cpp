#include <ZC/Video/OpenGL/ZC_Renderbuffer.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

ZC_Renderbuffer::ZC_Renderbuffer(int samples, unsigned int internalFormat, int width, int height)
{
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

ZC_Renderbuffer::ZC_Renderbuffer(unsigned int internalFormat, int width, int height)
{
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
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

unsigned int ZC_Renderbuffer::GetId() const noexcept
{
    return id;
}