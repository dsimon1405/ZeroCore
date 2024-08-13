#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
#include <ZC/GUI/ZC_GUI_Window.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>

#include <vector>

struct ZC_GUI_WinImmutable : public ZC_GUI_Window
{
        //  static data
    static inline std::list<ZC_GUI_WinImmutable*> winImmutables;    //  all windows

    static inline std::vector<ZC_GUI_Border> borders;
    static inline std::vector<ZC_Vec2<float>> bls;    //  objecte's bottom left corners
    static inline std::vector<ZC_GUI_ObjData> objDatas;

    static inline GLsizei drawCount = 0;

    static inline ZC_Buffer bufDAICs;
    static inline ZC_Buffer bufBorders;
    static inline ZC_Buffer bufBLs;   //  bottom left corners (positions)
    static inline ZC_Buffer bufObjDatas;
        
        //  object data
    struct ZC_DrawArraysIndirectCommand
    {
        GLuint count = 0;
        GLuint instanceCount = 0;
        GLuint first = 0;
        GLuint baseInstance = 0;   //  store start index of bl and objData (both have same index) for all elements in command
    } daic;
    size_t daicOffset;    //  offset in bufDAICs

    ZC_GUI_WinImmutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags, const ColorsWindow& colorsWindow = {});
    ZC_GUI_WinImmutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ColorsWindow& colorsWindow = {});

    ~ZC_GUI_WinImmutable();

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