#pragma once

#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Tools/Time/ZC_FPS_TimeMeasure.h>
#include <ZC/Events/ZC_EC.h>

enum ZC_SystemFlag
{
    ZC_SF__None         = 0,
    ZC_SF__GUI          = 1 << 1,   //  create ZC__GUI system
    ZC_SF__Updater      = 1 << 2,   //  create ZC__Updater system
    ZC_SF__Collision    = 1 << 3,   //  create ZC__Collision system
};
typedef int ZC_SystemFlags;

enum ZC_SW_Flags
{
    ZC_SWF__None                 = 0,
    ZC_SWF__Border               = 1 << 1,   //  if don't set - fullscreen; if set and (width or height <= 0) use fullscreen with border(reduced size will be 800x600), otherwise bordered with width and height from params
    ZC_SWF__Multisampling_1      = 1 << 2,   //  antialiasing with 1 sample on pixel (if Multisampling flags more than one, will take greatest).
    ZC_SWF__Multisampling_2      = 1 << 3,   //  antialiasing with 2 samples on pixel (if Multisampling flags more than one, will take greatest).
    ZC_SWF__Multisampling_3      = 1 << 4,   //  antialiasing with 3 samples on pixel (if Multisampling flags more than one, will take greatest).
    ZC_SWF__Multisampling_4      = 1 << 5,   //  antialiasing with 4 samples on pixel (if Multisampling flags more than one, will take greatest).
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
namespace ZC__System
{
    /*
    Creates ZC__Window, ZC_Event, ZC__FPS systems amd additional according system_flags.

    Params:
    - system_flags - create systems from the window.
    - win_flags - see ZC_SWindow::Flags.
    - win_width - window with border width.
    - win_height - window with border height.
    - win_name - window on border name.

    Return:
    On success true, otherwise false.
    */
    bool Init(ZC_SystemFlags system_flags, ZC_WindowFlags win_flags = ZC_SWF__None, int win_width = 0, int win_height = 0, const char* win_name = "");

    /*
    Run main cycle. At first configure ZC_GUI if it is, then start cycle. To break cycle call BreakMainCycle().
    Cycle shama:
    - upadte fps data.
    - poll events (call functions connected to events).
    - call updater.
    - call collision manager.
    - render scene.
    */
    void RunMainCycle();

    //  Break from fucntion RunMainCycle(), destroy ZC_SWindow and ZC_AudioSteram. 
    void BreakMainCycle();

    /*
    Set the default context buffer clear color. Default color is black (0,0,0,1).
    
    Params:
    r - red color.
    g - green color.
    b - blue color.
    a - alpha channel.
    */
    void GlClearColor(float r, float g, float b, float a);

    //  Enables set points size in shader program.
    void GlEnablePointSize();
} // ZC__System


namespace ZC__Window
{
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

    //  Return true if cursor can't break out window, otherwise false.
    bool IsCursorLimited();

    //  Start processing press keyboard keys in unicode (that function start heavy proccesing of each pressed key and used by ZC_GUI). Don't call it without necessarily!
    void StartInputText();

    //  Stop processing press keyboard keys in unicode (that function used by ZC_GUI). Don't call it without necessarily!
    void StopInputText();

    //  Sets the position X, Y coords in references. Position counting from bottom left corner of the window, like in OpenGL!
    void GetCursorPosition(float& posX, float& posY);

    //  Sets window max size in pixels.
    void SetMaxSize(int x, int y);

    //  Sets window min size in pixels.
    void SetMinSize(int x, int y);

    //  Change full screen mode. If full_screen is true, the window will be full screen.
    void SetFullScreen(bool full_screen);
    
    //  Returns true on borderless fullscreen mode, otherwise false.
    bool IsFullScreen();

    //  Sets to references TOP(y) LEFT(x) position of the window into the display. Positoin counting from TOP LEFT corner of the display to TOP LEFT corner of the window.
    void GetPosition(int& x, int& y);

    //  Sets the display width and height in references in pixels, or 0 in case of error.
    void GetDisplaySize(int& width, int& height);
} // ZC__Window


namespace ZC__FPS
{
    /*
    Sets the number of frames per second. Default value = 60.

    Params:
    limit - number of frames - must be >= 0 (0 - unlimit, everything is in the hands of your machine).
    */
    void SetLimit(long limit) noexcept;

    //  Return the time of the previous frame in setted timeasure setted with SetTimeMeasure(). Default is ZC_FPS_TM__Nanoseconds.
    float GetPreviousFrameTime() noexcept;

    //  Return the time of the previous frame in the specified time measure.
    float GetPreviousFrameTime(ZC_FPS_TimeMeasure time_measure) noexcept;

    //  Return setted time measure in fps system. Default is ZC_FPS_TM__Nanoseconds.
    ZC_FPS_TimeMeasure GetTimeMeasure() noexcept;
    
    //  Set time measure of fps. Default ZC_FPS_TM__Nanoseconds. Method ZC_SWindow::GetPreviousFrameTime() will return time in that measure. And collbacs from ZC_Event will gave time in that measure.
    void SetTimeMeasure(ZC_FPS_TimeMeasure timeMeasure);

    //  Displays or not the number of frames per second.
    void NeedDraw(bool needDraw);
    
    //  Returns true if the number of frames per second is displayed, false otherwise.
    bool IsDrawing();

    //  Returns number of current frame.
    unsigned long long GetCurrentFrameNumber();
} // ZC__FPS


namespace ZC__Updater
{
    //  Stop or start updater activity. Default is on.
    void ChangeState(bool needUpdate);

    //  Change activity of the concrete level of the updater.
    void ChangeLevelState(size_t lvl, bool is_active);

    /*
    Connect to update in each frame. Update is calling after polling events and before collsion.

    Params:
    - func - function to call on update. Parameter (float) is time of previous frame.
    - level - level on wich add function. Order of calling on update.

    Return:
    Event connection for disconnect.
    */
    ZC_EC Connect(ZC_Function<void(float)>&& func, size_t level);
} // ZC__Updater


namespace ZC__Collision
{
    //  Stop or start collision activity. Default is on.
    void ChangeState(bool nake_collision);
    
    //  Return true if collision are active, otherwise false.
    bool GetState();
} // ZC__Collision