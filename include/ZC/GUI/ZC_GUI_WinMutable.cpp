#include <ZC/GUI/ZC_GUI_WinMutable.h>

#include <ZC/GUI/ZC_GUI.h>
#include <ZC/GUI/ZC_GUI_Depth.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>

using namespace ZC_GUI_Bindings;

ZC_GUI_WinMutable::ZC_GUI_WinMutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags)
    : ZC_GUI_Window(_woiData, _winFlags),
    isDrawing(_winFlags & ZC_GUI_WF__NeedDraw),
    drawArrays(GL_POINTS,
        _winFlags & ZC_GUI_WF__NoBackground ? 1 : 0,    //  if there is no background, return 1 and start drawing at index 1
        _winFlags & ZC_GUI_WF__NoBackground ? 0 : 1),
    bufBorders(GL_SHADER_STORAGE_BUFFER, bind_Border),
    bufBLs(GL_SHADER_STORAGE_BUFFER, bind_BL),
    bufObjDatas(GL_SHADER_STORAGE_BUFFER, bind_ObjData)
{
    if (isDrawing) SetFocuseDepth();
    ZC_GUI::AddWindow(this);
}

ZC_GUI_WinMutable::ZC_GUI_WinMutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags)
    : ZC_GUI_Window(_woiData, uv, _winFlags),
    isDrawing(_winFlags & ZC_GUI_WF__NeedDraw),
    drawArrays(GL_POINTS,
        _winFlags & ZC_GUI_WF__NoBackground ? 1 : 0,    //  if there is no background, return 1 and start drawing at index 1
        _winFlags & ZC_GUI_WF__NoBackground ? 0 : 1),
    bufBorders(GL_SHADER_STORAGE_BUFFER, bind_Border),
    bufBLs(GL_SHADER_STORAGE_BUFFER, bind_BL),
    bufObjDatas(GL_SHADER_STORAGE_BUFFER, bind_ObjData)
{
    if (isDrawing) SetFocuseDepth();
    ZC_GUI::AddWindow(this);
}

ZC_GUI_WinMutable::~ZC_GUI_WinMutable()
{
    ZC_GUI::EraseWindow(this);
}

bool ZC_GUI_WinMutable::VIsDrawing_Obj() const noexcept
{
    return isDrawing;
}

void ZC_GUI_WinMutable::VConfigure_Obj()
{
    VConf_Set_bl_Obj(*(this->pBL));     //  calculate bl for all window's objects
    VConf_GetBordersAndObjsCount_Obj(this->bordersCount, this->objsCount);

    if (VIsConfigured_Obj())    //  buffers allready filled (reconfigureation)
    {
        borders = {};
        bls = {};
        objDatas = {};
    }
    borders.reserve(this->bordersCount);
    bls.reserve(this->objsCount);
    objDatas.reserve(this->objsCount);

    drawArrays.count = this->objsCount - drawArrays.first;    //  count drawing elements (GL_POINTS) in window; daic.first shows is border drown or not

    VConf_GetData_Obj(borders, bls, objDatas, 0);

    bufBorders.GLNamedBufferData(sizeof(ZC_GUI_Border) * borders.size(), borders.data(), GL_DYNAMIC_DRAW);
    bufBLs.GLNamedBufferData(sizeof(ZC_Vec2<float>) * bls.size(), bls.data(), GL_DYNAMIC_DRAW);
    bufObjDatas.GLNamedBufferData(sizeof(ZC_GUI_ObjData) * objDatas.size(), objDatas.data(), GL_DYNAMIC_DRAW);
}

bool ZC_GUI_WinMutable::VIsConfigured_Obj() const noexcept
{
    return !bls.empty();
}

bool ZC_GUI_WinMutable::VIsMutable_W() const noexcept
{
    return true;
}

void ZC_GUI_WinMutable::VSetDrawState_W(bool needDraw)
{
    if (VIsDrawing_Obj() == needDraw) return;
    if (needDraw && IsMovable() && !(this->woiData.indentFlags & ZC_WOIF__X_Left_Pixel))    //  look ZC_GUI_WF__Movable or ZC_GUI_Window ctr
        SetNewIndentParams((*pBL)[0], (*pBL)[1], ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel);
    isDrawing = needDraw;
    ZC_GUI::UpdateWindowDrawState(this);
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

void ZC_GUI_WinMutable::VMapObjData_Obj(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData)
{
    GLintptr offset = (pObjData - objDatas.data()) * sizeof(ZC_GUI_ObjData) + offsetIn_objData;
    bufObjDatas.GLMapNamedBufferRange_Write(offset, byteSize, pData);
}

void ZC_GUI_WinMutable::VSubDataBL_Obj(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end)
{
    GLintptr offset = (pBL_start - bls.data()) * sizeof(ZC_Vec2<float>);
    GLsizeiptr byteSize = (pBL_end - pBL_start + 1) * sizeof(ZC_Vec2<float>);
    bufBLs.GLNamedBufferSubData(offset, byteSize, pBL_start);
}

void ZC_GUI_WinMutable::VCursoreMove_EO(float rel_x, float rel_y)
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