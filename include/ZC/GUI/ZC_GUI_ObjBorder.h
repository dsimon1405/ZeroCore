#pragma once

#include <ZC/GUI/ZC_GUI_ObjComposite.h>

struct ZC_GUI_ObjBorder : public ZC_GUI_ObjComposite
{
    ZC_GUI_Border* pBorder;

    ZC_GUI_ObjBorder(const ZC_GUI_ObjData& _objData);

    ~ZC_GUI_ObjBorder();
    
    void VSetPosition_Obj(const ZC_Vec2<float>& pos) override;

    void VGetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount) override;
    void VGetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex) override;
    void VRecalculateBorder_Obj() override;
};
