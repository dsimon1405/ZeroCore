#pragma once

#include <ZC/Video/ZC_Window.h>
#include <Tools/Time/PC/SDL/ZC_SDL_FPS.h>
#include <ZC/Events/Button/ZC_ButtonOperator.h>
#include <ZC/Events/ZC_Mouse.h>

#include <SDL3/SDL_video.h>

class ZC_SDL_Window : public ZC_Window, public ZC_ButtonOperator, public ZC_Mouse
{
public:
    ZC_SDL_Window(bool border, int _width, int _height, const char* name);

    ~ZC_SDL_Window() override;

    bool HandleEvents() override;
    void SwapBuffer() override;
    void SetFPS(long limit) noexcept override;
    float GetPreviousFrameTime() const noexcept override;
    
    void HideCursor() override;
    void ShowCursor() override;
    void LimitCursor() override;
    void UnlimitCursor() override;
#ifdef ZC_IMGUI
    bool InitImGui() override;
#endif

    static void ConnectResize(ZC_Function<void(float,float)>&& func) noexcept;

private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
	ZC_SDL_FPS fps;
#ifdef ZC_IMGUI
    bool imgui = false;
#endif
    
    static inline ZC_Function<void(float,float)> fResize;

    bool SetOpenGLAttributes();
    void Resize();
};