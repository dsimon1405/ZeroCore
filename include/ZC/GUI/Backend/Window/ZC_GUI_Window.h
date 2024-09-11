#pragma once

#include <ZC/GUI/Backend/ZC_GUI_ObjBorder.h>
#include <ZC/Tools/ZC_WindowOrthoIndent.h>
#include <ZC/Tools/Math/ZC_Math.h>

#include <list>

typedef int ZC_GUI_WinFlags;
enum ZC_GUI_WinFlag
{
    ZC_GUI_WF__None                 = 0,
    ZC_GUI_WF__Stacionar            = 1,        //  The window is always under non-stacionar windows (if the window is in focus and not stationary windows overlap it, the stationary window is still drawn under those windows). Each new created stationary window will always display above the previous created stationary windows (avoid intersecting stationary windows, think of it as a desktop interface).
    ZC_GUI_WF__NeedDraw             = 1 << 1,   //  Need draw on start.
    ZC_GUI_WF__NoBackground         = 1 << 2,   //  Don't draw background (window background don't make collisioin with mouse cursor, objects of the window still make collision).
    ZC_GUI_WF__Movable              = 1 << 3,   //  Window can be moved. ZC_WOIData will be ignored and on first draw, window will be in the ZC_SWindow center, then will be set flags ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel.
    ZC_GUI_WF__Scrollable           = 1 << 4,   //  Can be used scroll.
    ZC_GUI_WF__Frame                = 1 << 5,   //  Window have 2 pixels frame (border).
    ZC_GUI_WF__OutAreaClickClose    = 1 << 6    //  If mouse cursor click outside window area, widnow will close.
};

struct ZC_GUI_Window : public ZC_WindowOrthoIndent1, public ZC_GUI_ObjBorder
{
    static inline const float depth_step_win         = 0.000001f;       //  difference betwween widnows
    static inline const float depth_step_obj         = 0.0000001f;      //  difference berwween objects in the window (uses in ZCR_GUI_Orientation::UpdateAxisPositions())
    static inline const float depth_stacionarStart   = 0.9999999f;
    static inline const float depth_openableStart    = 0.9999f;
    static inline const float depth_openableEnd      = 0.f;

    struct ColorsWindow
    {
        uint color_window;
        uint color_window_focused;
        ColorsObjBorder colorsObjBorder;

        ColorsWindow(uint _color_window = ZC_GUI_Colors::window, uint _color_window_focused = ZC_GUI_Colors::window_focused, const ColorsObjBorder& _colorsObjBorder = 
            ColorsObjBorder(Scroll::ColorsScroll(ZC_GUI_Colors::window_scroll_background, ZC_GUI_ButtonBase::ColorsButton(ZC_GUI_Colors::window_scroll_caret, ZC_GUI_Colors::window_scroll_caret_under_cursor,
                ZC_GUI_Colors::window_scroll_caret_pressed)), ZC_GUI_Colors::window_frame));
    };
    const uint color_window;
    const uint color_window_focused;

    ZC_GUI_WinFlags winFlags;
    GLsizeiptr bordersCount = 0,    //  future size of vector (and buffer for that vectors) of borders of heir or static window
        objsCount = 0;          //  future size of vectors (and buffers for that vectors) of bls, depths of heir or static window
    std::forward_list<ZC_GUI_Obj*> buttonKeyboard_objs;     //  ZC_ButtonKeyboard(s) and it's heirs

    ZC_GUI_Window(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags winFlags, const ColorsWindow& colorsWindow);

    virtual void VSetDrawState_W(bool needDraw) = 0;
    void VSetDrawState_Obj(bool needDraw, bool updateGPU) override;

    bool VIsStacionarWin_Obj() const noexcept override;

    bool IsBackground() const noexcept;
        //  make window unfocused (calls in ZC_EventManager::UpdateWindowState())
    void MakeUnfocused();
    bool VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept override;

        //  find keyboard event (calls from ZC_GUI_EventManager::GetButtonDownObject()). Overrides in ZC_GUI_TextInputWindow.
    virtual ZC_GUI_Obj* VGetButtonKeyboard_W(ZC_ButtonID buttonId);
    virtual bool VIsMutableWin_Obj() const noexcept = 0;
    virtual void VDraw_W() = 0;
    virtual void VReconf_UpdateTextUV_W() {} //  updates uv in text objs (colling from ZC_GUI_DrawManager::Reconf_UpdateTextUV())
    virtual bool VIsInputWindow_W() const noexcept { return false; }

    void VChanged_bl_WOI() override;    //  callback ZC_WindowOrthoIndent recalculated bl

    void VEraseFrom__buttonKeyboard_objs_Obj(ZC_GUI_Obj* pDelete) override;
        //  close window with ZC_GUI_WF__OutAreaClickClose flag
    void MouseButtonLeftOrRightDown();

    float GetStacionarDepth();
    void SetFocuseDepthAndColor();
    void SetFocusDepth(bool updateGPU);
};

typedef ZC_GUI_Window::ColorsWindow ZC_GUI_ColorsWindow;