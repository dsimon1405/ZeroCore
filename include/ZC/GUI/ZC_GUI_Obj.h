#pragma once

#include <ZC/GUI/ZC_GUI_EventObj.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>
#include <glad/glad.h>

#include <vector>


struct ZC_GUI_Obj : public ZC_GUI_EventObj
{
    ZC_GUI_Obj* pObjHolder = nullptr;
        //  put here unsafe RAM pointers in ctr with params, then change them on pointers from vectors in VGetData()
    ZC_Vec2<float>* pBL = nullptr;
    ZC_GUI_ObjData* pObjData = nullptr;

    bool isFirstGetDataCall = true;

    ZC_GUI_Obj(const ZC_GUI_ObjData& _objData);

    ~ZC_GUI_Obj();

    ZC_Vec2<float> GetPosition_bl_Obj();
    float GetWidth();
    float GetHeight();
    void SetObjHolder(ZC_GUI_Obj* _pObjHolder);

    
    virtual bool VIsStacionar_Obj() const noexcept;    //  override in ZC_GUI_Win...
    virtual bool VIsDrawing_Obj() const noexcept = 0;

    virtual bool VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pPrevObj) { return false; }
    virtual void VEraseObj_Obj(ZC_GUI_Obj* pObj) {}
    
    virtual const ZC_GUI_Border& VGetBorder_Obj();
        //  to avoid useless call VSetPosition() for all objects, uses when total changing ZC_GUI_Window::bls vector (for example on scrolling window or changing window size), to recalculate border.
        //  Don't forget to bufferSubData all borders in window (among window border) after that call!!!!!!!!!
    virtual void VRecalculateBorder_Obj(const ZC_GUI_Border& outer_border) {}
    
    virtual void VConfigure_Obj();    //  override in ZC_GUI_Win...
        //  true if configured, ohterwise false
    virtual bool VIsConfigured_Obj() const noexcept;    //  override in ZC_GUI_Win...
    virtual void VConf_Set_bl_Obj(const ZC_Vec2<float>& _bl);
    virtual void VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount);
    virtual void VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex);

    virtual void VMapObjData_Obj(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData);
    virtual void VSubDataBL_Obj(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end);

    virtual bool VIsUseScrollEvent_Obj() const noexcept { return false; }

    bool MakeCursorCollision_Obj(float x, float y, ZC_GUI_EventObj*& rpWindow, ZC_GUI_EventObj*& rpObj, ZC_GUI_EventObj*& rpScroll);

    bool VCheckCursorCollision_EO(float x, float y) override;
};