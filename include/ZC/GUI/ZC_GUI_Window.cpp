#include <ZC/GUI/ZC_GUI_Window.h>

#include <ZC/GUI/ZC_GUI.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>
#include "ZC_GUI_IconUV.h"

#include <cassert>

bool ZC_GUI_Window::VIsStacionar_Obj() const noexcept
{
    return winFlags & ZC_GUI_WF__Stacionar;
}

bool ZC_GUI_Window::IsBackground() const noexcept
{
    return !(winFlags & ZC_GUI_WF__NoBackground);
}

bool ZC_GUI_Window::IsMovable() const noexcept
{
    return IsBackground() && winFlags & ZC_GUI_WF__Movable;
}

void ZC_GUI_Window::MakeForcused()
{
    if (!VIsDrawing_Obj()) return;
    ZC_GUI::UpdateWindowDrawState(this);
}

ZC_GUI_Window::ZC_GUI_Window(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags)
    : ZC_WindowOrthoIndent1(false, _winFlags & ZC_GUI_WF__Movable ?
        ZC_WOIData{.width = _woiData.width, .height = _woiData.height, .indentX = 0.f, .indentY = 0.f, .indentFlags = ZC_WOIF__X_Center | ZC_WOIF__Y_Center } : _woiData),
    ZC_GUI_ObjBorder(ZC_GUI_ObjData{ .width = _woiData.width, .height = _woiData.height, .depth = _winFlags & ZC_GUI_WF__Stacionar ? GetStacionarDepth() : 0.f,
        .uv = ZC_GUI_IconUV::window }, _winFlags & ZC_GUI_WF__Scrollable),
    winFlags(_winFlags)
{
    *(this->pBL) = this->bl_WOI;
       //  if movable window drawing (on start in that case), center position allready calculated in ZC_WindowOrthoIndent, set indent flags bl (unbind movable window from ZC_SWindow resize event in ZC_WindowOrthoIndent)
    if (winFlags & ZC_GUI_WF__Movable && winFlags & ZC_GUI_WF__NeedDraw)
        SetNewIndentParams((*pBL)[0], (*pBL)[1], ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel);
}

ZC_GUI_Window::ZC_GUI_Window(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags)
    : ZC_WindowOrthoIndent1(false, _winFlags & ZC_GUI_WF__Movable ?
        ZC_WOIData{.width = _woiData.width, .height = _woiData.height, .indentX = 0.f, .indentY = 0.f, .indentFlags = ZC_WOIF__X_Center | ZC_WOIF__Y_Center } : _woiData),
    ZC_GUI_ObjBorder(ZC_GUI_ObjData{ .width = _woiData.width, .height = _woiData.height, .depth = _winFlags & ZC_GUI_WF__Stacionar ? GetStacionarDepth() : 0.f,
        .uv = uv }, _winFlags & ZC_GUI_WF__Scrollable),
    winFlags(_winFlags)
{
    *(this->pBL) = this->bl_WOI;
       //  if movable window drawing (on start in that case), center position allready calculated in ZC_WindowOrthoIndent, set indent flags bl (unbind movable window from ZC_SWindow resize event in ZC_WindowOrthoIndent)
    if (winFlags & ZC_GUI_WF__Movable && winFlags & ZC_GUI_WF__NeedDraw)
        SetNewIndentParams((*pBL)[0], (*pBL)[1], ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel);
}

void ZC_GUI_Window::VChanged_bl_WOI()
{
    if (this->bl_WOI == *pBL) return;     //  no changes in bl
    ZC_Vec2<float> rel_bl = this->bl_WOI - *pBL;
    VCursoreMove_EO(rel_bl[0], rel_bl[1]);
}

float ZC_GUI_Window::GetStacionarDepth()
{
    static float stacionarDepth = ZC_GUI_Depth::win_stacionarStart;
    stacionarDepth -= ZC_GUI_Depth::win_step;
    return stacionarDepth;
}

void ZC_GUI_Window::SetFocuseDepth()
{
    static ZC_GUI_Window* pChangingWindow = nullptr;
    static float focuseDepth = ZC_GUI_Depth::win_openableStart;

    if (VIsStacionar_Obj() || pObjData->depth == focuseDepth || this == pChangingWindow) return;

    if (focuseDepth - ZC_GUI_Depth::win_step < ZC_GUI_Depth::win_openableEnd)  //  start take depth values on positive ZC_GUI_Depth::windowStacionar, end on negative (reset)
    {
        pChangingWindow = this;
        focuseDepth = ZC_GUI_Depth::win_openableStart;

        //  call ZC_GUI_Collision::ResetDepth() - in wich calls all non stacionar windows with SetFocuseDepth()

        pChangingWindow = nullptr;
    }
    focuseDepth -= ZC_GUI_Depth::win_step;
    pObjData->depth = focuseDepth;
    if (VIsConfigured_Obj())
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, depth), sizeof(ZC_GUI_ObjData::depth), &(pObjData->depth));
}
// bool ZC_GUI_Window::VIsMovable_EO() override { return false; }
// void ZC_GUI_Window::VCursorCollisionStart_EO(float time) override {}
// void ZC_GUI_Window::VCursorCollisionEnd_EO(float time) override {}
// void ZC_GUI_Window::VCursoreMove_EO(float x, float y, float rel_x, float rel_y, float time) override {}
// void ZC_GUI_Window::VLeftButtonDown_EO(float time) override {}
// void ZC_GUI_Window::VLeftButtonUp_EO(float time) override {}
// void ZC_GUI_Window::VRightButtonDown_EO(float time) override {}
// void ZC_GUI_Window::VRightButtonUp_EO(float time) override {}
// void ZC_GUI_Window::VScroll_EO(float vertical, float time) override {}
