#include "ZC_GUI_WinImmutable.h"

#include <ZC/GUI/Backend/System/ZC_GUI.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

ZC_GUI_WinImmutable::ZC_GUI_WinImmutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags, const ColorsWindow& colorsWindow)
    : ZC_GUI_WinImmutable(_woiData, ZC_GUI_IconUV::quad, _winFlags, colorsWindow)
{}

ZC_GUI_WinImmutable::ZC_GUI_WinImmutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ColorsWindow& colorsWindow)
    : ZC_GUI_Window(_woiData, uv, _winFlags, colorsWindow),
    daic{
        .count = static_cast<GLuint>(_winFlags & ZC_GUI_WF__NoBackground ? 0 : 1),    //  if there is no background, return 0 and draw count became 0
        .instanceCount = static_cast<GLuint>(_winFlags & ZC_GUI_WF__NeedDraw ? 1 : 0),   //  make instanceCount 1 for drawing, otherwise 0
        .first = static_cast<GLuint>(_winFlags & ZC_GUI_WF__NoBackground ? 1 : 0),    //  if there is no background, return 1 and start drawing at index 1
        }
{
    if (_winFlags & ZC_GUI_WF__NeedDraw) SetFocuseDepthAndColor();
    ZC_GUI::AddWindow(this);
    winImmutables.emplace_back(this);
}

ZC_GUI_WinImmutable::~ZC_GUI_WinImmutable()
{
    ZC_GUI::EraseWindow(this);
    std::erase(winImmutables, this);

    if (upDS_con)       //  if immutable window destroyes, program is end. Need to free not stl static data here.
    {
        bufDAICs = ZC_Buffer();
        bufBorders = ZC_Buffer();
        bufBLs = ZC_Buffer();
        bufObjDatas = ZC_Buffer();
        upDS_con = nullptr;
    }
}

void ZC_GUI_WinImmutable::VSetDrawState_W(bool needDraw)
{
    if (VIsDrawing_Obj() == needDraw) return;
    if (needDraw)
    {
        daic.instanceCount = 1u;

        if (winFlags & ZC_GUI_WF__OutAreaClickClose)
            ZC_GUI::pGUI->eventManager.SetMouseButtonDownWatcherObj(this);

        ++draw_winds_count;
    }
    else
    {
        daic.instanceCount = 0u;

        if (ZC_GUI::pGUI->eventManager.pWin_mouseButtonDown_watcher == this)
            ZC_GUI::pGUI->eventManager.SetMouseButtonDownWatcherObj(nullptr);
    
        --draw_winds_count;
    }
    
    if (VIsConfigured_Obj()) bufDAICs.GLMapNamedBufferRange_Write(daicOffset + offsetof(ZC_DrawArraysIndirectCommand, instanceCount),
        sizeof(ZC_DrawArraysIndirectCommand::instanceCount), &(daic.instanceCount));
    ZC_GUI::UpdateWindowDrawState(this);

    upDS_con->SwitchToDrawLvl(ZC_RL_Default, draw_winds_count > 0u ? ZC_DrawerLevels::Gui : ZC_DL_None);
}

bool ZC_GUI_WinImmutable::VIsDrawing_Obj() const noexcept
{
    return daic.instanceCount == 1u;  //  1 for drawing, otherwise 0
}

