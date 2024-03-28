#pragma once

/*
Program entry point.
*/
int ZC_main();

#include <ZC_Config.h>

#ifdef ZC_PC
    #ifdef _NDEBUG
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
    #endif
    int main(int argv, char** args)
    {
        return ZC_main();
    }
#elif defined ZC_ANDROID_NATIVE_APP_GLUE
    #include <ZC/Video/ZC_android_app.h>
    // #include <android/sensor.h>

    void android_main(android_app* pAndroidApp)
    {
    //    ASensorEventQueue_getEvents

        ZC_android_app AndroidApp(pAndroidApp);
        ZC_main();
    }
#endif