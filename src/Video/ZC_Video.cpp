#include <ZC/Video/ZC_Video.h>

#include <ZC_Config.h>
#include <Tools/ZC_uptrCreateWithErrorCheck.h>

#include <memory>

#if defined(ZC_SDL_VIDEO)
    #include "PC/SDL/ZC_SDL_Window.h"
    ZC_upWindow ZC_Video::MakeWindow(bool border, int width, int height, const char* name)
    {
        return ZC_uptrCreateWithErrorCheck<ZC_Window, ZC_SDL_Window>(border, width, height, name);
    }
#elif defined(ZC_ANDROID_NATIVE_APP_GLUE)
    #include "Android/AndroidNativeAppGlue/ZC_AndroidNativeAppGlue_Window.h"
    ZC_upWindow ZC_Video::MakeWindow(bool border, int width, int height, const char* name)
    {
        return ZC_uptrCreateWithErrorCheck<ZC_Window, ZC_AndroidNativeAppGlue_Window>();
    }
#endif