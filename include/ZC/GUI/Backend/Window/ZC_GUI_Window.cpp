#include "ZC_GUI_Window.h"

#include <ZC/GUI/Backend/System/ZC_GUI.h>
#include <ZC/GUI/Backend/ZC_GUI_ObjData.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>

ZC_GUI_Window::ZC_GUI_Window(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ColorsWindow& colorsWindow)
    : ZC_WindowOrthoIndent1(false, CheckWOIData(_winFlags, _woiData)),
    ZC_GUI_ObjBorder(ZC_GUI_ObjData(_woiData.width, _woiData.height, _winFlags & ZC_GUI_WF__Stacionar ? GetStacionarDepth() : 1.f, colorsWindow.color_window, uv, 0,
        ZC_GUI_Bindings::location_tex_Icons), _winFlags & ZC_GUI_WF__Scrollable, _winFlags & ZC_GUI_WF__Frame ? 2.f : 0.f, colorsWindow.colorsObjBorder),
    color_window(colorsWindow.color_window),
    color_window_focused(colorsWindow.color_window_focused),
    winFlags(_winFlags)
{
    *(this->pBL) = this->bl_WOI;
}

ZC_WOIData ZC_GUI_Window::CheckWOIData(ZC_GUI_WinFlags _winFlags, const ZC_WOIData& _woiData)
{
    if (_winFlags & ZC_GUI_WF__Movable)     //  movable windows supost to use only ZC_WOIF__X_Center_Pixel | ZC_WOIF__Y_Center_Pixel
    {
        int mask_x = _woiData.indentFlags & 63;     //  see ZC_WindowOrthoIndent1::CalculateIndents
        int mask_y = _woiData.indentFlags & 4032;     //  see ZC_WindowOrthoIndent1::CalculateIndents
        float indent_x = _woiData.indentX;
        float indent_y = _woiData.indentY;
        if (mask_x != ZC_WOIF__X_Center_Pixel)
        {
            assert(false);      //  not ZC_WOIF__X_Center_Pixel
            mask_x = ZC_WOIF__X_Center_Pixel;
            indent_x = 0.f;
        }
        if (mask_y != ZC_WOIF__Y_Center_Pixel)
        {
            assert(false);      //  not ZC_WOIF__Y_Center_Pixel
            mask_y = ZC_WOIF__Y_Center_Pixel;
            indent_y = 0.f;
        }
        return ZC_WOIData(_woiData.width, _woiData.height, indent_x, indent_y, mask_x | mask_y);
    }
    return _woiData;
}

void ZC_GUI_Window::VSetDrawState_Obj(bool needDraw, bool updateGPU)
{
    VSetDrawState_W(needDraw);
}

bool ZC_GUI_Window::VIsStacionarWin_Obj() const noexcept
{
    return winFlags & ZC_GUI_WF__Stacionar;
}

bool ZC_GUI_Window::VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept
{
    return IsBackground() && winFlags & ZC_GUI_WF__Movable;
}

ZC_GUI_Obj* ZC_GUI_Window::VGetButtonKeyboard_W(ZC_ButtonID buttonId)
{
    for (ZC_GUI_Obj* pObj : buttonKeyboard_objs)
        if (*pObj == buttonId) return pObj;
    
    return nullptr;
}

bool ZC_GUI_Window::IsBackground() const noexcept
{
    return !(winFlags & ZC_GUI_WF__NoBackground);
}

bool ZC_GUI_Window::IsEscapeClsoe() const noexcept
{
    return winFlags & ZC_GUI_WF__EscapeClose;
}

void ZC_GUI_Window::MakeUnfocused()
{
    if (this->pObjData->color == color_window) return;
    this->pObjData->color = color_window;
    if (VIsConfigured_Obj())
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(pObjData->color));
}

void ZC_GUI_Window::VChanged_bl_WOI()
{
    if (this->bl_WOI == this->Get_bl_Obj()) return;     //  no changes in bl
    if (winFlags & ZC_GUI_WF__OutAreaClickClose) VSetDrawState_W(false);    //  not shure that window have same woi that a window wich calls that window, so close window on resize
    ZC_Vec2<float> rel_bl = this->bl_WOI - this->Get_bl_Obj();
    VCursorMove_Obj(rel_bl[0], rel_bl[1]);
}

void ZC_GUI_Window::VEraseFrom__buttonKeyboard_objs_Obj(ZC_GUI_Obj* pDelete)
{
    ZC_ForwardListErase(buttonKeyboard_objs, pDelete);
}

void ZC_GUI_Window::MouseButtonLeftOrRightDown()
{
    if (!(this->CheckCursorCollision_Obj())) VSetDrawState_W(false);  //  cursor not in window stop it drawing
}

void ZC_GUI_Window::VCursorMove_Obj(float rel_x, float rel_y)
{
    VCursorMove_W(rel_x, rel_y);

    if (this->winFlags & ZC_GUI_WF__Movable)    //  movable window was moved, recalculate indents from the center of the system's window
        this->CalculateCenterPixelIndentsFromBL_WOI(*(this->pBL));
}

float ZC_GUI_Window::GetStacionarDepth()
{
    static float stacionarDepth = depth_stacionarStart;
    stacionarDepth -= depth_step_win;
    return stacionarDepth;
}

void ZC_GUI_Window::SetFocuseDepthAndColor()
{
    if (VIsStacionarWin_Obj()) return;

    SetFocusDepth(false);
    this->pObjData->color = color_window_focused;
    if (VIsConfigured_Obj())    //  color goes after depth in struct so lecal change them both
        VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, depth), sizeof(ZC_GUI_ObjData::depth) + sizeof(ZC_GUI_ObjData::color), &(this->pObjData->depth));
}

void ZC_GUI_Window::SetFocusDepth(bool updateGPU)
{
    static ZC_GUI_Window* pChangingWindow = nullptr;    //  uses to reset depth for all openable window when reached depth end
    static float focuseDepth = depth_openableStart;

    if (this == pChangingWindow) return;
    if (!VIsDrawing_Obj())
    {
        this->pObjData->depth = 1.f; //  window not drawing, need set depth not equal to focuseDepth (no sense make glmap now. If window will be opened again it will have new depth) 
        return;
    }

    if (focuseDepth - depth_step_win < depth_openableEnd)  //  start take depth values on positive ZC_GUI_Depth::windowStacionar, end on negative (reset)
    {
        pChangingWindow = this;
        focuseDepth = depth_openableStart;
        
        ZC_GUI::pGUI->eventManager.UpdateDepths();

        pChangingWindow = nullptr;
    }

    focuseDepth -= depth_step_win;
    this->pObjData->depth = focuseDepth;

    if (updateGPU) VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, depth), sizeof(ZC_GUI_ObjData::depth), &(this->pObjData->depth));
}


    //  ZC_GUI_Window::ColorsWindow

ZC_GUI_Window::ColorsWindow::ColorsWindow(uint _color_window, uint _color_window_focused, const ColorsObjBorder& _colorsObjBorder)
    : color_window(_color_window),
    color_window_focused(_color_window_focused),
    colorsObjBorder(_colorsObjBorder)
{}