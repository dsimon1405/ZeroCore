#pragma once

#include <ZC/Video/ZC_Window.h>
#include <ZC/Video/ZC_android_app.h>

// #include <android/input.h>
#include <EGL/egl.h>

class ZC_AndroidNativeAppGlue_Window : public ZC_SWindowHolder, public ZC_android_app
{
public:
    ZC_AndroidNativeAppGlue_Window();
    
    ~ZC_AndroidNativeAppGlue_Window() override = default;

    bool HandleEvents() override;
    void VSwapBuffer() override;

private:
    EGLDisplay display = nullptr;
    EGLSurface surface = nullptr;
    EGLContext context = nullptr;

    static void HandleCMD(struct android_app* app, int cmd);
    static int HandleInput(struct android_app* app, AInputEvent* pEvent);

    bool InitGraphicOpenGL(ANativeWindow* pWindow);
    void CloseGraphicOpenGL();
};

