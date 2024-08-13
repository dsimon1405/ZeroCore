#pragma once

#include <ZC/GUI/ZC_GUI_Obj.h>

#include <list>
#include <algorithm>

struct ZC_GUI_ObjComposite : public ZC_GUI_Obj
{
    std::list<ZC_GUI_Obj*> objs;

    ZC_GUI_ObjComposite(const ZC_GUI_ObjData& _objData)
        : ZC_GUI_Obj(_objData)
    {}

    ZC_GUI_ObjComposite(ZC_GUI_ObjComposite&& oc)
        : ZC_GUI_Obj(dynamic_cast<ZC_GUI_Obj&&>(oc)),
        objs(std::move(oc.objs))
    {}
    
    ~ZC_GUI_ObjComposite()
    {
        for (ZC_GUI_Obj* pObj : objs)
            pObj->SetObjHolder(nullptr);
        if (this->pObjHolder) pObjHolder->VEraseObj_Obj(this);  //  erase itself from holder
    }

    ZC_Vec2<float>* VGet_pBL_end() override
    {
        return objs.empty() ? Get_pBL_start() : objs.back()->VGet_pBL_end();
    }

    ZC_GUI_ObjData* VGet_pObjData_end() override
    {
        return objs.empty() ? Get_pObjData_start() : objs.back()->VGet_pObjData_end();
    }

    ZC_GUI_Obj* VGet_pObj_end() override
    {
        return objs.empty() ? this : objs.back()->VGet_pObj_end();
    }

    float VGetTop_Obj() override
    {
        float total_top = (*pBL)[1] + GetHeight();
        for (ZC_GUI_Obj* pObj : objs)
        {
            float top = pObj->VGetTop_Obj();
            if (top > total_top) total_top = top;
        }
        return total_top;
    }

    float VGetBottom_Obj() override
    {
        float total_bottom = (*pBL)[1];
        for (ZC_GUI_Obj* pObj : objs)
        {
            float bottom = pObj->VGetBottom_Obj();
            if (bottom < total_bottom) total_bottom = bottom;
        }
        return total_bottom;
    }

    bool VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pPrevObj) override
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

    void VEraseObj_Obj(ZC_GUI_Obj* pObj) override
    {
        auto iter = std::find(objs.begin(), objs.end(), pObj);
        if (iter != objs.end())
        {
            objs.erase(iter);
            VEraseFrom__buttonKeyboard_objs_Obj(pObj);
        }
    }

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
    {
        *pBL = _bl;
        for (ZC_GUI_Obj* pObj : objs) pObj->VSet_pBL_Obj(_bl);
    }

    void VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount) override
    {
        ++rObjsCount;
        for (ZC_GUI_Obj* pObj : objs) pObj->VConf_GetBordersAndObjsCount_Obj(rBordersCount, rObjsCount);
    }

    void VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex,
        std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs) override
    {
        this->Conf_GetData_Obj(rBorder, rBLs, rObjDatas, borderIndex, rButtonKeyboard_objs);
        for (ZC_GUI_Obj* pObj : objs) pObj->VConf_GetData_Obj(rBorder, rBLs, rObjDatas, borderIndex, rButtonKeyboard_objs);
    }

    void VConf_SetTextUV_Obj() override
    {
        for (ZC_GUI_Obj* pObj : objs) pObj->VConf_SetTextUV_Obj();
    }
    
    void VSetDrawState_Obj(bool neeDraw, bool updateGPU) override
    {
        if (neeDraw == VIsDrawing_Obj()) return;
        pObjData->height = neeDraw ? actual_height : 0.f;
        for (ZC_GUI_Obj* pObj : objs) pObj->VSetDrawState_Obj(neeDraw, false);
        if (updateGPU) VSubDataObjData_Obj(this->pObjData, objs.empty() ? this->pObjData : objs.back()->pObjData);
    }

    void VMoveBL_Obj(float rel_x, float rel_y, int& update_borders) override
    {
        MoveVec2(rel_x, rel_y, *(this->pBL));
        for (ZC_GUI_Obj* pObj : objs) pObj->VMoveBL_Obj(rel_x, rel_y, update_borders);
    }
};