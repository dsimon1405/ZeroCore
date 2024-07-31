#include <ZC/GUI/ZC_GUI_Obj.h>

#include <ZC/Video/ZC_SWindow.h>
#include <ZC/GUI/ZC_GUI.h>

#include <cassert>

ZC_GUI_Obj::ZC_GUI_Obj(const ZC_GUI_ObjData& _objData)
    : pBL(new ZC_Vec2<float>()),
    pObjData(new ZC_GUI_ObjData(_objData)),
    actual_height(_objData.height)
{}

ZC_GUI_Obj::ZC_GUI_Obj(float width, float height, float depth, unsigned int color, const ZC_GUI_UV& uv, int borderIndex, unsigned int tex_binding)
    : ZC_GUI_Obj(ZC_GUI_ObjData(width, height, depth, color, uv, borderIndex, tex_binding))
{}

ZC_GUI_Obj::ZC_GUI_Obj(ZC_GUI_Obj&& o)
    : pBL(o.pBL),
    pObjData(o.pObjData),
    actual_height(o.actual_height),
    isFirstGetDataCall(o.isFirstGetDataCall)
{
    o.pBL = nullptr;
    o.pObjData = nullptr;
    o.isFirstGetDataCall = false;
}

ZC_GUI_Obj::~ZC_GUI_Obj()
{
    if (isFirstGetDataCall)
    {
        if(pBL) delete pBL;
        if(pObjData) delete pObjData;
    }
    if (this->pObjHolder) pObjHolder->VEraseObj_Obj(this);  //  erase itself from holder
}

ZC_Vec2<float> ZC_GUI_Obj::Get_bl_Obj()
{
    return *pBL;
}

float ZC_GUI_Obj::VGetWidth_Obj()
{
    return pObjData->width;
}

float ZC_GUI_Obj::VGetWidthComposite_Obj()
{
    return pObjData->width;
}

void ZC_GUI_Obj::VSetWidth_Obj(float width)
{
    pObjData->width = width;
}

float ZC_GUI_Obj::GetHeight()
{
    return actual_height;   //  allways actual, pObjData->height may be 0 if obj not drawing
}

void ZC_GUI_Obj::SetHeight_Obj(float height)
{
    actual_height = height;
    if (!VIsConfigured_Obj() || VIsDrawing_Obj()) pObjData->height = height;
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

ZC_Vec2<float>* ZC_GUI_Obj::Get_pBL_start()
{
    return pBL;
}

ZC_Vec2<float>* ZC_GUI_Obj::VGet_pBL_end()
{
    return pBL;
}

ZC_GUI_ObjData* ZC_GUI_Obj::Get_pObjData_start()
{
    return pObjData;
}

ZC_GUI_ObjData* ZC_GUI_Obj::VGet_pObjData_end()
{
    return pObjData;
}

ZC_GUI_Obj* ZC_GUI_Obj::VGet_pObj_end()
{
    return this;
}

float ZC_GUI_Obj::VGetTop_Obj()
{
    return VIsDrawing_Obj() ? (*pBL)[1] + GetHeight() : 0.f;
}

float ZC_GUI_Obj::VGetBottom_Obj()
{
    return VIsDrawing_Obj() ? (*pBL)[1] : 0.f;
}

void ZC_GUI_Obj::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    *pBL = _bl;
}

bool ZC_GUI_Obj::VIsMutableWin_Obj() const noexcept
{
    return pObjHolder ? pObjHolder->VIsMutableWin_Obj() : true;
}

bool ZC_GUI_Obj::VIsStacionarWin_Obj() const noexcept
{
    return pObjHolder->VIsStacionarWin_Obj();
}

bool ZC_GUI_Obj::VIsDrawing_Obj() const noexcept
{
    return pObjData->height == actual_height;
}

const ZC_GUI_Border& ZC_GUI_Obj::VGetBorder_Obj()
{
    return pObjHolder->VGetBorder_Obj();
}

ZC_GUI_Obj* ZC_GUI_Obj::VGetObjBorder_Obj()
{
    return pObjHolder->VGetObjBorder_Obj();
}

