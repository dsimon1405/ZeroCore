#include "ZC_AndroidNativeAppGlue_Window.h"

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <Video/OpenGL/Android/ZC_AndroidOpenGLReloder.h>

//#include <ZC/Audio/ZC_AudioStream.h>
#include <ZC/Audio/ZC_Audio.h>

ZC_AndroidNativeAppGlue_Window::ZC_AndroidNativeAppGlue_Window()
{
    if (!pAndroidApp)
    {
        ZC_ErrorLogger::Err("ZC_android_app::pAndroidApp = nullptr!");
        return;
    }

    pAndroidApp->userData = this;
    pAndroidApp->onAppCmd = HandleCMD;
    pAndroidApp->onInputEvent = HandleInput;

    while (true)
    {
        HandleEvents();
        if (display) break;
    }
}

bool ZC_AndroidNativeAppGlue_Window::HandleEvents()
{
    static int events;
    static struct android_poll_source* pSource;

    if(display)
    {

    }

    if (ALooper_pollOnce( 0, nullptr, &events, (void**)&pSource) >= 0)
    {
        // Process this event.
        if (pSource)
        {
            pSource->process(pAndroidApp, pSource);
        }

        // stop application
        if (pAndroidApp->destroyRequested != 0)
        {
            CloseGraphicOpenGL();
            return false;
        }
    }
    return true;
}

void ZC_AndroidNativeAppGlue_Window::SwapBuffer()
{
//    currentFrame++;
    eglSwapBuffers(display, surface);
}
#include <ZC/Tools/Console/ZC_cout.h>
int ZC_AndroidNativeAppGlue_Window::HandleInput(struct android_app* app, AInputEvent* pEvent)
{
    auto pWindow = static_cast<ZC_AndroidNativeAppGlue_Window*>(app->userData);
//    static int frame = 0;
//    if (pWindow->currentFrame != frame)
//    {
//        frame = pWindow->currentFrame;
        if (AInputEvent_getType(pEvent) == AINPUT_EVENT_TYPE_MOTION)
        {
            ZC_cout("motion");
        }
//        {
//            auto x = AMotionEvent_getX(pEvent, 0);
//            auto y = AMotionEvent_getY(pEvent, 0);
//
//            if (x <= 360 && y <= 730)
//            {
//                pWindow->red = 1;
//                pWindow->green = 0;
//                pWindow->blue = 0;
//            }
//            if (x >= 360 && y <= 730)
//            {
//                pWindow->red = 0;
//                pWindow->green = 1;
//                pWindow->blue = 0;
//            }
//            if (x <= 360 && y >= 730)
//            {
//                pWindow->red = 0;
//                pWindow->green = 0;
//                pWindow->blue = 1;
//            }
//            if (x >= 360 && y >= 730)
//            {
//                pWindow->red = 0;
//                pWindow->green = 0;
//                pWindow->blue = 0;
//            }
//            pWindow->ResetClearColor();
//            return false;
//        }
//    }
    return true;
}

