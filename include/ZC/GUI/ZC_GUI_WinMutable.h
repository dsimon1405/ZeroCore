#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
#include <ZC/GUI/ZC_GUI_Window.h>
#include <ZC/Video/OpenGL/ZC_GLDraw.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>

#include <vector>

typedef int ZC_GUI_WinFlags;
enum ZC_GUI_WinFlag
{
    ZC_GUI_WF__Stacionar        = 1,        //  The window is always under open windows (if the window is in focus and not stationary windows overlap it, the stationary window is still drawn under those windows)
    ZC_GUI_WF__NeedDraw         = 1 << 1,   //  Need draw on start
    ZC_GUI_WF__NoBackground     = 1 << 2,   //  Don't draw background (window backgground don't make collisioin with mouse cursor, objects of the window still make collision)
    ZC_GUI_WF__Scrable          = 1 << 3,   //  Can be used scrall (can't be used with ZC_GUI_WF__NoBackground)
};

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

    ZC_GUI_WinMutable(const ZC_WOIData& woiData, const ZC_UV& uv, ZC_GUI_WinFlags winFlags);

    ~ZC_GUI_WinMutable();
    
    bool VIsMutable_W() const noexcept override;
    bool VIsConfigured_W() const noexcept override;
    void VConfigureWindow_W() override;
    bool VIsDrawing_W() const noexcept override;
    bool VIsBackground() const noexcept override;
    void VSetDrawState(bool needDraw) override;

    void VDrawMutable_W() override;

    void VMapObjData_W(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData) override;
};




// struct ZC_DrawArraysIndirectCommand
// {
//     GLuint count = 0;
//     GLuint instanceCount = 0;
//     GLuint first = 0;
//     GLuint baseInstance = 0;   //  store start index of bl and objData (both have same index) for all elements in command
// };
    // ZC_DrawArraysIndirectCommand daic;
    // ZC_Buffer bufDAICs;