#include <ZC/GUI/ZC_GUI_Window.h>

#include <ZC/GUI/ZC_GUI.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>

#include <cassert>

bool ZC_GUI_Window::IsStacionar() const noexcept
{
    return winFlags & ZC_GUI_WF__Stacionar;
}

bool ZC_GUI_Window::IsBackground() const noexcept
{
    return !(winFlags & ZC_GUI_WF__NoBackground);
}

bool ZC_GUI_Window::IsMovable() const noexcept
{
    return winFlags & ZC_GUI_WF__Movable;
}

void ZC_GUI_Window::AddRow(ZC_GUI_RowParams rowParams)
{
    rowParams.indent_x += (*pBL)[0];    //  make indent_x a start position X value, in SDL window
    if (!rows.empty())  //   not first row, calculate indent_y from previous row
    {
        ZC_GUI_RowParams& rRowParams_last = rows.back().rowParams;
        rowParams.indent_y = rRowParams_last.indent_y - rRowParams_last.height - rowParams.indent_y;    //  make indent_y a row's top left position Y value, in SDL window
    }
    else rowParams.indent_y = (*pBL)[1] + pObjData->height - rowParams.indent_y;  //  first row, calculate indent_y from window top border
    rows.emplace_back(ZC_GUI_WinRow{ rowParams, {} });
}

bool ZC_GUI_Window::AddObj(ZC_GUI_Obj* pObj)
{
    if (VIsConfigured_W())
    {
        assert(false);  //  add obj after creation of buffers
        return false;
    }

    if (!pObj->VSetWindow_Obj(this)) return false;  //  can't set pWinHolder
    if (rows.empty()) AddRow(ZC_GUI_RowParams{});
    
    ZC_GUI_WinRow& rWinRow = rows.back();
    ZC_GUI_RowParams& rRowParams = rWinRow.rowParams;
    if (rRowParams.height < pObj->GetHeight()) rRowParams.height = pObj->GetHeight();   //  if the new object is above the row, update the row height

    if (rWinRow.objs.empty())   //  first object in the row
    {
        pObj->VSetPosition_Obj({ rRowParams.indent_x,   //  indent_x is start position of row in SDL window (look ZC_GUI_Window::AddRow())
            rRowParams.indent_y - pObj->GetHeight() });   //  indent_y is top left Y position in SDL window (look ZC_GUI_Window::AddRow())
    }
    else    //  not first obj in the line
    {
        ZC_GUI_Obj* pObj_lastInLine = rWinRow.objs.back();
        pObj->VSetPosition_Obj({ (*(pObj_lastInLine->pBL))[0] + pObj_lastInLine->GetWidth() + rRowParams.distance_x,  //  calculate bl_x from previous obj in the line
            rRowParams.indent_y - pObj->GetHeight() });   //  indent_y is top left Y position in SDL window (look ZC_GUI_Window::AddRow())
    }
    rWinRow.objs.emplace_back(pObj);
    return true;
}

void ZC_GUI_Window::MakeForcused()
{
    if (!VIsDrawing_W()) return;
    ZC_GUI::MakeWindowFocused(this);
}

void ZC_GUI_Window::SetFocuseDepth()
{
    winDepth.SetNextDepth(this);
}

ZC_GUI_Border ZC_GUI_Window::GetBorder() const noexcept
{
    return *pBorder;
}

ZC_GUI_Window::ZC_GUI_Window(const ZC_WOIData& woiData, const ZC_UV& uv, ZC_GUI_WinFlags _winFlags)
    : ZC_WindowOrthoIndent1(false, woiData),
    winFlags(_winFlags),
    pBorder{ new ZC_GUI_Border{ .bl = this->position, .tr = { this->position[0] + this->woiData.width, this->position[1] + this->woiData.height } } },
    pBL( new ZC_Vec2<float>(pBorder->bl)),
    pObjData{ new ZC_GUI_ObjData{ .width = this->woiData.width, .height = this->woiData.height,
        .depth = winFlags & ZC_GUI_WF__Stacionar ? ZC_GUI_Depth::windowStacionar : 0.f, .uv = uv } }
{}





bool ZC_GUI_Window::VMakeCursorCollision_EO(float x, float y, ZC_GUI_Window*& rpWindow, ZC_GUI_EventObj*& rpObj)
{
    assert(!rpWindow || !rpObj);    //  window or obj allready seted
    if (!Collision(x, y, (*this->pBL)[0], (*this->pBL)[1], (*this->pBL)[0] + this->pObjData->width, (*this->pBL)[1] + this->pObjData->height)) return false;
        
        //  window collisioned
    for (ZC_GUI_WinRow& row : rows)
    {
        for (ZC_GUI_Obj* pObj : row.objs)
        {
            if (pObj->VMakeCursorCollision_EO(x, y, rpWindow, rpObj))   //  found object, set window and out
            {
                rpWindow = this;
                return true;
            }
        }
    }
    if (!IsBackground()) return false;    //  there's no object under cursor in window, check is windowdrawable, if not, don't set window to ref pointer, just out
    rpWindow = this;    //  only the window is in collision
    return true;
}

bool ZC_GUI_Window::VCheckCursorCollision_EO(float x, float y)
{   //  retrurn true only if background is drawing and had collision 
    return IsBackground() && Collision(x, y, (*this->pBL)[0], (*this->pBL)[1], (*this->pBL)[0] + this->pObjData->width, (*this->pBL)[1] + this->pObjData->height);
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



//  WinDepth

void ZC_GUI_Window::WinDepth::SetNextDepth(ZC_GUI_Window* pWindow)
{
    static ZC_GUI_Window* pChangingWindow = nullptr;

    if (pWindow->IsStacionar()) return;
    if (pWindow->pObjData->depth == activeDepth) return;
    if (pWindow == pChangingWindow) return;

    if (activeDepth - ZC_GUI_Depth::windowStep < - ZC_GUI_Depth::windowStacionar)  //  start take depth values on positive ZC_GUI_Depth::windowStacionar, end on negative (reset)
    {
        pChangingWindow = pWindow;
        activeDepth = ZC_GUI_Depth::windowStacionar;

        //  call ZC_GUI_Collision::ResetDepth() - in wich calls all non stacionar windows with SetFocuseDepth()

        pChangingWindow = nullptr;
    }
    activeDepth -= ZC_GUI_Depth::windowStep;
    pWindow->pObjData->depth = activeDepth;
    if (pWindow->VIsConfigured_W())
        pWindow->VMapObjData_W(pWindow->pObjData, offsetof(ZC_GUI_ObjData, depth), sizeof(ZC_GUI_ObjData::depth), &(pWindow->pObjData->depth));
}