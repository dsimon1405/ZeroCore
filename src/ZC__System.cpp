#include <ZC/ZC__System.h>

#include "ZC_System.h"
#include <ZC/Video/OpenGL/Renderer/ZC_Renders.h>

namespace ZC__System
{
    bool Init(ZC_SystemFlags system_flags, ZC_WindowFlags win_flags, int win_width, int win_height, const char* win_name)
    {
        return ZC_System::Init(system_flags, win_flags, win_width, win_height, win_name);
    }

    void RunMainCycle()
    {
        if (ZC_System::upSystem) ZC_System::upSystem->RunMainCycle();
    }

    void BreakMainCycle()
    {
        if (ZC_System::upSystem) ZC_System::upSystem->BreakMainCycle();
    }

    void GlClearColor(float red, float green, float blue, float alpha)
    {
        ZC_Renders::GetRender(ZC_RL_Default)->SetClearColor(red, green, blue, alpha);
    }

    void GlEnablePointSize()
    {
        ZC_Render::GLEnablePointSize();
    }
    
} // namespace ZC__System


namespace ZC__Window
{
    void GetSize(int& width, int& height)
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VGetSize(width, height);
    }

    void HideCursor()
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VHideCursor();
    }

    void ShowCursor()
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VShowCursor();
    }

    void LimitCursor()
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VLimitCursor();
    }

    void UnlimitCursor()
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VUnlimitCursor();
    }

    bool IsCursorLimited()
    {
        return ZC_System::upSystem ? ZC_System::upSystem->upIWindow->VIsCursorLimited() : false;
    }

    void StartInputText()
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VStartInputText();
    }

    void StopInputText()
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VStopInputText();
    }

    void GetCursorPosition(float& posX, float& posY)
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upEventsHolder->GetCursorPosition(posX, posY);
    }

    void SetMaxSize(int x, int y)
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VSetMaxSize(x, y);
    }

    void SetMinSize(int x, int y)
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VSetMinSize(x, y);
    }

    void SetFullScreen(bool full_screen)
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VSetFullScreen(full_screen);
    }
    
    bool IsFullScreen()
    {
        return ZC_System::upSystem ? ZC_System::upSystem->upIWindow->VIsFullScreen() : false;
    }

    void GetPosition(int& x, int& y)
    {
        if (ZC_System::upSystem) ZC_System::upSystem->upIWindow->VGetPosition(x, y);
    }
} // namespace ZC__Window


namespace ZC__FPS
{
    void SetLimit(long limit) noexcept
    {
        if (ZC_System::upSystem) ZC_System::upSystem->fps.SetLimit(limit);
    }

    float GetPreviousFrameTime() noexcept
    {
        return ZC_System::upSystem ? ZC_System::upSystem->fps.GetPreviousFrameTime(ZC_System::upSystem->fps.GetTimeMeasure()) : 0.f;
    }

    float GetPreviousFrameTime(ZC_FPS_TimeMeasure time_measure) noexcept
    {
        return ZC_System::upSystem ? ZC_System::upSystem->fps.GetPreviousFrameTime(time_measure) : 0.f;
    }

    ZC_FPS_TimeMeasure GetTimeMeasure() noexcept
    {
        return ZC_System::upSystem ? ZC_System::upSystem->fps.GetTimeMeasure() : ZC_FPS_TM__Nanoseconds;
    }

    void SetTimeMeasure(ZC_FPS_TimeMeasure timeMeasure)
    {
        if (ZC_System::upSystem) ZC_System::upSystem->fps.SetTimeMeasure(timeMeasure);
    }

    void NeedDraw(bool needDraw)
    {
        if (ZC_System::upSystem) ZC_System::upSystem->fps.NeedDraw(needDraw);
    }

    bool IsDrawing()
    {
        return ZC_System::upSystem && ZC_System::upSystem->fps.IsDrawing();
    }

    unsigned long long GetCurrentFrameNumber()
    {
        return ZC_System::upSystem ? ZC_System::upSystem->fps.GetCurrentFrameNumber() : 0;
    }
} // namespace ZC__FPS


namespace ZC__Updater
{
    void ChangeState(bool needUpdate)
    {
        if (ZC_System::upSystem && ZC_System::upSystem->upUpdater) ZC_System::upSystem->upUpdater->ChangeState(needUpdate);
    }

    void ChangeLevelState(size_t lvl, bool is_active)
    {
        if (ZC_System::upSystem && ZC_System::upSystem->upUpdater) ZC_System::upSystem->upUpdater->ChangeLevelState(lvl, is_active);
    }

    ZC_EC Connect(ZC_Function<void(float)>&& func, size_t level)
    {
        return ZC_System::upSystem && ZC_System::upSystem->upUpdater ? ZC_System::upSystem->upUpdater->Connect(std::move(func), level) : ZC_EC();
    }
} // namespace ZC__Updater


namespace ZC__Collision
{
    void ChangeState(bool nake_collision)
    {
        if (ZC_System::upSystem && ZC_System::upSystem->upCollision_manager) ZC_System::upSystem->upCollision_manager->ChangeState(nake_collision);
    }
    
    bool GetState()
    {
        return ZC_System::upSystem && ZC_System::upSystem->upCollision_manager ? ZC_System::upSystem->upCollision_manager->GetState() : false;
    }
} // ZC__Collision