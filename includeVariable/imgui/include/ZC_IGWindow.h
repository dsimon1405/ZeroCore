#pragma once

#include <ZC/Tools/ZC_string.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RenderSet.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Tools/ZC_WindowOrthoIndent.h>

#include <forward_list>

namespace ZC_ImGui { bool Init(void* pWindow, void* pGlContext); }
class ZC_Renderer;

class ZC_IGWindow : protected ZC_WindowOrthoIndent
{
    friend bool ZC_ImGui::Init(void* pWindow, void* pGlContext);
    friend class ZC_Renderer;
public:
    ~ZC_IGWindow();

    /*
    Allows to draw window or not.

    Params:
    _needDraw - if true window will draw until user don't close it (using X on TitleBar if it exists), or calls this function with false.
    */
    void NeedDraw(bool _needDraw);
    //  Sets to draw or not all ImGui windows.
    static void NeedImGuiDraw(bool _needDraw) noexcept;
    /*
    Returns true if the cursor is in one of the ImGui windows.
    Only relevant after the start of polling events and before calling the end of polling events signal (sigHandleEventsEnd in ZC_SDL_EventsHolder.cpp).
    */
    static bool IsCursorInOneOfWindows() noexcept;

protected:
    /*
    Create ImGui window for heir.

    Params:
    unicName - unic name for window.
    needDraw - window must be draw after creation.
    _width - window width.
    _height - window height.
    _indentX - value of horizontal indent from border of global window. If used IndentFlag:
        X_Left_Pixel, X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        X_Left_Percent, X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        X_Center -> value no metter.
    _indentX - value of vertival indent from border of global window. If used IndentFlag: 
        Y_Top_Pixel, Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        Y_Top_Percent, Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        Y_Center -> value no metter.
    _indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    _mayClose - window must have cross to close the window.
    _igfw - ImGuiWindowFlags.
    */
    ZC_IGWindow(std::string&& unicName, bool needDraw, float _width, float _height,
        float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags, bool _mayClose, int _igwf);

private:
    static inline std::forward_list<std::string> unicNames;
    static inline std::forward_list<ZC_IGWindow*> rendererWindows;  //  heirs for call in ZC_Renderer into ZC_IGWindow::Draw();
    static inline bool needDraw = true;     //  for all ImGui

    const char* name;
    bool isDrawing;

    //  used for safely remove from ZC_Renderer in handle events end signal (caurse NeedDraw function may calls from ZC_Renderer::DrawAll() and change ZC_Renderer state!)
    ZC_SConnection sconChangeDrawingState;
    bool mayClose;
    int igwf;
    bool needSetPosition = true;
    static inline bool isCursorInOneOfWindows = false;

    void UpdateAndDraw();
    void VCallAfterZC_WindowResized() override;

    virtual void DrawWindow() = 0;

    const char* AddName(std::string&& unicName);
    //  set position of next window calls before ImGui::Begin();
    void SetPosition();
    void UpadteRendererState(float time);

    //  refresh for next frame in events handle events end signal
    static void Make_isCursorInOneOfWindows_false(float time) noexcept;
    //  function for call into ZC_Renderer::Draw();
    static void Draw();
    static void NeedDrawImGui(bool _isNeedDraw) noexcept;
};