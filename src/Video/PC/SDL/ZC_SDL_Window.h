#pragma once

#include <Video/ZC_SWindowHolder.h>

#include <SDL3/SDL_video.h>

struct ZC_SDL_Window : public ZC_SWindowHolder
{
    ZC_SDL_Window(int flags, int _width, int _height, const char* name);
    ~ZC_SDL_Window() override;

private:
    SDL_Window* pWindow = nullptr;
    SDL_GLContext glContext = nullptr;

    void VSwapBuffer() override;
    void VGetSize(int& width, int& height) const noexcept override;
    void VHideCursor() override;
    void VShowCursor() override;
    void VLimitCursor() override;
    void VUnlimitCursor() override;
    void VStartInputText() override;
    void VStopInputText() override;
    void VSetMaxSize(int x, int y) override;
    void VSetMinSize(int x, int y) override;
    void VSetFullScreen(bool full_screen) override;

    bool SetOpenGLAttributes(int samplesCount);
    // bool LoadOpenGLFunctions();
};