void ZC_AndroidNativeAppGlue_Window::HandleCMD(struct android_app* app, int cmd)
{
    auto pWindow = static_cast<ZC_AndroidNativeAppGlue_Window*>(app->userData);
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
        {
//            ZC_SavedState state;
//            state.vec.emplace_back(0);
//            state.vec.emplace_back(1);
//            state.vec.emplace_back(0);
//            pAndroidApp->savedState = malloc(sizeof(ZC_SavedState));
//            *((ZC_SavedState*)pAndroidApp->savedState) = state;
//            pAndroidApp->savedStateSize = sizeof(ZC_SavedState);
            break;
        }

        case APP_CMD_INIT_WINDOW:
        {
            if (app->window && pWindow->InitGraphicOpenGL(app->window))
            {
                ZC_AndroidOpenGLReloader::sReload();
            }
            break;
        }

        case APP_CMD_TERM_WINDOW:
        {
            pWindow->CloseGraphicOpenGL();
            break;
        }

        case APP_CMD_GAINED_FOCUS:
        {
            ZC_Audio::ReopenAudioStream();
            break;
        }

        case APP_CMD_LOST_FOCUS:
        {
            ZC_Audio::CloseAudioStream();
            break;
        }

            /**
             * Command from main thread: the AInputQueue has changed.  Upon processing
             * this command, android_app->inputQueue will be updated to the new queue
             * (or NULL).
             */
        case APP_CMD_INPUT_CHANGED:
        {
            break;
        }
        case APP_CMD_WINDOW_RESIZED:
        {
            break;
        }
            /**
             * Command from main thread: the system needs that the current ANativeWindow
             * be redrawn.  You should redraw the window before handing this to
             * android_app_exec_cmd() in order to avoid transient drawing glitches.
             */
        case APP_CMD_WINDOW_REDRAW_NEEDED:
        {
            break;
        }
            /**
             * Command from main thread: the content area of the window has changed,
             * such as from the soft input window being shown or hidden.  You can
             * find the new content rect in android_app::contentRect.
             */
        case APP_CMD_CONTENT_RECT_CHANGED:
        {
            break;
        }
            /**
             * Command from main thread: the current device configuration has changed.
             */
        case APP_CMD_CONFIG_CHANGED:
        {
            break;
        }
            /**
             * Command from main thread: the system is running low on memory.
             * Try to reduce your memory use.
             */
        case APP_CMD_LOW_MEMORY:
        {
            break;
        }
            /**
             * Command from main thread: the app's activity has been started.
             */
        case APP_CMD_START:
        {
            break;
        }
            /**
             * Command from main thread: the app's activity has been resumed.
             */
        case APP_CMD_RESUME:
        {
            break;
        }
            /**
             * Command from main thread: the app's activity is being destroyed,
             * and waiting for the app thread to clean up and exit before proceeding.
             */
        case APP_CMD_DESTROY:
        {
            break;
        }
            /**
             * Command from main thread: the app's activity has been paused.
             */
        case APP_CMD_PAUSE:
        {
            break;
        }
        default:
            break;
    }
}

