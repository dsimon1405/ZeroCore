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

void ZC_System::RunMainCycle()
{
    if (!upIWindow) return;
    if (upGUI) upGUI->Configure();
    fps.StartNewFrame();    //  make prepearing call, to avoid false large information about frist frmae time
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