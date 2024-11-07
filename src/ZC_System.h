#pragma once

#include <Video/ZC_IWindow.h>
#include <Events/ZC_EventsHolder.h>
#include <Tools/Time/ZC_FPS.h>
#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>
#include <ZC/GUI/Backend/System/ZC_GUI.h>
#include <Collision/ZC_CollisionManager.h>
#include <Tools/ZC_Updater.h>
#include <ZC/Tools/ZC_uptr.h>

struct ZC_System
{
    static inline ZC_uptr<ZC_System> upSystem;

    ZC_uptr<ZC_IWindow> upIWindow;
    ZC_uptr<ZC_EventsHolder> upEventsHolder;
	ZC_FPS fps;
    ZC_Renderer renderer;
    ZC_uptr<ZC_GUI> upGUI;
    ZC_uptr<ZC_CollisionManager> upCollision_manager;
    ZC_uptr<ZC_Updater> upUpdater;

    ~ZC_System();

    static bool Init(int system_flags, int win_flags, int win_width, int win_height, const char* win_name);
    
    void RunMainCycle();
    void BreakMainCycle();

private:
    bool is_main_cycle_run = true;

    ZC_System(ZC_uptr<ZC_IWindow>&& _upWindowHolder, int system_flags);

    static void LoadShProgs();
};