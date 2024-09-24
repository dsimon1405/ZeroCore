#pragma once

#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Tools/Time/ZC_FPS_TimeMeasure.h>
#include <ZC/Events/ZC_EC.h>

typedef int ZC_WindowFlags;     //  ZC_SWindow::Flags
/*
System Window management namespace.
Window coords shema:
-   x = 0, y = 1 ----- x = 1, y = 1
-      |               |
-      |               |
-      |               |
-      |               |
-   x = 0, y = 0 ----- x = 1, y = 0
*/
namespace ZC_SWindow
{
    enum Flags
    {
        ZC_SW__None                 = 0,
        ZC_SW__Border               = 1 << 1,   //  if don't set - fullscreen; if set and (width or height <= 0) use fullscreen with border(reduced size will be 800x600), otherwise bordered with width and height windowю
        ZC_SW__Multisampling_1      = 1 << 2,   //  antialiasing with 1 sample on pixel (if Multisampling flags more than one, will take greatest).
        ZC_SW__Multisampling_2      = 1 << 3,   //  antialiasing with 2 samples on pixel (if Multisampling flags more than one, will take greatest).
        ZC_SW__Multisampling_3      = 1 << 4,   //  antialiasing with 3 samples on pixel (if Multisampling flags more than one, will take greatest).
        ZC_SW__Multisampling_4      = 1 << 5,   //  antialiasing with 4 samples on pixel (if Multisampling flags more than one, will take greatest).
        ZC_SW__GUI                  = 1 << 6,   //  creates ZC_GUI context
    };

    /*
    Create window. Parameters have effect only in ZC_PC build.

    Params:
    flags - see ZC_SWindow::Flags.
    width - window with border width.
    height - window with border height.
    name - window on border name.

    Return:
    On success unique pointer of ZC_SWindow, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    bool MakeWindow(ZC_WindowFlags flags = ZC_SW__None, int width = 0, int height = 0, const char* name = "");

    /*
    Set the default context buffer clear color.
    
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
    
    //  Sets the width and height of the window in references.
    void GetSize(int& width, int& height);

    //  Hide mouses cursor (have effect only in ZC_PC build mode).
    void HideCursor();

    //  Make visible mouses cursor (have effect only in ZC_PC build mode).
    void ShowCursor();

    //  Mouses cursor can't break out window (have effect only in ZC_PC build mode).
    void LimitCursor();

    //  Mouses cursor can break out window (have effect only in ZC_PC build mode).
    void UnlimitCursor();

    //  Start processing press keyboard keys in unicode (that function start heavy proccesing of each pressed key and used by ZC_GUI). Don't call it without necessarily!
    void StartInputText();

    //  Stop processing press keyboard keys in unicode (that function used by ZC_GUI). Don't call it without necessarily!
    void StopInputText();

    //  Run cycle (handle events => OpenGL draw).
    void RunMainCycle();

    //  Displays or not the number of frames per second.
    void NeedDrawFPS(bool needDraw);
    
    //  Returns true if the number of frames per second is displayed, false otherwise.
    bool IsFPSDrawing();

    //  Sets the position X, Y coords in references.
    void GetCursorPosition(float& posX, float& posY);
    
    //  Set time measure of fps. Default ZC_FPS_TM__Nanoseconds. Method ZC_SWindow::GetPreviousFrameTime() will return time in that measure. And collbacs from ZC_Event will gave time in that measure.
    void SetFPSTimeMeasure(ZC_FPS_TimeMeasure timeMeasure);

    /*
    Connect to update in each frame. Update is calling after polling events and before collsion.

    Params:
    - func - function to call on update. Parameter (float) is time of previous frame.
    - level - level on wich add function. Order of calling on update.

    Return:
    Event connection for disconnect.
    */
    ZC_EC ConnectUpdate(ZC_Function<void(float)>&& func, size_t level);

    //  Returns number of current frame.
    unsigned long long GetCurrentFrameNumber();
};