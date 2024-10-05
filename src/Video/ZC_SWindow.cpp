#include <ZC/Video/ZC_SWindow.h>

#include "ZC_SWindowHolder.h"
#include <ZC/Video/OpenGL/Renderer/ZC_Renders.h>

bool ZC_SWindow::MakeWindow(ZC_WindowFlags flags, int width, int height, const char* name)
{
    return ZC_SWindowHolder::MakeWindowHolder(flags, width, height, name);
}

void ZC_SWindow::CloseWindow()
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->CloseWindow();
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

float ZC_SWindow::GetPreviousFrameTime(ZC_FPS_TimeMeasure time_measure) noexcept
{
    return ZC_SWindowHolder::upWindowHolder ? ZC_SWindowHolder::upWindowHolder->GetPreviousFrameTime(time_measure) : 0.f;
}

ZC_FPS_TimeMeasure ZC_SWindow::GetFPSTimeMeasure() noexcept
{
    return ZC_SWindowHolder::upWindowHolder ? ZC_SWindowHolder::upWindowHolder->GetFPSTimeMeasure() : ZC_FPS_TM__Nanoseconds;
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

void ZC_SWindow::StartInputText()
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VStartInputText();
}

void ZC_SWindow::StopInputText()
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VStopInputText();
}

void ZC_SWindow::RunMainCycle()
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

void ZC_SWindow::SetFPSTimeMeasure(ZC_FPS_TimeMeasure timeMeasure)
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->SetFPSTimeMeasure(timeMeasure);
}

ZC_EC ZC_SWindow::ConnectToUpdater(ZC_Function<void(float)>&& func, size_t level)
{
    return ZC_SWindowHolder::upWindowHolder ? ZC_SWindowHolder::upWindowHolder->ConnectToUpdater(std::move(func), level) : ZC_EC();
}

unsigned long long ZC_SWindow::GetCurrentFrameNumber()
{
    return ZC_SWindowHolder::upWindowHolder ? ZC_SWindowHolder::upWindowHolder->GetCurrentFrameNumber() : 0;
}

void ZC_SWindow::SetMaxSize(int x, int y)
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VSetMaxSize(x, y);
}

void ZC_SWindow::SetMinSize(int x, int y)
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VSetMinSize(x, y);
}

void ZC_SWindow::ChangeUpdaterState(bool needUpdate)
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->ChangeUpdaterState(needUpdate);
}

void ZC_SWindow::ChangeUpdaterLevelState(size_t lvl, bool is_active)
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->ChangeUpdaterLevelState(lvl, is_active);
}

void ZC_SWindow::SetFullScreen(bool full_screen)
{
    if (ZC_SWindowHolder::upWindowHolder) ZC_SWindowHolder::upWindowHolder->VSetFullScreen(full_screen);
}