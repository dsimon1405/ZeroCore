#pragma once

#include <ZC/GUI/Backend/ZC_GUI_Obj.h>

#include <list>

struct ZC_GUI_ObjComposite : public ZC_GUI_Obj
{
    std::list<ZC_GUI_Obj*> objs;

    ZC_GUI_ObjComposite(const ZC_GUI_ObjData& _objData);

    ZC_GUI_ObjComposite(ZC_GUI_ObjComposite&& oc);
    
    ~ZC_GUI_ObjComposite();

    ZC_Vec2<float>* VGet_pBL_end() override;
    ZC_GUI_ObjData* VGet_pObjData_end() override;
    ZC_GUI_Obj* VGet_pObj_end() override;
    float VGetTop_Obj() override;
    float VGetBottom_Obj() override;
    bool VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pPrevObj = nullptr) override;
    void VEraseObj_Obj(ZC_GUI_Obj* pObj) override;
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount) override;
    void VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex,
        std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs) override;
    void VConf_SetTextUV_Obj() override;
    void VSetDrawState_Obj(bool neeDraw, bool updateGPU) override;
    void VMoveBL_Obj(float rel_x, float rel_y, int& update_borders) override;
};