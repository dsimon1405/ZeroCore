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
    upWindowHolder->renderer.Configure(true);  //  make user chois use 3D grapfics or not in ZC_Window::Flags ? (depth test) (need change stencil test =( )
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
        renderer.DrawAll();
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
	fps(ZC_FPS::Seconds),
    renderer({ &ZC_WindowHolder::SwapBuffer, this })
{}

void ZC_WindowHolder::LoadShProgs()
{
    ZC_ShProgs shProgs;
    typedef typename ZC_ShProgs::Name ShPName; 
    ShPName shPNames[] { ShPName::ZCR_ColorFigure, ShPName::ZCR_Point, ShPName::ZCR_LineFigure, ShPName::ZCR_Stencil, ShPName::ZCR_Texture_Vertex_TexCoord,
        ShPName::ZCR_LineMesh, ShPName::ZCR_LineOrientation3D };
    shProgs.Load(shPNames, sizeof(shPNames) / sizeof(ShPName));
}