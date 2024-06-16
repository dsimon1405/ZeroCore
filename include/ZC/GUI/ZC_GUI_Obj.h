#pragma once

#include <ZC/GUI/ZC_GUI_EventObj.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>
#include <glad/glad.h>

#include <vector>

struct ZC_GUI_Window;

struct ZC_GUI_Obj : public ZC_GUI_EventObj
{
    ZC_GUI_Obj* pObjHolder = nullptr;
    ZC_GUI_Window* pWinHolder = nullptr;

        //  put here unsafe RAM pointers in ctr with params, then change them on pointers from vectors in VGetData()
    ZC_Vec2<float>* pBL = nullptr;
    ZC_GUI_ObjData* pObjData = nullptr;
    bool isFirstGetDataCall = true;

    ZC_GUI_Obj(const ZC_GUI_ObjData& _objData);

    virtual ~ZC_GUI_Obj();

    ZC_Vec2<float> GetPosition();
    float GetWidth();
    float GetHeight();

    virtual bool VSetWindow_Obj(ZC_GUI_Window* pWindow);
    virtual void VSetPosition_Obj(const ZC_Vec2<float>& _bl);    //  override in heirs wich new bl of composite object
    virtual void VGetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount);
    virtual void VGetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex);

    virtual bool VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pPrevObj) { return false; }
    virtual void VEraseObj_Obj(ZC_GUI_Obj* pObj) {}
    virtual bool VSetObjHolder_Obj(ZC_GUI_Obj* _pObjHolder);
    //  to avoid useless call VSetPosition() for all objects, uses when total changing ZC_GUI_Window::bls vector (for example on scrolling window or changing window size), to recalculate border.
    //  Don't forget to bufferSubData all borders in window (among window border) after that call!!!!!!!!!
    virtual void VRecalculateBorder_Obj() {}
};