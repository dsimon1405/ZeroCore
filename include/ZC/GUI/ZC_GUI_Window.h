#pragma once

#include <ZC/Tools/ZC_WindowOrthoIndent.h>
#include <ZC/GUI/ZC_GUI_ObjBorder.h>
#include <ZC/GUI/ZC_GUI_Depth.h>

#include <list>

typedef int ZC_GUI_WinFlags;
enum ZC_GUI_WinFlag
{
    ZC_GUI_WF__Stacionar        = 1,        //  The window is always under non-stacionar windows (if the window is in focus and not stationary windows overlap it, the stationary window is still drawn under those windows). Each new created stationary window will always display above the previous created stationary windows (avoid intersecting stationary windows, think of it as a desktop interface).
    ZC_GUI_WF__NeedDraw         = 1 << 1,   //  Need draw on start
    ZC_GUI_WF__NoBackground     = 1 << 2,   //  Don't draw background (window background don't make collisioin with mouse cursor, objects of the window still make collision)
    ZC_GUI_WF__Movable          = 1 << 3,   //  Window can be moved. ZC_WOIData will be ignored and on first draw, window will be in the ZC_SWindow center, then will be set flags ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel.
    ZC_GUI_WF__Scrollable       = 1 << 4,   //  Can be used scroll
};

struct ZC_GUI_Window : public ZC_WindowOrthoIndent1, public ZC_GUI_ObjBorder
{

    bool VIsStacionar_Obj() const noexcept override;

    bool IsBackground() const noexcept;
    bool IsMovable() const noexcept;
    void MakeForcused();

        //  returns true if vectors and GPU buffers allready created
    virtual bool VIsMutable_W() const noexcept = 0;
    virtual void VSetDrawState_W(bool needDraw) = 0;
    virtual void VDraw_W() = 0;

// protected:
    ZC_GUI_WinFlags winFlags;
    GLsizeiptr bordersCount = 0,    //  future size of vector (and buffer for that vectors) of borders of heir or static window
        objsCount = 0;          //  future size of vectors (and buffers for that vectors) of bls, depths of heir or static window

    ZC_GUI_Window(const ZC_WOIData& _woiData, ZC_GUI_WinFlags winFlags);
    ZC_GUI_Window(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags winFlags);


    void VChanged_bl_WOI() override;    //  callback ZC_WindowOrthoIndent recalculated bl

    float GetStacionarDepth();
    void SetFocuseDepth();




    
    void VCursorCollisionStart_EO(float time) override {}
    void VCursorCollisionEnd_EO(float time) override {}

    // void VCursoreMove_EO(float x, float y, float rel_x, float rel_y, float time) override {}
    bool VLeftButtonDown_EO(float time, bool& cursorMoveWhilePressed) override
    {
        cursorMoveWhilePressed = IsMovable();
        MakeForcused();
        return true;
    }
    void VLeftButtonUp_EO(float time) override {}
    bool VRightButtonDown_EO(float time, bool& cursorMoveWhilePressed) override { return true; }
    void VRightButtonUp_EO(float time) override {}
    void VScroll_EO(float vertical, float time) override {}
};
