#include "ZC_GUI_ObjComposite.h"

#include <algorithm>

ZC_GUI_ObjComposite::ZC_GUI_ObjComposite(const ZC_GUI_ObjData& _objData)
    : ZC_GUI_Obj(_objData)
{}

ZC_GUI_ObjComposite::ZC_GUI_ObjComposite(ZC_GUI_ObjComposite&& oc)
    : ZC_GUI_Obj(static_cast<ZC_GUI_Obj&&>(oc))
{       //  obj can be added in new object with VAddObj_Obj() before calling dstr of current object, so to avoid damaging pointer on objHolder in dstr, make it all nullptr here and clear objs
    for (ZC_GUI_Obj* pObj : oc.objs)
        pObj->SetObjHolder(nullptr);
    oc.objs.clear();
}

ZC_GUI_ObjComposite::~ZC_GUI_ObjComposite()
{
    for (ZC_GUI_Obj* pObj : objs)
        pObj->SetObjHolder(nullptr);
    if (this->pObjHolder) pObjHolder->VEraseObj_Obj(this);  //  erase itself from holder
}

ZC_Vec2<float>* ZC_GUI_ObjComposite::VGet_pBL_end()
{
    return objs.empty() ? Get_pBL_start() : objs.back()->VGet_pBL_end();
}

ZC_GUI_ObjData* ZC_GUI_ObjComposite::VGet_pObjData_end()
{
    return objs.empty() ? Get_pObjData_start() : objs.back()->VGet_pObjData_end();
}

ZC_GUI_Obj* ZC_GUI_ObjComposite::VGet_pObj_end()
{
    return objs.empty() ? this : objs.back()->VGet_pObj_end();
}

float ZC_GUI_ObjComposite::VGetTop_Obj()
{
    float total_top = (*pBL)[1] + GetHeight();
    for (ZC_GUI_Obj* pObj : objs)
    {
        float top = pObj->VGetTop_Obj();
        if (top > total_top) total_top = top;
    }
    return total_top;
}

float ZC_GUI_ObjComposite::VGetBottom_Obj()
{
    float total_bottom = (*pBL)[1];
    for (ZC_GUI_Obj* pObj : objs)
    {
        float bottom = pObj->VGetBottom_Obj();
        if (bottom < total_bottom) total_bottom = bottom;
    }
    return total_bottom;
}

bool ZC_GUI_ObjComposite::VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pPrevObj)
{
    if (pPrevObj)
    {
        auto iter = std::find(objs.begin(), objs.end(), pPrevObj);
        if (iter != objs.end()) objs.emplace(++iter, pObj);
        else return false;
    }
    else objs.emplace_back(pObj);
    pObj->SetObjHolder(this);
    if (VIsConfigured_Obj()) VConfigure_Obj();
    return true;
}

void ZC_GUI_ObjComposite::VEraseObj_Obj(ZC_GUI_Obj* pObj)
{
    auto iter = std::find(objs.begin(), objs.end(), pObj);
    if (iter != objs.end())
    {
        objs.erase(iter);
        VEraseFrom__buttonKeyboard_objs_Obj(pObj);
    }
}

void ZC_GUI_ObjComposite::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    *pBL = _bl;
    for (ZC_GUI_Obj* pObj : objs) pObj->VSet_pBL_Obj(_bl);
}

void ZC_GUI_ObjComposite::VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount)
{
    ++rObjsCount;
    for (ZC_GUI_Obj* pObj : objs) pObj->VConf_GetBordersAndObjsCount_Obj(rBordersCount, rObjsCount);
}

void ZC_GUI_ObjComposite::VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex,
    std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs)
{
    this->Conf_GetData_Obj(rBorder, rBLs, rObjDatas, borderIndex, rButtonKeyboard_objs);
    for (ZC_GUI_Obj* pObj : objs) pObj->VConf_GetData_Obj(rBorder, rBLs, rObjDatas, borderIndex, rButtonKeyboard_objs);
}

void ZC_GUI_ObjComposite::VConf_SetTextUV_Obj()
{
    for (ZC_GUI_Obj* pObj : objs) pObj->VConf_SetTextUV_Obj();
}

void ZC_GUI_ObjComposite::VSetDrawState_Obj(bool neeDraw, bool updateGPU)
{
    if (neeDraw == VIsDrawing_Obj()) return;
    pObjData->height = neeDraw ? actual_height : 0.f;
    for (ZC_GUI_Obj* pObj : objs) pObj->VSetDrawState_Obj(neeDraw, false);
    if (updateGPU) VSubDataObjData_Obj(this->pObjData, objs.empty() ? this->pObjData : objs.back()->pObjData);
}

void ZC_GUI_ObjComposite::VMoveBL_Obj(float rel_x, float rel_y, int& update_borders)
{
    MoveVec2(rel_x, rel_y, *(this->pBL));
    for (ZC_GUI_Obj* pObj : objs) pObj->VMoveBL_Obj(rel_x, rel_y, update_borders);
}