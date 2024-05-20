#include <ZC/Video/ZC_SWindow.h>

#include "ZC_SWindowHolder.h"
#include <ZC/Video/OpenGL/Renderer/ZC_Renders.h>

bool ZC_SWindow::MakeWindow(ZC_WindowFlags flags, int width, int height, const char* name)
{
    return ZC_SWindowHolder::MakeWindowHolder(flags, width, height, name);
}

void ZC_SWindow::GlClearColor(float red, float green, float blue, float alpha)
{
    ZC_Renders::GetRender(ZC_RL_Default)->SetClearColor(red, green, blue, alpha);
}

void ZC_SWindow::GlEnablePointSize()
{
    ZC_Render::GLEnablePointSize();
}

void ZC_SWindow::SetFPS(long limit) noexcept
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->SetFPS(limit);
}

float ZC_SWindow::GetPreviousFrameTime() noexcept
{
    return ZC_SWindowHolder::upWindowHolder ? ZC_SWindowHolder::upWindowHolder->GetPreviousFrameTime() : 0.f;
}

void ZC_SWindow::GetSize(int& width, int& height)
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VGetSize(width, height);
}

void ZC_SWindow::HideCursor()
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VHideCursor();
}

void ZC_SWindow::ShowCursor()
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VShowCursor();
}

void ZC_SWindow::LimitCursor()
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VLimitCursor();
}

void ZC_SWindow::UnlimitCursor()
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VUnlimitCursor();
}

void ZC_SWindow::RuntMainCycle()
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->RunMainCycle();
}

void ZC_SWindow::NeedDrawFPS(bool needDraw)
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->NeedDrawFPS(needDraw);
}

bool ZC_SWindow::IsFPSDrawing()
{
    return ZC_SWindowHolder::upWindowHolder && ZC_SWindowHolder::upWindowHolder->IsFPSDrawing();
}

void ZC_SWindow::GetCursorPosition(float& posX, float& posY)
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->GetCursorPosition(posX, posY);
}