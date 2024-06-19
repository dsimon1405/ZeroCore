#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
#include <ZC/GUI/ZC_GUI_Window.h>
// #include <ZC/Video/OpenGL/ZC_GLDraw.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>

#include <vector>

struct ZC_GUI_WinImmutable : public ZC_GUI_Window
{
    // bool isDrawing;
    // ZC_DrawArrays drawArrays;

    struct ZC_DrawArraysIndirectCommand
    {
        GLuint count = 0;
        GLuint instanceCount = 0;
        GLuint first = 0;
        GLuint baseInstance = 0;   //  store start index of bl and objData (both have same index) for all elements in command
    } daic;
    GLintptr daicOffset;    //  offset in bufDAICs

    static inline std::list<ZC_GUI_WinImmutable*> winImmutables;

    static inline std::vector<ZC_GUI_Border> borders;
    static inline std::vector<ZC_Vec2<float>> bls;    //  bottom left corners of simple objectes
    static inline std::vector<ZC_GUI_ObjData> objDatas;

    static inline ZC_Buffer bufDAICs;
    static inline ZC_Buffer bufBorders;
    static inline ZC_Buffer bufBLs;   //  bottom left corners (positions)
    static inline ZC_Buffer bufObjDatas;

    ZC_GUI_WinImmutable(const ZC_WOIData& woiData, const ZC_UV& uv, ZC_GUI_WinFlags _winFlags);

    ~ZC_GUI_WinImmutable();
    
    bool VIsMutable_W() const noexcept override;
    bool VIsConfigured_W() const noexcept override;
    static void ConfigureWindow_W(std::list<ZC_GUI_Window*>* pWindows) ;
    bool VIsDrawing_W() const noexcept override;
    void VSetDrawState_W(bool needDraw) override;
    void VDrawMutable_W() override;
    void VMapObjData_W(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData) override;
    void VSubDataBL_W(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end) override;

    void VCursoreMove_EO(float x, float y, float rel_x, float rel_y, float time) override;
    void VCallAfterZC_WindowResized_WOI() override
    {
        //  make system window resize in event manager. ortho indent will calculate fesh (this->position), if it's differ from current (bl), update all bl list += difference
    }

};