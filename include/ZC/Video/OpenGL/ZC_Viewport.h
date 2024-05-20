#pragma once

#include <glad/glad.h>

class ZC_Viewport
{
public:
    //  Create a viewport with window size.
    static ZC_Viewport CreateStandardWindowViewport();
    
    ZC_Viewport(GLint _startX, GLint _startY, GLsizei _width, GLsizei _height);

    void Use();
    void SetSize(GLsizei _width, GLsizei _height);
    void GetSize(GLsizei& rWidth, GLsizei& rHeight) const noexcept;
    GLsizei GetWidth() const noexcept;
    GLsizei GetHeight() const noexcept;

private:
    static inline ZC_Viewport* pActiveViewport = nullptr;

    GLint startX,
        startY;
    GLsizei width,
        height;
};