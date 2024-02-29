#include <ZC/Video/ZC_Window.h>

#include "ZC_WindowHolder.h"
#include <Video/OpenGL/ZC_OpenGL.h>

bool ZC_Window::MakeWindow(ZC_WindowFlags flags, int width, int height, const char* name)
{
    return ZC_WindowHolder::MakeWindowHolder(flags, width, height, name);
}

void ZC_Window::GlClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void ZC_Window::GlEnablePointSize()
{
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void ZC_Window::SetFPS(long limit) noexcept
{
    if (ZC_WindowHolder::pWindowHolder) ZC_WindowHolder::pWindowHolder->SetFPS(limit);
}

float ZC_Window::GetPreviousFrameTime() noexcept
{
    return ZC_WindowHolder::pWindowHolder ? ZC_WindowHolder::pWindowHolder->GetPreviousFrameTime() : 0.f;
}

void ZC_Window::GetSize(int& width, int& height)
{
    if (ZC_WindowHolder::pWindowHolder) ZC_WindowHolder::pWindowHolder->VGetSize(width, height);
}

void ZC_Window::HideCursor()
{
    if (ZC_WindowHolder::pWindowHolder) ZC_WindowHolder::pWindowHolder->VHideCursor();
}

void ZC_Window::ShowCursor()
{
    if (ZC_WindowHolder::pWindowHolder) ZC_WindowHolder::pWindowHolder->VShowCursor();
}

void ZC_Window::LimitCursor()
{
    if (ZC_WindowHolder::pWindowHolder) ZC_WindowHolder::pWindowHolder->VLimitCursor();
}

void ZC_Window::UnlimitCursor()
{
    if (ZC_WindowHolder::pWindowHolder) ZC_WindowHolder::pWindowHolder->VUnlimitCursor();
}

void ZC_Window::RuntMainCycle()
{
    if (ZC_WindowHolder::pWindowHolder) ZC_WindowHolder::pWindowHolder->RunMainCycle();
}