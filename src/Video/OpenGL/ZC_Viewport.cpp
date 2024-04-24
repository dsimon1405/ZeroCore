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
    if (startX == currentStartX && startY == currentStartY && width == currentWidth && height == currentHeight) return;
    
    glViewport(startX, startY, width, height);
    currentStartX = startX;
    currentStartY = startY;
    currentWidth = width;
    currentHeight = height;
}

void ZC_Viewport::SetSize(int _width, int _height)
{
    width = _width;
    height = _height;
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