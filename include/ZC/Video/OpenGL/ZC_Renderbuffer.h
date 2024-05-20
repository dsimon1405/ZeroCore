#pragma once

#include <glad/glad.h>

struct ZC_Renderbuffer
{
    static ZC_Renderbuffer GLNamedRenderbufferStorage(GLenum internalformat, GLsizei width, GLsizei height);
    static ZC_Renderbuffer GLNamedRenderbufferStorageMultisample(GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
    
    ZC_Renderbuffer() = default;

    ZC_Renderbuffer(ZC_Renderbuffer&& rb);
    ZC_Renderbuffer& operator = (ZC_Renderbuffer&& rb);

    ~ZC_Renderbuffer();

    GLuint GetId() const noexcept;

private:
    GLuint id = 0;
};