void ZC_GUI_Obj::VConfigure_Obj()
{
    if (pObjHolder) pObjHolder->VConfigure_Obj();
}

bool ZC_GUI_Obj::VIsConfigured_Obj() const noexcept
{
    return pObjHolder ? pObjHolder->VIsConfigured_Obj() : false;
}

void ZC_GUI_Obj::VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount)
{
    ++rObjsCount;
}

void ZC_GUI_Obj::VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex,
        std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs)
{
    Conf_GetData_Obj(rBorder, rBLs, rObjDatas, borderIndex, rButtonKeyboard_objs);
}

void ZC_GUI_Obj::Conf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex,
        std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs)
{
    pObjData->borderIndex = borderIndex;
    ZC_Vec2<float>* pBL_temp = &rBLs.emplace_back(*pBL);
    VConf_SetTextUV_Obj();  //  update uv, uses text heirs 
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

// void ZC_GUI_Obj::ChangeObjsDrawState(bool needDraw, ZC_GUI_Obj* pObj_start, ZC_GUI_Obj* pObj_end)
// {
//     ZC_GUI_Obj* pObjBorder_start = pObj_start->VGetObjBorder_Obj();
//     if (pObjBorder_start != pObj_end->VGetObjBorder_Obj() || !VIsConfigured_Obj()) return; //  must be in one border, in configured window
//     bool unused = false;
//     pObjBorder_start->VChangeObjsDrawState_Obj(needDraw, pObj_start, pObj_end->VGet_pObj_end(), unused);
// }

// bool ZC_GUI_Obj::VChangeObjsDrawState_Obj(bool needDraw, ZC_GUI_Obj* pObj_start, ZC_GUI_Obj* pObj_end, bool& mustBeChanged)
// {
//     return ChangeObjsDrawState_Obj(needDraw, pObj_start, pObj_end, mustBeChanged);
// }

// bool ZC_GUI_Obj::ChangeObjsDrawState_Obj(bool needDraw, ZC_GUI_Obj* pObj_start, ZC_GUI_Obj* pObj_end, bool& mustBeChanged)
// {
//     if (!mustBeChanged && pObj_start == this) mustBeChanged = true;     //  if pObj_start not found yet, chek current obj
//     if (mustBeChanged) pObjData->height = needDraw ? actual_height : 0;     //  if pObj_start where found, make changes
//     return this != pObj_end;
// }

void ZC_GUI_Obj::VSetDrawState_Obj(bool neeDraw, bool updateGPU)
{
    if (neeDraw == VIsDrawing_Obj()) return;
    pObjData->height = neeDraw ? actual_height : 0.f;
    if (updateGPU) VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, height), sizeof(ZC_GUI_ObjData::height), &(this->pObjData->height));
}

void ZC_GUI_Obj::VMapObjData_Obj(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData)
{
    if (pObjHolder) pObjHolder->VMapObjData_Obj(pObjData, offsetIn_objData, byteSize, pData);
}

void ZC_GUI_Obj::VSubDataBL_Obj(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end)
{
    if (pObjHolder) pObjHolder->VSubDataBL_Obj(pBL_start, pBL_end);
}

void ZC_GUI_Obj::VSubDataBorder_Obj(ZC_GUI_Border* pBorder_start, ZC_GUI_Border* pBorder_end)
{
    if (pObjHolder) pObjHolder->VSubDataBorder_Obj(pBorder_start, pBorder_end);
}

void ZC_GUI_Obj::VSubDataObjData_Obj(ZC_GUI_ObjData* pObjData_start, ZC_GUI_ObjData* pObjData_end)
{
    if (pObjHolder) pObjHolder->VSubDataObjData_Obj(pObjData_start, pObjData_end);
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

void ZC_GUI_Obj::VButtonDown_Obj(ZC_ButtonID buttonID, float time)
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

void ZC_GUI_Obj::VButtonUp_Obj(ZC_ButtonID buttonID, float time)
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

void ZC_GUI_Obj::VEraseFrom__buttonKeyboard_objs_Obj(ZC_GUI_Obj* pDelete)
{
    if (pObjHolder) pObjHolder->VEraseFrom__buttonKeyboard_objs_Obj(pDelete);
}