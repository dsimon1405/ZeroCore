#pragma once

#include <ZC/Tools/Math/ZC_Vec.h>
#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC_Config.h>

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
    ZC_Window(const ZC_Window&) = delete;
    ZC_Window& operator = (const ZC_Window&) = delete;

    ZC_Window(ZC_Window&& win) = delete;
    ZC_Window& operator = (ZC_Window&& win) = delete;

    virtual ~ZC_Window() = default;
    
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
    virtual void SetFPS(long limit) noexcept {}     //  сделать = 0;

    //  Returns the creation time of the previous frame.
    virtual float GetPreviousFrameTime() const noexcept {}     //  сделать = 0;
    
    /*
    Returns the width of the window.
    */
    static int GetWidth() noexcept;
    
    /*
    Returns the height and height of the window.
    */
    static int GetHeight() noexcept;

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
    Hide mouses cursor (have effect only in ZC_PC build mode).
    */
    virtual void HideCursor() {}

    /*
    Make visible mouses cursor (have effect only in ZC_PC build mode).
    */
    virtual void ShowCursor() {}

    /*
    Mouses cursor can't break out window (have effect only in ZC_PC build mode).
    */
    virtual void LimitCursor() {}

    /*
    Mouses cursor can break out window (have effect only in ZC_PC build mode).
    */
    virtual void UnlimitCursor() {}

#ifdef ZC_IMGUI
    virtual bool InitImGui() { return false; }
#endif

protected:
    static inline int width = 0,
        height = 0;

    ZC_Window() = default;
};