#include "ZC_System.h"

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShaderManager.h>
#include <ZC/ZC__System.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_System::~ZC_System()
{
    fps.NeedDraw(false);
    ZC_Renders::EraseRender(ZC_RL_Default);
    ZC_ShaderManager::Clear();
}

bool ZC_System::Init(int system_flags, int win_flags, int win_width, int win_height, const char* win_name)
{
    ZC_uptr<ZC_IWindow> upWin = ZC_IWindow::MakeWindowHolder(win_flags, win_width, win_height, win_name);
    if (ZC_ErrorLogger::WasError()) return false;
    upSystem = new ZC_System(std::move(upWin), system_flags);    //  if window created successful, create system
    LoadShProgs();
    ZC_Renders::CreateRender(ZC_RL_Default, ZC_Render::DS_Loop, ZC_FBO());  //  must be after set upSystem
    if (system_flags & ZC_SF__GUI) upSystem->upGUI = new ZC_GUI();
    if (system_flags & ZC_SF__Updater) upSystem->upUpdater = new ZC_Updater();
    if (system_flags & ZC_SF__Collision) upSystem->upCollision_manager = new ZC_CollisionManager();
    return true;
}

// #define ZC_SystemRunMainCycle_timer
#ifdef ZC_SystemRunMainCycle_timer
#include <ZC/Tools/Time/ZC_Timer.h>
#include <iostream>
#include <ZC/Events/ZC_Events.h>
#endif 

void ZC_System::RunMainCycle()
{
    if (!upIWindow) return;
    if (upGUI) upGUI->Configure();
    fps.StartNewFrame();    //  make prepearing call, to avoid false large information about frist frmae time
#ifdef ZC_SystemRunMainCycle_timer

    double time_secs = 2.;
    ZC_Timer timer_poll_events(ZC_TR__seconds, time_secs, ZC_TRO__average, "poll events");
    ZC_Timer timer_updater(ZC_TR__seconds, time_secs, ZC_TRO__average, "updater");
    ZC_Timer timer_collision(ZC_TR__seconds, time_secs, ZC_TRO__average, "collision");
    ZC_Timer timer_draw(ZC_TR__seconds, time_secs, ZC_TRO__average, "draw");
    static bool timer_active = true;
    auto lamb_change_timer_activity = [](ZC_ButtonID, float) { timer_active = !timer_active; };
    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_P, { lamb_change_timer_activity }, nullptr);
    while (true)
    {
        timer_poll_events.ChangeActivity(timer_active);
        timer_updater.ChangeActivity(timer_active);
        timer_collision.ChangeActivity(timer_active);
        timer_draw.ChangeActivity(timer_active);

        float time = fps.StartNewFrame();   //  time in nanoseconds (default) or in user's seted measure
        timer_poll_events.StartPoint();
        upEventsHolder->PollEvents(time);
        if (!is_main_cycle_run)
        {
            upIWindow->VDestroy();
            return;     //  check after polling events
        }
        timer_poll_events.EndPoint();
        timer_updater.StartPoint();
        if (upUpdater) upUpdater->Call(time);
        timer_updater.EndPoint();
        timer_collision.StartPoint();
        if (upCollision_manager) upCollision_manager->MakeCollision();
        timer_collision.EndPoint();
        timer_draw.StartPoint();
        renderer.Draw(upGUI ? upGUI.Get() : nullptr);
        timer_draw.EndPoint();

        if (timer_active && timer_poll_events.GetValuesCount() == 0ul) std::cout<<std::endl;
    }
#else
    while (true)
    {
        float time = fps.StartNewFrame();   //  time in nanoseconds (default) or in user's seted measure
        upEventsHolder->PollEvents(time);
        if (!is_main_cycle_run)
        {
            upIWindow->VDestroy();
            return;     //  check after polling events
        }
        if (upUpdater) upUpdater->Call(time);
        if (upCollision_manager) upCollision_manager->MakeCollision();
        renderer.Draw(upGUI ? upGUI.Get() : nullptr);
    }
#endif
}

void ZC_System::BreakMainCycle()
{
    is_main_cycle_run = false;
}

ZC_System::ZC_System(ZC_uptr<ZC_IWindow>&& _upIWindow, int system_flags)
    : upIWindow(std::move(_upIWindow)),
    upEventsHolder(ZC_EventsHolder::MakeEventsHolder()),
	fps(ZC_FPS_TimeMeasure::ZC_FPS_TM__Nanoseconds),
    renderer({ &ZC_IWindow::VSwapBuffer, upIWindow.Get() })
{}

void ZC_System::LoadShProgs()
{
    ZC_ShProgs shProgs;
    ZC_ShPName loadAll { ShPN_LoadAll };
    shProgs.Load(&loadAll, 0);
}