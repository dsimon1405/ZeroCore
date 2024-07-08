#include <ZC/GUI/ZC_GUI_Obj.h>

#include <ZC/Video/ZC_SWindow.h>
#include <ZC/GUI/ZC_GUI.h>

#include <cassert>

ZC_GUI_Obj::ZC_GUI_Obj(const ZC_GUI_ObjData& _objData)
    : pBL(new ZC_Vec2<float>()),
    pObjData(new ZC_GUI_ObjData(_objData))
{}

ZC_GUI_Obj::~ZC_GUI_Obj()
{
    if (isFirstGetDataCall)
    {
        if(pBL) delete pBL;
        if(pObjData) delete pObjData;
    }
}

ZC_Vec2<float> ZC_GUI_Obj::GetPosition_bl_Obj()
{
    return *pBL;
}

float ZC_GUI_Obj::GetWidth()
{
    return pObjData->width;
}

float ZC_GUI_Obj::GetHeight()
{
    return pObjData->height;
}

void ZC_GUI_Obj::SetObjHolder(ZC_GUI_Obj* _pObjHolder)
{
    pObjHolder = _pObjHolder;
}

bool ZC_GUI_Obj::IsWindowDrawing_Obj() const noexcept
{
    return pObjHolder ? pObjHolder->IsWindowDrawing_Obj() : VIsDrawing_Obj();
};

ZC_GUI_Obj* ZC_GUI_Obj::GetWindow()
{
    return pObjHolder ? pObjHolder->GetWindow() : this;
}

bool ZC_GUI_Obj::IsWindowFocused()
{
    return pObjHolder ? pObjHolder->IsWindowFocused() : ZC_GUI::pGUI->eventManager.IsWindowFocused(this);
}

void ZC_GUI_Obj::MakeWindowFocused()
{
    if (IsWindowFocused()) return;
    if (pObjHolder) pObjHolder->MakeWindowFocused();
    else
    {
        if (!VIsDrawing_Obj()) return;
        ZC_GUI::UpdateWindowDrawState(this);
    }
}

bool ZC_GUI_Obj::VIsStacionar_Obj() const noexcept
{
    return pObjHolder->VIsStacionar_Obj();
}

const ZC_GUI_Border& ZC_GUI_Obj::VGetBorder_Obj()
{
    return pObjHolder->VGetBorder_Obj();
}

void ZC_GUI_Obj::VConf_Set_bl_Obj(const ZC_Vec2<float>& _bl)
{
    *pBL = _bl;
}

void ZC_GUI_Obj::VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount)
{
    ++rObjsCount;
}

void ZC_GUI_Obj::VConfigure_Obj()
{
    if (pObjHolder) pObjHolder->VConfigure_Obj();
}

bool ZC_GUI_Obj::VIsConfigured_Obj() const noexcept
{
    return pObjHolder->VIsConfigured_Obj();
}

void ZC_GUI_Obj::VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex,
        std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs)
{
    pObjData->borderIndex = borderIndex;
    ZC_Vec2<float>* pBL_temp = &rBLs.emplace_back(*pBL);
    VConf_SetTextUV();  //  update uv, uses text heirs 
    ZC_GUI_ObjData* pObjData_temp = &rObjDatas.emplace_back(*pObjData);

    if (this->isFirstGetDataCall)
    {
        delete pBL;
        delete pObjData;
        isFirstGetDataCall = false;
    }
    pBL = pBL_temp;
    pObjData = pObjData_temp;

    if (VIsButtonKeyboard_Obj()) rButtonKeyboard_objs.emplace_front(this);
}

void ZC_GUI_Obj::VMapObjData_Obj(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData)
{
    if (pObjHolder) return pObjHolder->VMapObjData_Obj(pObjData, offsetIn_objData, byteSize, pData);
}

void ZC_GUI_Obj::VSubDataBL_Obj(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end)
{
    if (pObjHolder) return pObjHolder->VSubDataBL_Obj(pBL_start, pBL_end);
}

bool ZC_GUI_Obj::MakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    if (!VCheckCursorCollision_Obj(x, y)) return false;
    rpObj = this;
    if (VIsUseScrollEvent_Obj()) rpScroll = this;
    return true;
}

bool ZC_GUI_Obj::CheckCursorCollision_Obj()
{
    float x = 0.f;
    float y = 0.f;
    ZC_SWindow::GetCursorPosition(x, y);
    return VCheckCursorCollision_Obj(x, y);
}

bool ZC_GUI_Obj::Collision(float x, float y, float bl_x, float bl_y, float tr_x, float tr_y)
{
    return bl_x <= x && bl_y <= y && x <= tr_x && y <= tr_y;
}

bool ZC_GUI_Obj::VCheckCursorCollision_Obj(float x, float y)
{
    return VIsDrawing_Obj() && this->Collision(x, y, (*pBL)[0], (*pBL)[1], (*pBL)[0] + pObjData->width, (*pBL)[1] + pObjData->height);
}

void ZC_GUI_Obj::ButtonDown(ZC_ButtonID buttonID, float time)
{
    if (!(ZC_GUI::pGUI->eventManager.IsActiveEventManager()) || !IsWindowDrawing_Obj() || !VIsDrawing_Obj()) return;   //  window or object don't drawing, event coud be used not gui
    switch (buttonID)
    {
    case ZC_ButtonID::M_LEFT:
    {
        if (!VMouseButtonLeftDown_Obj(time)) return;
        if (VIsUseCursorMoveEventOnMBLetfDown_Obj()) ZC_GUI::pGUI->eventManager.SetCursorMoveObj(this);     //  left button down objects may be added for mouse cursor move event
        MakeWindowFocused();    //  if LMB is pressed, regardless of the object or window, the window should focus if it can
    } break;
    case ZC_ButtonID::M_RIGHT:
    {
        if (!VRightButtonDown_Obj(time)) return;
        MakeWindowFocused();    //  if RMB is pressed, regardless of the object or window, the window should focus if it can
    } break;
    default: if (!VKeyboardButtonDown_Obj(time)) return;
        break;
    }
    ZC_GUI::pGUI->eventManager.SetPressedObj(this);
}

void ZC_GUI_Obj::ButtonUp(ZC_ButtonID buttonID, float time)
{
    switch (buttonID)
    {
    case ZC_ButtonID::M_LEFT:
    {
        VMouseButtonLeftUp_Obj(time);
        ZC_GUI::pGUI->eventManager.MouseButtonUp();
    } break;
    case ZC_ButtonID::M_RIGHT: VRightButtonUp_Obj(time); break;
    default: VKeyboardButtonUp_Obj(time); break;
    }
    ZC_GUI::pGUI->eventManager.SetPressedObj(nullptr);
}