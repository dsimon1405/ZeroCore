#include "ZC_SWindowHolder.h"

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShaderManager.h>
#include <ZC/Video/ZC_SWindow.h>

#ifdef ZC_SDL_VIDEO
#include <Video/PC/SDL/ZC_SDL_Window.h>
bool ZC_SWindowHolder::MakeWindowHolder(int flags, int width, int height, const char* name)
{
    upWindowHolder = ZC_uptrMakeFromChild<ZC_SWindowHolder, ZC_SDL_Window>(flags, width, height, name);
    if (!upWindowHolder) return false;
    upWindowHolder->LoadShProgs();
    upWindowHolder->AddZC_Render();
    if (flags & ZC_SWF__GUI) upWindowHolder->upGUI = new ZC_GUI();
    return true;
}
// #elif defined(ZC_ANDROID_NATIVE_APP_GLUE)
// #include "Android/AndroidNativeAppGlue/ZC_AndroidNativeAppGlue_Window.h"
// ZC_upWindow ZC_SWindow::MakeWindow(bool border, int width, int height, const char* name)
// {
//     return ZC_uptrCreateWithErrorCheck<ZC_SWindow, ZC_AndroidNativeAppGlue_Window>();
// }
#endif

ZC_SWindowHolder::~ZC_SWindowHolder()
{
    fps.NeedDraw(false);
    ZC_Renders::EraseRender(ZC_RL_Default);
    ZC_ShaderManager::Clear();
}

void ZC_SWindowHolder::CloseWindow()
{
    isDrawing = false;
}

void ZC_SWindowHolder::RunMainCycle()
{
    if (upGUI) upGUI->Configure();
    fps.StartNewFrame();    //  make prepearing call, to avoid false large information about frist frmae time
    while (true)
    {
        float time = fps.StartNewFrame();   //  time in nanoseconds (default) or in user's seted measure 
        upEventsHolder->PollEvents(time);
        if (!isDrawing) return;     //  check after polling events
        updater.Call(time);
        collision_manager.MakeCollision();
        renderer.Draw(upGUI ? upGUI.Get() : nullptr);
    }
}

void ZC_SWindowHolder::SetFPS(long limit)
{
	fps.SetLimit(limit);
}

float ZC_SWindowHolder::GetPreviousFrameTime() const noexcept
{
	return fps.GetPreviousFrameTime(fps.GetTimeMeasure());
}

float ZC_SWindowHolder::GetPreviousFrameTime(ZC_FPS_TimeMeasure time_measure) const noexcept
{
	return fps.GetPreviousFrameTime(time_measure);
}

ZC_FPS_TimeMeasure ZC_SWindowHolder::GetFPSTimeMeasure() const noexcept
{
    return fps.GetTimeMeasure();
}

void ZC_SWindowHolder::NeedDrawFPS(bool needDraw)
{
    fps.NeedDraw(needDraw);
}

bool ZC_SWindowHolder::IsFPSDrawing()
{
    return fps.IsDrawing();
}

void ZC_SWindowHolder::GetCursorPosition(float& posX, float& posY)
{
    upEventsHolder->GetCursorPosition(posX, posY);
}

void ZC_SWindowHolder::SetFPSTimeMeasure(ZC_FPS_TimeMeasure timeMeasure)
{
    fps.ChangeTimeMeasure(timeMeasure);
}

ZC_EC ZC_SWindowHolder::ConnectToUpdater(ZC_Function<void(float)>&& func, size_t level)
{
    return updater.Connect(std::move(func), level);
}

unsigned long long ZC_SWindowHolder::GetCurrentFrameNumber() const
{
    return fps.GetCurrentFrameNumber();
}

void ZC_SWindowHolder::ChangeUpdaterState(bool needUpdate)
{
    updater.ChangeUpdaterState(needUpdate);
}

void ZC_SWindowHolder::ChangeUpdaterLevelState(size_t lvl, bool is_active)
{
    updater.ChangeLevelState(lvl, is_active);
}

ZC_SWindowHolder::ZC_SWindowHolder()
    : upEventsHolder(ZC_EventsHolder::MakeEventsHolder()),
	fps(ZC_FPS_TimeMeasure::ZC_FPS_TM__Nanoseconds),
    renderer({ &ZC_SWindowHolder::VSwapBuffer, this })
{}

void ZC_SWindowHolder::LoadShProgs()
{
    ZC_ShProgs shProgs;
    ZC_ShPName loadAll { ShPN_LoadAll };
    shProgs.Load(&loadAll, 0);
}

void ZC_SWindowHolder::AddZC_Render()
{
    ZC_Renders::CreateRender(ZC_RL_Default, ZC_Render::DS_Loop, ZC_FBO());
}