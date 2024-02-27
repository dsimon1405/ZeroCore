#pragma once

#include <ZC/Video/ZC_Window.h>
#include <Tools/Time/ZC_FPS.h>
#include <ZC/Events/ZC_Button.h>
#include <ZC/Events/ZC_Mouse.h>

#include <SDL3/SDL_video.h>

struct ZC_SDL_Window : public ZC_Window, public ZC_Button, public ZC_Mouse
{
    SDL_Window* pWindow = nullptr;
    SDL_GLContext glContext = nullptr;
	ZC_FPS fps;

    ZC_SDL_Window(bool border, int _width, int _height, const char* name);
    ~ZC_SDL_Window() override;

    bool HandleEvents() override;
    void SwapBuffer() override;
    void VSetFPS(long limit) noexcept override;
    float VGetPreviousFrameTime() const noexcept override;
    void VGetSize(int& width, int& height) const noexcept override;
    void VHideCursor() override;
    void VShowCursor() override;
    void VLimitCursor() override;
    void VUnlimitCursor() override;

private:
    bool SetOpenGLAttributes();
    void Resize();
    bool LoadOpenGLFunctions();
};