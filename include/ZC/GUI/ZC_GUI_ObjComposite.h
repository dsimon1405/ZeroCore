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
            VIsConfigured_Obj() ? VConfigure_Obj() : VEraseFrom__buttonKeyboard_objs_Obj(pObj);
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

    bool VChangeObjsDrawState_Obj(bool needDraw, ZC_GUI_Obj* pObj_start, ZC_GUI_Obj* pObj_end, bool& mustBeChanged) override
    {
        if (!ChangeObjsDrawState_Obj(needDraw, pObj_start, pObj_end, mustBeChanged)) return false;
        for (ZC_GUI_Obj* pObj : objs) if (!pObj->VChangeObjsDrawState_Obj(needDraw, pObj_start, pObj_end, mustBeChanged)) return false;
        return true;
    }
};