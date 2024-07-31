#include <ZC/GUI/ZC_GUI_WinMutable.h>

#include <ZC/GUI/ZC_GUI.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>
#include "ZC_GUI_IconUV.h"

ZC_GUI_WinMutable::ZC_GUI_WinMutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags)
    : ZC_GUI_WinMutable(_woiData, ZC_GUI_IconUV::window, _winFlags)
{}

ZC_GUI_WinMutable::ZC_GUI_WinMutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags)
    : ZC_GUI_Window(_woiData, uv, _winFlags),
    isDrawing(_winFlags & ZC_GUI_WF__NeedDraw),
    drawArrays(GL_POINTS,
        _winFlags & ZC_GUI_WF__NoBackground ? 1 : 0,    //  if there is no background, return 1 and start drawing at index 1
        _winFlags & ZC_GUI_WF__NoBackground ? 0 : 1),
    bufBorders(GL_SHADER_STORAGE_BUFFER, ZC_GUI_Bindings::bind_Border),
    bufBLs(GL_SHADER_STORAGE_BUFFER, ZC_GUI_Bindings::bind_BL),
    bufObjDatas(GL_SHADER_STORAGE_BUFFER, ZC_GUI_Bindings::bind_ObjData)
{
    if (isDrawing) SetFocuseDepthAndColor();
    ZC_GUI::AddWindow(this);
}

ZC_GUI_WinMutable::~ZC_GUI_WinMutable()
{
    ZC_GUI::EraseWindow(this);
}

void ZC_GUI_WinMutable::VSetDrawState_W(bool needDraw)
{
    if (VIsDrawing_Obj() == needDraw) return;
    if (needDraw && this->VIsUseCursorMoveEventOnMBLetfDown_Obj() && !(this->woiData.indentFlags & ZC_WOIF__X_Left_Pixel))    //  look ZC_GUI_WF__Movable or ZC_GUI_Window ctr
        SetNewIndentParams((*pBL)[0], (*pBL)[1], ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel);
    isDrawing = needDraw;
    ZC_GUI::UpdateWindowDrawState(this);
}

bool ZC_GUI_WinMutable::VIsDrawing_Obj() const noexcept
{
    return isDrawing;
}

void ZC_GUI_WinMutable::VConfigure_Obj()
{
    this->VSet_pBL_Obj(this->Get_bl_Obj());     //  calculate bl for all window's objects
    
    this->bordersCount = 0;     //  make default value for reconfiguration case
    this->objsCount = 0;
    this->VConf_GetBordersAndObjsCount_Obj(this->bordersCount, this->objsCount);

    if (VIsConfigured_Obj()) buttonKeyboard_objs = {};
        //  store data in temp vectors for teconfiguration case
    std::vector<ZC_GUI_Border> temp_borders;
    temp_borders.reserve(this->bordersCount);
    std::vector<ZC_Vec2<float>> temp_bls;
    temp_bls.reserve(this->objsCount);
    std::vector<ZC_GUI_ObjData> temp_objDatas;
    temp_objDatas.reserve(this->objsCount);

    drawArrays.count = this->objsCount - drawArrays.first;    //  count drawing elements (GL_POINTS) in window; daic.first shows is border drawn or not

    VConf_GetData_Obj(temp_borders, temp_bls, temp_objDatas, 0, this->buttonKeyboard_objs);

    borders = std::move(temp_borders);
    bls = std::move(temp_bls);
    objDatas = std::move(temp_objDatas);

    bufBorders.GLNamedBufferData(sizeof(ZC_GUI_Border) * borders.size(), borders.data(), GL_DYNAMIC_DRAW);
    bufBLs.GLNamedBufferData(sizeof(ZC_Vec2<float>) * bls.size(), bls.data(), GL_DYNAMIC_DRAW);
    bufObjDatas.GLNamedBufferData(sizeof(ZC_GUI_ObjData) * objDatas.size(), objDatas.data(), GL_DYNAMIC_DRAW);
}

bool ZC_GUI_WinMutable::VIsConfigured_Obj() const noexcept
{
    return !bls.empty();
}

bool ZC_GUI_WinMutable::VIsMutableWin_Obj() const noexcept
{
    return true;
}

void ZC_GUI_WinMutable::VDraw_W()
{
    if (!isDrawing) return;
    if (!VIsConfigured_Obj()) VConfigure_Obj();

    bufBorders.GLBindBufferBase();
    bufBLs.GLBindBufferBase();
    bufObjDatas.GLBindBufferBase();
    drawArrays.Draw();
}

void ZC_GUI_WinMutable::VReconf_UpdateTextUV_W()
{       //  update uv in text objs
    for (Row& row : this->rows)
        for (ZC_GUI_Obj* pObj : row.objs) pObj->VConf_SetTextUV_Obj();
        //  update all the data (more productive than map many UVs one at a time)
    bufObjDatas.GLNamedBufferSubData(0, sizeof(ZC_GUI_ObjData) * objDatas.size(), objDatas.data());
}

void ZC_GUI_WinMutable::VMapObjData_Obj(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData)
{
    if (VIsConfigured_Obj()) bufObjDatas.GLMapNamedBufferRange_Write(((pObjData - objDatas.data()) * sizeof(ZC_GUI_ObjData)) + offsetIn_objData, byteSize, pData);
}

void ZC_GUI_WinMutable::VSubDataBL_Obj(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end)
{
    if (VIsConfigured_Obj()) bufBLs.GLNamedBufferSubData((pBL_start - bls.data()) * sizeof(ZC_Vec2<float>), (pBL_end - pBL_start + 1) * sizeof(ZC_Vec2<float>), pBL_start);
}

void ZC_GUI_WinMutable::VSubDataBorder_Obj(ZC_GUI_Border* pBorder_start, ZC_GUI_Border* pBorder_end)
{
    if (VIsConfigured_Obj()) bufBorders.GLNamedBufferSubData((pBorder_start - borders.data()) * sizeof(ZC_GUI_Border), (pBorder_end - pBorder_start + 1) * sizeof(ZC_GUI_Border), pBorder_start);
}

void ZC_GUI_WinMutable::VSubDataObjData_Obj(ZC_GUI_ObjData* pObjData_start, ZC_GUI_ObjData* pObjData_end)
{
    if (VIsConfigured_Obj()) bufObjDatas.GLNamedBufferSubData((pObjData_start - objDatas.data()) * sizeof(ZC_GUI_ObjData), (pObjData_end - pObjData_start + 1) * sizeof(ZC_GUI_ObjData), pObjData_start);
}

void ZC_GUI_WinMutable::VCursorMove_Obj(float rel_x, float rel_y)
{
    ZC_Vec2<float> rel(rel_x, rel_y);
    for (ZC_Vec2<float>& rBL : bls) rBL += rel;
    for (ZC_GUI_Border& rBorder : borders)
    {
        if (rBorder.bl[0] == rBorder.tr[0]) continue;   //  border non seen in window, in bl and tr same value, sets in ZC_GUI_ObjBorder::VSetPosition_Obj()
        rBorder.bl += rel;
        rBorder.tr += rel;
    }
    bufBLs.GLNamedBufferSubData(0, sizeof(ZC_Vec2<float>) * this->objsCount, bls.data());
    bufBorders.GLNamedBufferSubData(0, sizeof(ZC_GUI_Border) * this->bordersCount, borders.data());
}