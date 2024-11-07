#include "ZC_IWindow.h"

#include <ZC_Config.h>

#ifdef ZC_SDL_VIDEO
#include <Video/PC/SDL/ZC_SDL_Window.h>
ZC_uptr<ZC_IWindow> ZC_IWindow::MakeWindowHolder(int flags, int width, int height, const char* name)
{
    return ZC_uptrMakeFromChild<ZC_IWindow, ZC_SDL_Window>(flags, width, height, name);
}
// #elif defined(ZC_ANDROID_NATIVE_APP_GLUE)
// #include "Android/AndroidNativeAppGlue/ZC_AndroidNativeAppGlue_Window.h"
// ZC_upWindow ZC_SWindow::MakeWindow(bool border, int width, int height, const char* name)
// {
//     return ZC_uptrCreateWithErrorCheck<ZC_SWindow, ZC_AndroidNativeAppGlue_Window>();
// }
#endif