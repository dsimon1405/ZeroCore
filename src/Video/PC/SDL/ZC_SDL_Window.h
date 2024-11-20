#pragma once

#include <Video/ZC_IWindow.h>

#include <SDL3/SDL_video.h>

struct ZC_SDL_Window : public ZC_IWindow
{
    ZC_SDL_Window(int flags, int _width, int _height, const char* name);

private:
    SDL_Window* pWindow = nullptr;
    SDL_GLContext glContext = nullptr;

    bool full_sreen = false;
    
    void VDestroy() override;
    void VSwapBuffer() override;
    void VGetSize(int& width, int& height) const noexcept override;
    void VHideCursor() override;
    void VShowCursor() override;
    void VLimitCursor() override;
    void VUnlimitCursor() override;
    bool VIsCursorLimited() const noexcept override;
    void VStartInputText() override;
    void VStopInputText() override;
    void VSetMaxSize(int x, int y) override;
    void VSetMinSize(int x, int y) override;
    void VSetFullScreen(bool _full_screen) override;
    bool VIsFullScreen() const noexcept override;
    void VGetPosition(int& x, int& y) override;

    bool SetOpenGLAttributes(int samplesCount);
    // bool LoadOpenGLFunctions();
};