#pragma once

#include <ZC/Tools/Math/ZC_Vec.h>
#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC_Config.h>
#include <ZC/Tools/Signal/ZC_Signal.h>

class ZC_Window;
using ZC_upWindow = ZC_uptr<ZC_Window>;

/*
Window management class

Window coords:
    top left corner: x = 0, y = 0;
    top right corner: x = window width, y = 0;
    bottom left corner: x = 0, y = window heigth;
    bottom right corner: x = window with , y = window height.
*/
class ZC_Window
{
public:

    /*
    Create window. Parameters have effect only in ZC_PC build.

    Params:
    border - on false - fullscrean window without border(ignore all next parameters), on true - window with border (if width <= 0 or height <= 0 fullscreen).
    width - window with border width.
    height - window with border height.
    name - window on border name.

    Return:
    On success unique pointer of ZC_Window, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    static ZC_upWindow MakeWindow(bool border = false, int width = 0, int height = 0, const char* name = "");

    ZC_Window(const ZC_Window&) = delete;
    ZC_Window& operator = (const ZC_Window&) = delete;

    ZC_Window(ZC_Window&& win) = delete;
    ZC_Window& operator = (ZC_Window&& win) = delete;

    virtual ~ZC_Window();
    
    /*
    Set the window buffer clear color.

    Params:
    r - red color.
    g - green color.
    b - blue color.
    */
    void SetClearColor(float r, float g, float b);

    /*
    Clear the window buffer.
    */
    void Clear(GLbitfield mask);
    
    /*
    Handle input events.

    Return:
    If there was an event that required closing the window, false, otherwise true.
    */
    virtual bool HandleEvents() = 0;

    /*
    Changes the current buffer to a buffer with a prepared sketch
    */
    virtual void SwapBuffer() = 0;

    /*
    Sets the number of frames per second. Default value = 60.

    Params:
    limit - number of frames - must be >= 0 (0 - unlimit, everything is in the hands of your machine).
    */
    static void SetFPS(long limit) noexcept;

    //  Returns the creation time of the previous frame.
    static float GetPreviousFrameTime();
    
    /*
    Returns the width of the window.
    */
    static void GetSize(int& width, int& height);

    /*
    Hide mouses cursor (have effect only in ZC_PC build mode).
    */
    static void HideCursor();

    /*
    Make visible mouses cursor (have effect only in ZC_PC build mode).
    */
    static void ShowCursor();

    /*
    Mouses cursor can't break out window (have effect only in ZC_PC build mode).
    */
    static void LimitCursor();

    /*
    Mouses cursor can break out window (have effect only in ZC_PC build mode).
    */
    static void UnlimitCursor();

    static ZC_SConnection ConnectResize(ZC_Function<void(float,float)>&& func);

    static ZC_Window* GetCurrentWindow();

protected:
    ZC_Window();

    ZC_Signal<void(float,float)> sigResize { false };

private:
    static inline ZC_Window* pWindow = nullptr;

    virtual void VSetFPS(long limit) noexcept = 0;
    virtual float VGetPreviousFrameTime() const noexcept = 0;
    virtual void VGetSize(int& width, int& height) const noexcept = 0;
    virtual void VHideCursor() {}
    virtual void VShowCursor() {}
    virtual void VLimitCursor() {}
    virtual void VUnlimitCursor() {}
    virtual void VResize() {}
};