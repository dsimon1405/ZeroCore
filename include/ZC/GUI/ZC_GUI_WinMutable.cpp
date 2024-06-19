#include <ZC/GUI/ZC_GUI_WinMutable.h>

#include <ZC/GUI/ZC_GUI.h>
#include <ZC/GUI/ZC_GUI_Depth.h>

ZC_GUI_WinMutable::ZC_GUI_WinMutable(const ZC_WOIData& woiData, const ZC_UV& uv, ZC_GUI_WinFlags _winFlags)
    : ZC_GUI_Window(woiData, uv, _winFlags),
    isDrawing(_winFlags & ZC_GUI_WF__NeedDraw),
    drawArrays(GL_POINTS,
        _winFlags & ZC_GUI_WF__NoBackground ? 1 : 0,    //  if there is no background, return 1 and start drawing at index 1
        this->objsCount),
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

bool ZC_GUI_WinMutable::VIsMutable_W() const noexcept
{
    return true;
}
bool ZC_GUI_WinMutable::VIsConfigured_W() const noexcept
{
    return !bls.empty();
}

void ZC_GUI_WinMutable::VConfigureWindow_W(std::list<ZC_GUI_Window*>* pWindows)
{
    for (ZC_GUI_WinRow& winRow : rows)
        for (ZC_GUI_Obj* pObj : winRow.objs)
            pObj->VGetBordersAndObjsCount_Obj(this->bordersCount, this->objsCount);

    if (!VIsConfigured_W())    //  buffers allready filled, VConfigureWindow() called on resize from ZC_GUI_Obj::AddObj()/EraseObj()
    {
        borders = {};
        bls = {};
        objDatas = {};
    }
    borders.reserve(this->bordersCount);
    bls.reserve(this->objsCount);
    objDatas.reserve(this->objsCount);

    drawArrays.count = this->objsCount;

    ZC_GUI_Border border_temp = *(this->pBorder);
    ZC_Vec2<float> bl_temp = *(this->pBL);
    ZC_GUI_ObjData objData_temp = *(this->pObjData);
    if (!VIsConfigured_W())    //  if it first configure need delete single pointers created in ctr, and set pointers from vector
    {
        delete this->pBorder;
        delete this->pBL;
        delete this->pObjData;
    }
    this->pBorder = &borders.emplace_back(border_temp);
    this->pBL = &bls.emplace_back(bl_temp);
    this->pObjData = &objDatas.emplace_back(objData_temp);

    for (ZC_GUI_WinRow& winRow : rows)
        for (ZC_GUI_Obj* pObj : winRow.objs)
            pObj->VGetData_Obj(borders, bls, objDatas, 0);  //  0 is start index of border of window

    bufBorders.GLNamedBufferData(sizeof(ZC_GUI_Border) * borders.size(), &borders.front(), GL_DYNAMIC_DRAW);
    bufBLs.GLNamedBufferData(sizeof(ZC_Vec2<float>) * bls.size(), &bls.front(), GL_DYNAMIC_DRAW);
    bufObjDatas.GLNamedBufferData(sizeof(ZC_GUI_ObjData) * objDatas.size(), &objDatas.front(), GL_DYNAMIC_DRAW);
}

bool ZC_GUI_WinMutable::VIsDrawing_W() const noexcept
{
    return isDrawing;
}

void ZC_GUI_WinMutable::VSetDrawState_W(bool needDraw)
{
    if (VIsDrawing_W() == needDraw) return;
    isDrawing = needDraw;
    ZC_GUI::UpdateWindowDrawState(this);
}

void ZC_GUI_WinMutable::VDrawMutable_W()
{
    if (!isDrawing) return;
    if (!VIsConfigured_W()) VConfigureWindow_W();

    bufBorders.GLBindBufferBase();
    bufBLs.GLBindBufferBase();
    bufObjDatas.GLBindBufferBase();
    drawArrays.Draw();
}

void ZC_GUI_WinMutable::VMapObjData_W(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData)
{
    GLintptr offset = pObjData - &objDatas.front() + offsetIn_objData;
    bufObjDatas.GLMapNamedBufferRange_Write(offset, byteSize, pData);
}

void ZC_GUI_WinMutable::VSubDataBL_W(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end)
{
    GLintptr offset = (pBL_start - bls.data()) * sizeof(ZC_Vec2<float>);
    GLsizeiptr byteSize = (pBL_end - pBL_start + 1) * sizeof(ZC_Vec2<float>);
    bufBLs.GLNamedBufferSubData(offset, byteSize, pBL_start);
}

void ZC_GUI_WinMutable::VCursoreMove_EO(float x, float y, float rel_x, float rel_y, float time)
{
    for (ZC_Vec2<float>& rBL : bls)
    {
        rBL[0] += rel_x;
        rBL[1] += rel_y;
    }
    for (ZC_GUI_Border& rBorder : borders)
    {
        if (rBorder.bl[0] == rBorder.tr[0]) continue;   //  border non seen in window, in bl and tr same value, sets in ZC_GUI_ObjBorder::VSetPosition_Obj()
        rBorder.bl[0] += rel_x;
        rBorder.bl[1] += rel_y;
        rBorder.tr[0] += rel_x;
        rBorder.tr[1] += rel_y;
    }
    bufBLs.GLNamedBufferSubData(0, sizeof(ZC_Vec2<float>) * bls.size(), bls.data());
    bufBorders.GLNamedBufferSubData(0, sizeof(ZC_GUI_Border) * borders.size(), borders.data());
}