#pragma once

#include <glad/glad.h>

struct ZC_Framebuffer
{
    static ZC_Framebuffer CreateFramebuffer();
    
    ZC_Framebuffer() = default;

    ZC_Framebuffer(ZC_Framebuffer&& fb);
    ZC_Framebuffer& operator = (ZC_Framebuffer&& fb);

    ~ZC_Framebuffer();

    void Bind() const;
    static void Unbind();

    void GLBlitNamedFramebuffer(const ZC_Framebuffer& fbToDraw, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
        GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask);
    GLuint GetId() const noexcept;
    void CheckCreation();

    void GLNamedFramebufferTexture(GLenum attachment, GLuint texture);
    void GLNamedFramebufferRenderbuffer(GLenum attachment, GLuint renderbuffer);

private:
    GLuint id = 0;

    static inline GLuint activeId;
};