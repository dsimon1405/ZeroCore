#include <ZC/GUI/ZC_GUI_Obj.h>

#include <ZC/GUI/ZC_GUI_Window.h>

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

void ZC_GUI_Obj::VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex)
{
    pObjData->borderIndex = borderIndex;
    ZC_Vec2<float>* pBL_temp = &rBLs.emplace_back(*pBL);
    ZC_GUI_ObjData* pObjData_temp = &rObjDatas.emplace_back(*pObjData);

    if (this->isFirstGetDataCall)
    {
        delete pBL;
        delete pObjData;
        isFirstGetDataCall = false;
    }

    pBL = pBL_temp;
    pObjData = pObjData_temp;
}

void ZC_GUI_Obj::VMapObjData_Obj(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData)
{
    if (pObjHolder) return pObjHolder->VMapObjData_Obj(pObjData, offsetIn_objData, byteSize, pData);
}

void ZC_GUI_Obj::VSubDataBL_Obj(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end)
{
    if (pObjHolder) return pObjHolder->VSubDataBL_Obj(pBL_start, pBL_end);
}

bool ZC_GUI_Obj::MakeCursorCollision_Obj(float x, float y, ZC_GUI_EventObj*& rpWindow, ZC_GUI_EventObj*& rpObj, ZC_GUI_EventObj*& rpScroll)
{
    if (!VCheckCursorCollision_EO(x, y)) return false;
    rpObj = this;
    if (VIsUseScrollEvent_Obj()) rpScroll = this;
    return true;
}

bool ZC_GUI_Obj::VCheckCursorCollision_EO(float x, float y)
{
    return VIsDrawing_Obj() && this->Collision(x, y, (*pBL)[0], (*pBL)[1], (*pBL)[0] + pObjData->width, (*pBL)[1] + pObjData->height);
}