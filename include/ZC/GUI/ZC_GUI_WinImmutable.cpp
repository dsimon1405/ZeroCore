#include <ZC/GUI/ZC_GUI_WinImmutable.h>

#include <ZC/GUI/ZC_GUI.h>
#include <ZC/GUI/ZC_GUI_Depth.h>

ZC_GUI_WinImmutable::ZC_GUI_WinImmutable(const ZC_WOIData& woiData, const ZC_UV& uv, ZC_GUI_WinFlags _winFlags)
    : ZC_GUI_Window(woiData, uv, _winFlags),
    daic{
        .count = this->objsCount,
        .instanceCount = _winFlags & ZC_GUI_WF__NeedDraw,
        .first = _winFlags & ZC_GUI_WF__NoBackground ? 1 : 0,    //  if there is no background, return 1 and start drawing at index 1
        }
    // bufBorders(GL_SHADER_STORAGE_BUFFER, bind_Border),
    // bufBLs(GL_SHADER_STORAGE_BUFFER, bind_BL),
    // bufObjDatas(GL_SHADER_STORAGE_BUFFER, bind_ObjData)
{
    if (_winFlags & ZC_GUI_WF__NeedDraw) SetFocuseDepth();
    ZC_GUI::AddWindow(this);
    winImmutables.emplace_back(this);
}

ZC_GUI_WinImmutable::~ZC_GUI_WinImmutable()
{
    ZC_GUI::EraseWindow(this);
}

bool ZC_GUI_WinImmutable::VIsMutable_W() const noexcept
{
    return false;
}

bool ZC_GUI_WinImmutable::VIsConfigured_W() const noexcept
{
    return !bls.empty();
}

void ZC_GUI_WinImmutable::ConfigureWindow_W(std::list<ZC_GUI_Window*>* pWindows)
{
    GLsizeiptr totalBorders = 0,
        totalObjs = 0;
    for (ZC_GUI_WinImmutable* pWin : winImmutables)
    {
        for (ZC_GUI_WinRow& winRow : pWin->rows)
            for (ZC_GUI_Obj* pObj : winRow.objs)
                pObj->VGetBordersAndObjsCount_Obj(pWin->bordersCount, pWin->objsCount);

        pWin->daic.count = pWin->objsCount;    //  count drawing elements (GL_POINTS) in window
        totalBorders += pWin->bordersCount;
        totalObjs += pWin->objsCount;
    }

    std::vector<ZC_DrawArraysIndirectCommand> daics;
    daics.reserve(winImmutables.size());
    borders.reserve(totalBorders);
    bls.reserve(totalObjs);
    objDatas.reserve(totalObjs);

    for (ZC_GUI_WinImmutable* pWin : winImmutables)
    {
        pWin->daicOffset = static_cast<GLintptr>(daics.size() * sizeof(ZC_DrawArraysIndirectCommand));
        int winBorderIndex = static_cast<int>(borders.size());

        ZC_GUI_Border*  pBorder_temp = &borders.emplace_back(*(pWin->pBorder));
        ZC_Vec2<float>* pBL_temp = &bls.emplace_back(*(pWin->pBL));
        ZC_GUI_ObjData* pObjData_temp = &objDatas.emplace_back(*(pWin->pObjData));
        delete pWin->pBorder;
        delete pWin->pBL;
        delete pWin->pObjData;
        pWin->pBorder = pBorder_temp;
        pWin->pBL = pBL_temp;
        pWin->pObjData = pObjData_temp;

        for (ZC_GUI_WinRow& winRow : pWin->rows)
            for (ZC_GUI_Obj* pObj : winRow.objs)
                pObj->VGetData_Obj(borders, bls, objDatas, winBorderIndex);  //  0 is start index of border of window
    }

    // bufBorders.GLNamedBufferData(sizeof(ZC_GUI_Border) * borders.size(), &borders.front(), GL_DYNAMIC_DRAW);
    // bufBLs.GLNamedBufferData(sizeof(ZC_Vec2<float>) * bls.size(), &bls.front(), GL_DYNAMIC_DRAW);
    // bufObjDatas.GLNamedBufferData(sizeof(ZC_GUI_ObjData) * objDatas.size(), &objDatas.front(), GL_DYNAMIC_DRAW);
}

// bool ZC_GUI_WinImmutable::VIsDrawing_W() const noexcept
// {
//     return isDrawing;
// }

// void ZC_GUI_WinImmutable::VSetDrawState_W(bool needDraw)
// {
//     if (VIsDrawing_W() == needDraw) return;
//     isDrawing = needDraw;
//     ZC_GUI::UpdateWindowDrawState(this);
// }

// void ZC_GUI_WinImmutable::VDrawMutable_W()
// {
//     if (!isDrawing) return;
//     if (!VIsConfigured_W()) VConfigureWindow_W();

//     bufBorders.GLBindBufferBase();
//     bufBLs.GLBindBufferBase();
//     bufObjDatas.GLBindBufferBase();
//     drawArrays.Draw();
// }

// void ZC_GUI_WinImmutable::VMapObjData_W(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData)
// {
//     GLintptr offset = pObjData - &objDatas.front() + offsetIn_objData;
//     bufObjDatas.GLMapNamedBufferRange_Write(offset, byteSize, pData);
// }

// void ZC_GUI_WinImmutable::VSubDataBL_W(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end)
// {
//     GLintptr offset = (pBL_start - bls.data()) * sizeof(ZC_Vec2<float>);
//     GLsizeiptr byteSize = (pBL_end - pBL_start + 1) * sizeof(ZC_Vec2<float>);
//     bufBLs.GLNamedBufferSubData(offset, byteSize, pBL_start);
// }

// void ZC_GUI_WinImmutable::VCursoreMove_EO(float x, float y, float rel_x, float rel_y, float time)
// {
//     for (ZC_Vec2<float>& rBL : bls)
//     {
//         rBL[0] += rel_x;
//         rBL[1] += rel_y;
//     }
//     for (ZC_GUI_Border& rBorder : borders)
//     {
//         if (rBorder.bl[0] == rBorder.tr[0]) continue;   //  border non seen in window, in bl and tr same value, sets in ZC_GUI_ObjBorder::VSetPosition_Obj()
//         rBorder.bl[0] += rel_x;
//         rBorder.bl[1] += rel_y;
//         rBorder.tr[0] += rel_x;
//         rBorder.tr[1] += rel_y;
//     }
//     bufBLs.GLNamedBufferSubData(0, sizeof(ZC_Vec2<float>) * bls.size(), bls.data());
//     bufBorders.GLNamedBufferSubData(0, sizeof(ZC_GUI_Border) * borders.size(), borders.data());
// }