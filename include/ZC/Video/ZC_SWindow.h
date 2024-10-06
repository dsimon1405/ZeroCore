#pragma once

#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Tools/Time/ZC_FPS_TimeMeasure.h>
#include <ZC/Events/ZC_EC.h>

enum ZC_SW_Flags
{
    ZC_SWF__None                 = 0,
    ZC_SWF__Border               = 1 << 1,   //  if don't set - fullscreen; if set and (width or height <= 0) use fullscreen with border(reduced size will be 800x600), otherwise bordered with width and height windowю
    ZC_SWF__Multisampling_1      = 1 << 2,   //  antialiasing with 1 sample on pixel (if Multisampling flags more than one, will take greatest).
    ZC_SWF__Multisampling_2      = 1 << 3,   //  antialiasing with 2 samples on pixel (if Multisampling flags more than one, will take greatest).
    ZC_SWF__Multisampling_3      = 1 << 4,   //  antialiasing with 3 samples on pixel (if Multisampling flags more than one, will take greatest).
    ZC_SWF__Multisampling_4      = 1 << 5,   //  antialiasing with 4 samples on pixel (if Multisampling flags more than one, will take greatest).
    ZC_SWF__GUI                  = 1 << 6,   //  creates ZC_GUI context
};

typedef int ZC_WindowFlags;     //  ZC_SWindow::ZC_SW_Flags
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
    bool MakeWindow(ZC_WindowFlags flags = ZC_SWF__None, int width = 0, int height = 0, const char* name = "");
    
    //  Closing window.
    void CloseWindow();

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

    //  Return the time of the previous frame in setted timeasure setted with SetFPSTimeMeasure(). Default is ZC_FPS_TM__Nanoseconds.
    float GetPreviousFrameTime() noexcept;

    //  Return the time of the previous frame in the specified time measure.
    float GetPreviousFrameTime(ZC_FPS_TimeMeasure time_measure) noexcept;

    //  Return setted time measure in fps system. Default is ZC_FPS_TM__Nanoseconds.
    ZC_FPS_TimeMeasure GetFPSTimeMeasure() noexcept;
    
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
    ZC_EC ConnectToUpdater(ZC_Function<void(float)>&& func, size_t level);

    //  Returns number of current frame.
    unsigned long long GetCurrentFrameNumber();

    //  Sets window max size in pixels.
    void SetMaxSize(int x, int y);

    //  Sets window min size in pixels.
    void SetMinSize(int x, int y);

    //  Stop or start updater activity. Default is on.
    void ChangeUpdaterState(bool needUpdate);

    //  Change activity of the concrete level of the updater.
    void ChangeUpdaterLevelState(size_t lvl, bool is_active);

    //  Change full screen mode. If full_screen is true, the window will be full screen.
    void SetFullScreen(bool full_screen);
};