#include "ZC_WindowHolder.h"

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#ifdef ZC_SDL_VIDEO
#include "PC/SDL/ZC_SDL_Window.h"
bool ZC_WindowHolder::MakeWindowHolder(int flags, int width, int height, const char* name)
{
    upWindowHolder = ZC_uptrMakeFromChild<ZC_WindowHolder, ZC_SDL_Window>(flags, width, height, name);
    if (!upWindowHolder) return false;
    upWindowHolder->LoadShProgs();
    upWindowHolder->AddZC_Render();
    return true;
}
// #elif defined(ZC_ANDROID_NATIVE_APP_GLUE)
// #include "Android/AndroidNativeAppGlue/ZC_AndroidNativeAppGlue_Window.h"
// ZC_upWindow ZC_Window::MakeWindow(bool border, int width, int height, const char* name)
// {
//     return ZC_uptrCreateWithErrorCheck<ZC_Window, ZC_AndroidNativeAppGlue_Window>();
// }
#endif

ZC_WindowHolder::~ZC_WindowHolder()
{
    fps.NeedDraw(false);
    ZC_Renders::EraseRender(ZC_RL_Default);
}

void ZC_WindowHolder::RunMainCycle()
{
    while (true)
    {
        if (!(upEventsHolder->PollEvents(fps.StartNewFrame()))) break;
        renderer.Draw();
    }
}

void ZC_WindowHolder::SetFPS(long limit)
{
	fps.SetLimit(limit);
}

float ZC_WindowHolder::GetPreviousFrameTime() const noexcept
{
	return fps.PreviousFrameTime();
}

void ZC_WindowHolder::NeedDrawFPS(bool needDraw)
{
    fps.NeedDraw(needDraw);
}

bool ZC_WindowHolder::IsFPSDrawing()
{
    return fps.IsDrawing();
}

void ZC_WindowHolder::GetCursorPosition(float& posX, float& posY)
{
    upEventsHolder->GetCursorPosition(posX, posY);
}

ZC_WindowHolder::ZC_WindowHolder()
    : upEventsHolder(ZC_EventsHolder::MakeEventsHolder()),
	fps(ZC_FPS::Seconds)
,
    renderer({ &ZC_WindowHolder::SwapBuffer, this })
{}

void ZC_WindowHolder::LoadShProgs()
{
    ZC_ShProgs shProgs;
    ZC_ShPName loadAll { ShPN_LoadAll };
    shProgs.Load(&loadAll, 0);
}

void ZC_WindowHolder::AddZC_Render()
{
    ZC_Renders::CreateRender(ZC_RL_Default, ZC_Render::DS_Loop, ZC_FBO());
}