#pragma once


typedef int ZC_WindowFlags;     //  ZC_Window::Flags
/*
Window management namespace.

Window coords:
    top left corner: x = 0, y = 0;
    top right corner: x = window width, y = 0;
    bottom left corner: x = 0, y = window heigth;
    bottom right corner: x = window with , y = window height.
*/
namespace ZC_Window
{
    enum Flags
    {
        ZC_Window_None                = 0,
        ZC_Window_Border              = 1 << 1,   //  if don't set - fullscreen; if set and (width or height <= 0) use fullscreen with border(reduced size will be 800x600),
                                                  //  otherwise bordered with width and height windowю
        ZC_Window_Multisampling_1     = 1 << 2,   //  antialiasing with 1 sample on pixel (if Multisampling flags more than one, will take greatest).
        ZC_Window_Multisampling_2     = 1 << 3,   //  antialiasing with 2 samples on pixel (if Multisampling flags more than one, will take greatest).
        ZC_Window_Multisampling_3     = 1 << 4,   //  antialiasing with 3 samples on pixel (if Multisampling flags more than one, will take greatest).
        ZC_Window_Multisampling_4     = 1 << 5    //  antialiasing with 4 samples on pixel (if Multisampling flags more than one, will take greatest).
    };

    /*
    Create window. Parameters have effect only in ZC_PC build.

    Params:
    flags - see ZC_Window::Flags.
    width - window with border width.
    height - window with border height.
    name - window on border name.

    Return:
    On success unique pointer of ZC_Window, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    bool MakeWindow(ZC_WindowFlags flags = ZC_Window_None, int width = 0, int height = 0, const char* name = "");

    /*
    Set the window buffer clear color.

    Params:
    r - red color.
    g - green color.
    b - blue color.
    a - alpha channel.
    */
    void GlClearColor(float r, float g, float b, float a);

    //  Enables set points size in shader prograrm.
    void GlEnablePointSize();

    /*
    Sets the number of frames per second. Default value = 60.

    Params:
    limit - number of frames - must be >= 0 (0 - unlimit, everything is in the hands of your machine).
    */
    void SetFPS(long limit) noexcept;

    //  Returns the creation time of the previous frame.
    float GetPreviousFrameTime() noexcept;
    
    //  Returns the width of the window.
    void GetSize(int& width, int& height);

    //  Hide mouses cursor (have effect only in ZC_PC build mode).
    void HideCursor();

    //  Make visible mouses cursor (have effect only in ZC_PC build mode).
    void ShowCursor();

    //  Mouses cursor can't break out window (have effect only in ZC_PC build mode).
    void LimitCursor();

    //  Mouses cursor can break out window (have effect only in ZC_PC build mode).
    void UnlimitCursor();

    //  Run cycle (handle events => OpenGL draw).
    void RuntMainCycle();
};