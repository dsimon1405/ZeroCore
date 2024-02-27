#include <ZC/Video/ZC_Window.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <Tools/ZC_uptrCreateWithErrorCheck.h>

#if defined(ZC_SDL_VIDEO)
#include "PC/SDL/ZC_SDL_Window.h"
ZC_upWindow ZC_Window::MakeWindow(bool border, int width, int height, const char* name)
{
    return ZC_uptrCreateWithErrorCheck<ZC_Window, ZC_SDL_Window>(border, width, height, name);
}
#elif defined(ZC_ANDROID_NATIVE_APP_GLUE)
#include "Android/AndroidNativeAppGlue/ZC_AndroidNativeAppGlue_Window.h"
ZC_upWindow ZC_Window::MakeWindow(bool border, int width, int height, const char* name)
{
    return ZC_uptrCreateWithErrorCheck<ZC_Window, ZC_AndroidNativeAppGlue_Window>();
}
#endif

ZC_Window::ZC_Window()
{
    pWindow = this;
}

ZC_Window::~ZC_Window()
{
    pWindow = nullptr;
}

void ZC_Window::SetFPS(long limit) noexcept
{
    if (pWindow) pWindow->VSetFPS(limit);
}

float ZC_Window::GetPreviousFrameTime()
{
    return pWindow ? pWindow->VGetPreviousFrameTime() : 0.f;
}

void ZC_Window::GetSize(int& width, int& height)
{
    if (pWindow) pWindow->VGetSize(width, height);
}

void ZC_Window::HideCursor()
{
    if (pWindow) pWindow->VHideCursor();
}

void ZC_Window::ShowCursor()
{
    if (pWindow) pWindow->VShowCursor();
}

void ZC_Window::LimitCursor()
{
    if (pWindow) pWindow->VLimitCursor();
}

void ZC_Window::UnlimitCursor()
{
    if (pWindow) pWindow->VUnlimitCursor();
}

ZC_SConnection ZC_Window::ConnectResize(ZC_Function<void(float,float)>&& func)
{
    return pWindow ? pWindow->sigResize.Connect(std::move(func)) : ZC_SConnection();
}




void ZC_Window::SetClearColor(float r, float g, float b)
{
    glClearColor(r, g, b, 1);
}    

void ZC_Window::Clear(GLbitfield mask)
{
    glClear(mask);
}

ZC_Window* ZC_Window::GetCurrentWindow()
{
    return pWindow;
}