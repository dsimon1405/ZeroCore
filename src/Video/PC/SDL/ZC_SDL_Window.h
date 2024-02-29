#pragma once

#include <Video/ZC_WindowHolder.h>

#include <SDL3/SDL_video.h>

struct ZC_SDL_Window : public ZC_WindowHolder
{
    ZC_SDL_Window(int flags, int _width, int _height, const char* name);
    ~ZC_SDL_Window() override;

private:
    SDL_Window* pWindow = nullptr;
    SDL_GLContext glContext = nullptr;

    void SwapBuffer() override;
    void VGetSize(int& width, int& height) const noexcept override;
    void VHideCursor() override;
    void VShowCursor() override;
    void VLimitCursor() override;
    void VUnlimitCursor() override;

    bool SetOpenGLAttributes(int samplesCount);
    bool LoadOpenGLFunctions();
};