bool ZC_AndroidNativeAppGlue_Window::InitGraphicOpenGL(ANativeWindow* pWindow)
{
    const EGLint attribs[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
                              EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                              EGL_BLUE_SIZE,    ZC_OPEN_GL_COLLOR_BUFFER_SIZE,
                              EGL_GREEN_SIZE,   ZC_OPEN_GL_COLLOR_BUFFER_SIZE,
                              EGL_RED_SIZE,     ZC_OPEN_GL_COLLOR_BUFFER_SIZE,
                              EGL_DEPTH_SIZE, ZC_OPEN_GL_DEPTH_BUFFER_SIZE,
                              EGL_NONE};

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == nullptr)
    {
        ZC_ErrorLogger::Err("eglGetDisplay() fail!", __FILE__, __LINE__);
        return false;
    }

    if (eglInitialize(display, nullptr, nullptr) == EGL_FALSE)
    {
        eglTerminate(display);
        display = nullptr;
        ZC_ErrorLogger::Err("eglInitialize() fail!", __FILE__, __LINE__);
        return false;
    }

    EGLint numConfigs = 0;
    if (eglChooseConfig(display, attribs, nullptr, 0, &numConfigs) == EGL_FALSE)
    {
        eglTerminate(display);
        display = nullptr;
        ZC_ErrorLogger::Err("eglChooseConfig() fail!", __FILE__, __LINE__);
        return false;
    }

    EGLConfig supportedConfigs[numConfigs];
    if (eglChooseConfig(display, attribs, supportedConfigs, numConfigs, &numConfigs) == EGL_FALSE)
    {
        eglTerminate(display);
        display = nullptr;
        ZC_ErrorLogger::Err("eglChooseConfig() fail!", __FILE__, __LINE__);
        return false;
    }

    EGLConfig config = nullptr;
    for (int i = 0; i < numConfigs; ++i)
    {
        EGLint red, green, blue, depth;
        if (eglGetConfigAttrib(display, supportedConfigs[i], EGL_RED_SIZE, &red)
            && eglGetConfigAttrib(display, supportedConfigs[i], EGL_GREEN_SIZE, &green)
            && eglGetConfigAttrib(display, supportedConfigs[i], EGL_BLUE_SIZE, &blue)
            && eglGetConfigAttrib(display, supportedConfigs[i], EGL_DEPTH_SIZE, &depth)
            && red == ZC_OPEN_GL_COLLOR_BUFFER_SIZE
            && green == ZC_OPEN_GL_COLLOR_BUFFER_SIZE
            && blue == ZC_OPEN_GL_COLLOR_BUFFER_SIZE
            && depth == ZC_OPEN_GL_DEPTH_BUFFER_SIZE)
        {
            config = supportedConfigs[i];
            break;
        }
    }
    if (!config) config = supportedConfigs[0];

    EGLint format = 0;
    if (eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format) == EGL_FALSE)
    {
        eglTerminate(display);
        display = nullptr;
        ZC_ErrorLogger::Err("eglGetConfigAttrib() fail!", __FILE__, __LINE__);
        return false;
    }

    surface = eglCreateWindowSurface(display, config, pWindow, NULL);
    if (surface == nullptr)
    {
        eglTerminate(display);
        display = nullptr;
        ZC_ErrorLogger::Err("eglCreateWindowSurface() fail!", __FILE__, __LINE__);
        return false;
    }

    EGLint contextAttribs[] =
            {
                    EGL_CONTEXT_MAJOR_VERSION, ZC_OPEN_GL_MAJOR_VERSION,
                    EGL_CONTEXT_MINOR_VERSION, ZC_OPEN_GL_MINOR_VERSION,
                    EGL_NONE
            };

    context = eglCreateContext(display, config, NULL, contextAttribs);
    if (context == nullptr)
    {
        eglDestroySurface(display, surface);
        eglTerminate(display);
        display = nullptr;
        surface = nullptr;
        ZC_ErrorLogger::Err("eglCreateContext() fail!", __FILE__, __LINE__);
        return false;
    }

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        eglDestroySurface(display, surface);
        eglDestroyContext(display, context);
        eglTerminate(display);
        display = nullptr;
        context = nullptr;
        surface = nullptr;
        ZC_ErrorLogger::Err("eglMakeCurrent() fail!", __FILE__, __LINE__);
        return false;
    }

    if (eglQuerySurface(display, surface, EGL_WIDTH, &width) == EGL_FALSE)
    {
        eglDestroySurface(display, surface);
        eglDestroyContext(display, context);
        eglTerminate(display);
        display = nullptr;
        context = nullptr;
        surface = nullptr;
        ZC_ErrorLogger::Err("eglQuerySurface(EGL_WIDTH) fail!", __FILE__, __LINE__);
        return false;
    }
    if (eglQuerySurface(display, surface, EGL_HEIGHT, &height) == EGL_FALSE)
    {
        eglDestroySurface(display, surface);
        eglDestroyContext(display, context);
        eglTerminate(display);
        display = nullptr;
        context = nullptr;
        surface = nullptr;
        ZC_ErrorLogger::Err("eglQuerySurface(EGL_HEIGHT) fail!", __FILE__, __LINE__);
        return false;
    }

//    const char* a = (const char*)glGetString(GL_VERSION);
    ZC_OpenGLAssigneErrorCallback();
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    ZC_ErrorLogger::Clear();
    return true;
}

void ZC_AndroidNativeAppGlue_Window::CloseGraphicOpenGL()
{
    if (display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(display, context);
        }

        if (surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(display, surface);
        }

        eglTerminate(display);
    }

    display = nullptr;
    context = nullptr;
    surface = nullptr;
}