void ZC_GUI_WinImmutable::VConfigure_Obj()
{
    if (winImmutables.empty()) return;

    for (ZC_GUI_WinImmutable* pWin : winImmutables)
        pWin->VSet_pBL_Obj(pWin->Get_bl_Obj());     //  calculate bl for all window's objects

    GLsizeiptr totalBorders = 0ll,
        totalObjs = 0ll;
    for (ZC_GUI_WinImmutable* pWin : winImmutables)
    {
        pWin->VConf_GetBordersAndObjsCount_Obj(pWin->bordersCount, pWin->objsCount);

        pWin->daic.count = pWin->objsCount - pWin->daic.first;    //  count drawing elements (GL_POINTS) in window; daic.first shows is border drown or not
        totalBorders += pWin->bordersCount;
        totalObjs += pWin->objsCount;

        draw_winds_count += pWin->daic.instanceCount;
    }

    std::vector<ZC_DrawArraysIndirectCommand> daics;    //  don't need on class level, caurse all data will be actual only on gpu and each immutable window change daic data for it self, using -> daicOffset
    daics.reserve(winImmutables.size());
    borders.reserve(totalBorders);
    bls.reserve(totalObjs);
    objDatas.reserve(totalObjs);

    for (ZC_GUI_WinImmutable* pWin : winImmutables)
    {
        pWin->daicOffset = daics.size() * sizeof(ZC_DrawArraysIndirectCommand);

        pWin->daic.baseInstance = bls.size();      //  base instance is index in bufBLs and bufObjDatas (for gpu use)
        daics.emplace_back(pWin->daic);
        pWin->VConf_GetData_Obj(borders, bls, objDatas, 0, pWin->buttonKeyboard_objs);
    }

    daics_count = daics.size();

    using namespace ZC_GUI_Bindings;
    bufDAICs = ZC_Buffer(GL_DRAW_INDIRECT_BUFFER);
    bufBorders = ZC_Buffer(GL_SHADER_STORAGE_BUFFER, bind_Border);
    bufBLs = ZC_Buffer(GL_SHADER_STORAGE_BUFFER, bind_BL);
    bufObjDatas = ZC_Buffer(GL_SHADER_STORAGE_BUFFER, bind_ObjData);
    bufDAICs.GLNamedBufferStorage(sizeof(ZC_DrawArraysIndirectCommand) * daics_count, daics.data(), GL_MAP_WRITE_BIT);
    bufBorders.GLNamedBufferStorage(sizeof(ZC_GUI_Border) * borders.size(), borders.data(), GL_DYNAMIC_STORAGE_BIT);
    bufBLs.GLNamedBufferStorage(sizeof(ZC_Vec2<float>) * bls.size(), bls.data(), GL_DYNAMIC_STORAGE_BIT);
    bufObjDatas.GLNamedBufferStorage(sizeof(ZC_GUI_ObjData) * objDatas.size(), objDatas.data(), GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    mult_draw_arr_indir = ZC_MultiDrawArraysIndirect(&bufDAICs, GL_POINTS, 0, daics_count, 0);

    upDS_con = new ZC_DSController(&(ZC_ShProgs::Get(ShPN_ZC_GUI)->shProg), &mult_draw_arr_indir, &ZC_GUI::pGUI->drawManager.vao_empty,
        ZC_TexturesHolder{ .pTexture = ZC_GUI::pGUI->drawManager.textures.data(), .texturesCount = 2u }, std::forward_list<ZC_uptr<ZC_RSPersonalData>>{},
        std::forward_list<ZC_DSController::RenderSet>{ { ZC_RL_Default } }, { &bufBorders, &bufBLs, &bufObjDatas }, nullptr);

    if (draw_winds_count > 0u) upDS_con->SwitchToDrawLvl(ZC_RL_Default, ZC_DrawerLevels::Gui);
}

bool ZC_GUI_WinImmutable::VIsConfigured_Obj() const noexcept
{
    return !bls.empty();
}

bool ZC_GUI_WinImmutable::VIsMutableWin_Obj() const noexcept
{
    return false;
}

// void ZC_GUI_WinImmutable::VDraw_W()
// {
//     if (bls.empty()) return;    //  no data

//     bufBorders.GLBindBufferBase();
//     bufBLs.GLBindBufferBase();
//     bufObjDatas.GLBindBufferBase();

//     bufDAICs.BindBuffer();  //  bind indirect buffer

//     glMultiDrawArraysIndirect(GL_POINTS, 0, drawCount, 0);
// }

void ZC_GUI_WinImmutable::VReconf_UpdateTextUV_W()
{       //  update uv in text objs
    for (ZC_GUI_WinImmutable* pWin : winImmutables)
        for (Row& row : pWin->rows)
            for (ZC_GUI_Obj* pObj : row.objs) pObj->VConf_SetTextUV_Obj();
        //  update all the data (more productive than map many UVs one at a time)
    bufObjDatas.GLNamedBufferSubData(0, sizeof(ZC_GUI_ObjData) * objDatas.size(), objDatas.data());
}

void ZC_GUI_WinImmutable::VMapObjData_Obj(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData)
{
    if (VIsConfigured_Obj()) bufObjDatas.GLMapNamedBufferRange_Write((pObjData - objDatas.data()) * sizeof(ZC_GUI_ObjData) + offsetIn_objData, byteSize, pData);
}

void ZC_GUI_WinImmutable::VSubDataBL_Obj(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end)
{
    if (VIsConfigured_Obj()) bufBLs.GLNamedBufferSubData((pBL_start - bls.data()) * sizeof(ZC_Vec2<float>), (pBL_end - pBL_start + 1) * sizeof(ZC_Vec2<float>), pBL_start);
}

void ZC_GUI_WinImmutable::VSubDataBorder_Obj(ZC_GUI_Border* pBorder_start, ZC_GUI_Border* pBorder_end)
{
    if (VIsConfigured_Obj()) bufBorders.GLNamedBufferSubData((pBorder_start - borders.data()) * sizeof(ZC_GUI_Border), (pBorder_end - pBorder_start + 1) * sizeof(ZC_GUI_Border), pBorder_start);
}

void ZC_GUI_WinImmutable::VSubDataObjData_Obj(ZC_GUI_ObjData* pObjData_start, ZC_GUI_ObjData* pObjData_end)
{
    if (VIsConfigured_Obj()) bufObjDatas.GLNamedBufferSubData((pObjData_start - objDatas.data()) * sizeof(ZC_GUI_ObjData), (pObjData_end - pObjData_start + 1) * sizeof(ZC_GUI_ObjData), pObjData_start);
}

void ZC_GUI_WinImmutable::VCursorMove_W(float rel_x, float rel_y)
{
    ZC_Vec2<float> rel(rel_x, rel_y);
    for (GLuint i = 0; i < this->objsCount; ++i) *(pBL + i) += rel;
    for (GLsizeiptr i = 0; i < this->bordersCount; ++i)
    {
        ZC_GUI_Border& rBorder = *(pBorder + i);
        // if (rBorder.bl[0] == rBorder.tr[0]) continue;   //  border non seen in window, in bl and tr same value, sets in ZC_GUI_ObjBorder::VSetPosition_Obj()
        rBorder.bl += rel;
        rBorder.tr += rel;
    }
    bufBLs.GLNamedBufferSubData((pBL - bls.data()) * sizeof(ZC_Vec2<float>), sizeof(ZC_Vec2<float>) * this->objsCount, pBL);
    bufBorders.GLNamedBufferSubData((pBorder - borders.data()) * sizeof(ZC_GUI_Border), sizeof(ZC_GUI_Border) * this->bordersCount, pBorder);
}