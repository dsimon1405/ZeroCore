#include <ZC/GUI/ZC_GUI_Window.h>

#include <ZC/GUI/ZC_GUI.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>

#include <cassert>

ZC_GUI_Window::ZC_GUI_Window(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags)
    : ZC_WindowOrthoIndent1(false, _winFlags & ZC_GUI_WF__Movable ?
        ZC_WOIData(_woiData.width, _woiData.height, 0.f, 0.f, ZC_WOIF__X_Center | ZC_WOIF__Y_Center)
        : _woiData),
    ZC_GUI_ObjBorder(ZC_GUI_ObjData(_woiData.width, _woiData.height, _winFlags & ZC_GUI_WF__Stacionar ? GetStacionarDepth() : 0.f, color_unfocused, uv, 0, ZC_GUI_Bindings::bind_tex_Icons),
        _winFlags & ZC_GUI_WF__Scrollable, _winFlags & ZC_GUI_WF__Frame),
    winFlags(_winFlags)
{
    *(this->pBL) = this->bl_WOI;
       //  if movable window drawing (on start in that case), center position allready calculated in ZC_WindowOrthoIndent, set indent flags bl (unbind movable window from ZC_SWindow resize event in ZC_WindowOrthoIndent)
    if (winFlags & ZC_GUI_WF__Movable && winFlags & ZC_GUI_WF__NeedDraw)
        SetNewIndentParams((*pBL)[0], (*pBL)[1], ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel);
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

void ZC_GUI_Window::MakeUnfocused()
{
    if (this->pObjData->color == color_unfocused) return;
    this->pObjData->color = color_unfocused;
    if (VIsConfigured_Obj())
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(pObjData->color));
}

void ZC_GUI_Window::VChanged_bl_WOI()
{
    if (this->bl_WOI == this->Get_bl_Obj()) return;     //  no changes in bl
    ZC_Vec2<float> rel_bl = this->bl_WOI - this->Get_bl_Obj();
    VCursorMove_Obj(rel_bl[0], rel_bl[1]);
}

void ZC_GUI_Window::VEraseFrom__buttonKeyboard_objs_Obj(ZC_GUI_Obj* pDelete)
{
    ZC_ForwardListErase(buttonKeyboard_objs, pDelete);
}

float ZC_GUI_Window::GetStacionarDepth()
{
    static float stacionarDepth = depth_stacionarStart;
    stacionarDepth -= depth_step;
    return stacionarDepth;
}

void ZC_GUI_Window::SetFocuseDepthAndColor()
{
    static ZC_GUI_Window* pChangingWindow = nullptr;    //  uses to reset depth for all openable window when reached depth end
    static float focuseDepth = depth_openableStart;

    if (VIsStacionarWin_Obj() || this == pChangingWindow) return;
    if (pObjData->depth == focuseDepth) 
    {
        if (!VIsDrawing_Obj()) pObjData->depth = 0.f; //  window stop drawing, need set depth not equal to focuseDepth (no sense make glmap now. If window will be opened again it will have new depth) 
        return;
    }

    if (focuseDepth - depth_step < depth_openableEnd)  //  start take depth values on positive ZC_GUI_Depth::windowStacionar, end on negative (reset)
    {
        pChangingWindow = this;
        focuseDepth = depth_openableStart;

        //  call ZC_GUI_Collision::ResetDepth() - in wich calls all non stacionar windows with SetFocuseDepth()

        pChangingWindow = nullptr;
    }
    focuseDepth -= depth_step;
    pObjData->depth = focuseDepth;
    pObjData->color = color_focused;
    if (VIsConfigured_Obj())    //  color goes after depth in struct so lecal change them both
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, depth), sizeof(ZC_GUI_ObjData::depth) + sizeof(ZC_GUI_ObjData::color), &(pObjData->depth));
}
