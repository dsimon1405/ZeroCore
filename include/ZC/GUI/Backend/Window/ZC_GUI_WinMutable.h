#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_Window.h>
#include <ZC/GUI/Backend/ZC_GUI_ObjData.h>
#include <ZC/Video/OpenGL/ZC_GLDraw.h>

#include <vector>

struct ZC_GUI_WinMutable : public ZC_GUI_Window
{
    bool isDrawing;
    ZC_DrawArrays drawArrays;

    std::vector<ZC_GUI_Border> borders;
    std::vector<ZC_Vec2<float>> bls;    //  bottom left corners of simple objectes
    std::vector<ZC_GUI_ObjData> objDatas;

    ZC_Buffer bufBorders;
    ZC_Buffer bufBLs;   //  bottom left corners (positions)
    ZC_Buffer bufObjDatas;

    ZC_GUI_WinMutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags, const ColorsWindow& colorsWindow = {});
    ZC_GUI_WinMutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ColorsWindow& colorsWindow = {});

    ~ZC_GUI_WinMutable();

    void VSetDrawState_W(bool needDraw) override;

    bool VIsDrawing_Obj() const noexcept override;
    void VConfigure_Obj() override;
    bool VIsConfigured_Obj() const noexcept override;
    
    bool VIsMutableWin_Obj() const noexcept override;
    void VDraw_W() override;
    void VReconf_UpdateTextUV_W() override;
    void VMapObjData_Obj(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData) override;
    void VSubDataBL_Obj(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end) override;
    void VSubDataBorder_Obj(ZC_GUI_Border* pBorder_start, ZC_GUI_Border* pBorder_end) override;
    void VSubDataObjData_Obj(ZC_GUI_ObjData* pObjData_start, ZC_GUI_ObjData* pObjData_end) override;

    void VCursorMove_Obj(float rel_x, float rel_y) override;
};