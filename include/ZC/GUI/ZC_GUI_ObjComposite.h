// #pragma once

// #include <ZC/GUI/ZC_GUI_Obj.h>
// #include <ZC/GUI/ZC_GUI_Window.h>

// #include <list>

// struct ZC_GUI_ObjComposite : public ZC_GUI_Obj
// {
//     std::list<ZC_GUI_Obj*> objs;

//     ZC_GUI_ObjComposite() = default;

//     ZC_GUI_ObjComposite(const ZC_GUI_ObjData& _objData);

//     virtual bool VSetWindow_Obj(ZC_GUI_Window* pWindow);

//     void VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount) override;
//     void VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs,
//         std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex) override;
//     bool VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pPrevObj) override;
//     void VEraseObj_Obj(ZC_GUI_Obj* pObj) override;
//     bool VSetObjHolder_Obj(ZC_GUI_Obj* _pObjHolder) override;
// };
