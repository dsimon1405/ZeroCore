#pragma once

#include <ZC/Tools/ZC_uptr.h>

class ZC_IWindow
{
public:
    static ZC_uptr<ZC_IWindow> MakeWindowHolder(int flags, int width, int height, const char* name);

    virtual ~ZC_IWindow() = default;

    virtual void VDestroy() = 0;
    virtual void VGetSize(int& width, int& height) const noexcept = 0;
    virtual void VSwapBuffer() = 0;
    virtual void VHideCursor() {}
    virtual void VShowCursor() {}
    virtual void VLimitCursor() {}
    virtual void VUnlimitCursor() {}
    virtual void VStartInputText() {}
    virtual void VStopInputText() {}
    virtual void VSetMaxSize(int x, int y) {}
    virtual void VSetMinSize(int x, int y) {}
    virtual void VSetFullScreen(bool full_screen) {}

protected:
    ZC_IWindow() = default;
};