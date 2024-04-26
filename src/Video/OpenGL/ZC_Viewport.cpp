#include <ZC/Video/OpenGL/ZC_Viewport.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include "ZC/Video/ZC_Window.h"

ZC_Viewport::ZC_Viewport(int _startX, int _startY, int _width, int _height)
    : startX(_startX),
    startY(_startY),
    width(_width),
    height(_height)
{}

ZC_Viewport ZC_Viewport::CreateStandardWindowViewport()
{
    int width,
        height;
    ZC_Window::GetSize(width, height);
    return { 0, 0, width, height };
}

void ZC_Viewport::Use()
{    
    if (pActiveViewport == this) return;
    pActiveViewport = this;
    
    glViewport(startX, startY, width, height);
}

void ZC_Viewport::SetSize(int _width, int _height)
{
    width = _width;
    height = _height;

    if (pActiveViewport != this) return;
    glViewport(startX, startY, width, height);
}

void ZC_Viewport::GetSize(int& rWidth, int& rHeight) const noexcept
{
    rWidth = width;
    rHeight = height;
}

int ZC_Viewport::GetWidth() const noexcept
{
    return width;
}

int ZC_Viewport::GetHeight() const noexcept
{
    return height;
}