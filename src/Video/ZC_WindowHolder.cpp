#include "ZC_WindowHolder.h"

// #include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <Video/OpenGL/Renderer/ZC_Renderer.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#ifdef ZC_SDL_VIDEO
#include "PC/SDL/ZC_SDL_Window.h"
bool ZC_WindowHolder::MakeWindowHolder(int flags, int width, int height, const char* name)
{
    pWindowHolder = ZC_uptrMakeFromChild<ZC_WindowHolder, ZC_SDL_Window>(flags, width, height, name);
    if (!pWindowHolder) return false;
    LoadShProgs();
    ZC_Renderer::Init(true);
    return true;
}
// #elif defined(ZC_ANDROID_NATIVE_APP_GLUE)
// #include "Android/AndroidNativeAppGlue/ZC_AndroidNativeAppGlue_Window.h"
// ZC_upWindow ZC_Window::MakeWindow(bool border, int width, int height, const char* name)
// {
//     return ZC_uptrCreateWithErrorCheck<ZC_Window, ZC_AndroidNativeAppGlue_Window>();
// }
#endif

void ZC_WindowHolder::RunMainCycle()
{
    while (true)
    {
        if (!(upEventsHolder->PollEvents(fps.StartNewFrame()))) break;
        ZC_Renderer::DrawAll();
        pWindowHolder->SwapBuffer();                                          //  передать функцию в ZC_Renderer
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

ZC_WindowHolder::ZC_WindowHolder()
    : upEventsHolder(ZC_EventsHolder::MakeEventsHolder()),
	fps(ZC_FPS::Seconds)
{}

void ZC_WindowHolder::LoadShProgs()
{
    ZC_ShProgs shProgs;
    typedef typename ZC_ShProgs::Name ShPName; 
    ShPName shPNames[] { ShPName::ZCR_Color, ShPName::ZCR_Point, ShPName::ZCR_Line, ShPName::ZCR_Stencil, ShPName::ZCR_Texture_Vertex_TexCoord, ShPName::ZCR_Mesh };
    shProgs.Load(shPNames, sizeof(shPNames) / sizeof(ShPName));
}