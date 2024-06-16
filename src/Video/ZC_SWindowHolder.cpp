#include "ZC_SWindowHolder.h"

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShaderManager.h>

#ifdef ZC_SDL_VIDEO
#include "PC/SDL/ZC_SDL_Window.h"
bool ZC_SWindowHolder::MakeWindowHolder(int flags, int width, int height, const char* name)
{
    upWindowHolder = ZC_uptrMakeFromChild<ZC_SWindowHolder, ZC_SDL_Window>(flags, width, height, name);
    if (!upWindowHolder) return false;
    upWindowHolder->LoadShProgs();
    upWindowHolder->AddZC_Render();
    upWindowHolder->gui.Init();
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

void ZC_SWindowHolder::RunMainCycle()
{
    gui.Configure();
    while (true)
    {
        if (!(upEventsHolder->PollEvents(fps.StartNewFrame(), gui.eventManager))) break;
        renderer.Draw(gui);
    }
}

void ZC_SWindowHolder::SetFPS(long limit)
{
	fps.SetLimit(limit);
}

float ZC_SWindowHolder::GetPreviousFrameTime() const noexcept
{
	return fps.PreviousFrameTime();
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

ZC_SWindowHolder::ZC_SWindowHolder()
    : upEventsHolder(ZC_EventsHolder::MakeEventsHolder()),
	fps(ZC_FPS::Seconds)
,
    renderer({ &ZC_SWindowHolder::SwapBuffer, this })
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