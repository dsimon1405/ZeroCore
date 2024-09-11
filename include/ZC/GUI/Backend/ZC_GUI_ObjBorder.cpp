#include <ZC/GUI/Backend/ZC_GUI_ObjBorder.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/GUI/Backend/ZC_GUI.h>
#include <ZC/Video/ZC_SWindow.h>
#include <ZC/GUI/Backend/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/ZC_GUI_Bindings.h>

#include <algorithm>

ZC_GUI_ObjBorder::ZC_GUI_ObjBorder(const ZC_GUI_ObjData& _objData, bool _isScrollable, float frameWidth, const ColorsObjBorder& colorsObjBorder)
    : ZC_GUI_Obj(_objData),
    pBorder{ new ZC_GUI_Border() },
    upScroll(_isScrollable ? new Scroll(this->GetHeight() - (frameWidth * 2.f), colorsObjBorder.colorsScroll) : nullptr),
    // upFrame(nullptr)
    upFrame(frameWidth == 0.f ? nullptr : new Frame(frameWidth, this->VGetWidth_Obj(), this->GetHeight(), colorsObjBorder.color_frame))
{
    if (upScroll) upScroll->SetObjHolder(this);
    if (upFrame) upFrame->SetObjHolder(this);
}

ZC_GUI_ObjBorder::ZC_GUI_ObjBorder(ZC_GUI_ObjBorder&& ob)
    : ZC_GUI_Obj(dynamic_cast<ZC_GUI_Obj&&>(ob)),
    pBorder(ob.pBorder),
    rows(std::move(ob.rows)),
    upScroll(std::move(ob.upScroll)),
    upFrame(std::move(ob.upFrame))
{
    if (upScroll) upScroll->SetObjHolder(this);
    if (upFrame) upFrame->SetObjHolder(this);
}

ZC_GUI_ObjBorder::~ZC_GUI_ObjBorder()
{
    if (this->isFirstGetDataCall) if(pBorder) delete pBorder;
    if (upScroll) upScroll->SetObjHolder(nullptr);
    for (Row& row : rows)   //  unset obj holder
        for (ZC_GUI_Obj* pObj : row.objs)
            pObj->SetObjHolder(nullptr);
    if (this->pObjHolder) pObjHolder->VEraseObj_Obj(this);  //  erase itself from holder
}

ZC_Vec2<float>* ZC_GUI_ObjBorder::VGet_pBL_end()
{
    return upScroll ? upScroll->VGet_pBL_end()
                    : rows.empty() ? upFrame ? upFrame->VGet_pBL_end()
                                             : Get_pBL_start()
                                    : rows.back().objs.back()->VGet_pBL_end();
    // return rows.empty() ? Get_pBL_start() : rows.back().objs.back()->VGet_pBL_end();
}

ZC_GUI_ObjData* ZC_GUI_ObjBorder::VGet_pObjData_end()
{
    return upScroll ? upScroll->VGet_pObjData_end()
                    : rows.empty() ? upFrame ? upFrame->VGet_pObjData_end()
                                             : Get_pObjData_start()
                                    : rows.back().objs.back()->VGet_pObjData_end();
    // return rows.empty() ? Get_pObjData_start() : rows.back().objs.back()->VGet_pObjData_end();
}

ZC_GUI_Obj* ZC_GUI_ObjBorder::VGet_pObj_end()
{
    return rows.empty() ? this : rows.back().objs.back()->VGet_pObj_end();
}

bool ZC_GUI_ObjBorder::VIsDrawing_Obj() const noexcept
{
    return this->pObjData->height == this->actual_height && pBorder->IsDrawing();
}

const ZC_GUI_Row* ZC_GUI_ObjBorder::AddRow(ZC_GUI_Row&& row, const ZC_GUI_Row* pRow_prev)
{
    if (!(VIsMutableWin_Obj()) && VIsConfigured_Obj()) return nullptr;  //  window is stacionar and allready configured
    ZC_GUI_Row* pResult = nullptr;
    if (!pRow_prev) pResult = &(rows.emplace_back(std::move(row)));
    else
    {
        auto iter = std::find(rows.begin(), rows.end(), pRow_prev);
        if (iter == rows.end()) return nullptr;
        pResult = &*(rows.emplace(iter, std::move(row)));
    }
    if (pResult)
    {
        pResult->SetObjHolder(this);
        if (!pResult->objs.empty() && VIsConfigured_Obj()) VConfigure_Obj();    //  adds not empty row in configured window, need reconfigure
    }
    return pResult;
}

void ZC_GUI_ObjBorder::EraseRow(const Row* pRow)
{
    if (std::erase(rows, pRow) != 0 && VIsMutableWin_Obj() && VIsConfigured_Obj()) VConfigure_Obj();
}

bool ZC_GUI_ObjBorder::VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev)
{
    if (!(VIsMutableWin_Obj()) && VIsConfigured_Obj()) return false;  //  window is stacionar and allready configured
    if (rows.empty()) rows.emplace_back(Row({}));   //  empty rows, add default row
    pObj->SetObjHolder(this);
    bool result = rows.back().AddObj(pObj, pObj_prev);
    if (result && VIsConfigured_Obj()) VConfigure_Obj();
    return result;
}

bool ZC_GUI_ObjBorder::AddObjInRow_B(const Row* pRow, ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev)
{
    if (!(VIsMutableWin_Obj()) && VIsConfigured_Obj()) return false;  //  window is immutable and allready configured
    Row* pRow_nonConst = ZC_Find(rows, pRow);
    pObj->SetObjHolder(this);
    bool result = false;
    if (pRow_nonConst) result = pRow_nonConst->AddObj(pObj, pObj_prev);
    if (result && VIsConfigured_Obj()) VConfigure_Obj();
    return result;
}

void ZC_GUI_ObjBorder::VEraseObj_Obj(ZC_GUI_Obj* pObj)
{
    for (Row& row : rows)
    {
        if (std::erase(row.objs, pObj) != 0)
        {
            VEraseFrom__buttonKeyboard_objs_Obj(pObj);
            break;
        }
    }
}

ZC_GUI_Border& ZC_GUI_ObjBorder::VGetBorder_Obj()
{
    return *pBorder;
}

ZC_GUI_Obj* ZC_GUI_ObjBorder::VGetObjBorder_Obj()
{
    return this;
}

void ZC_GUI_ObjBorder::CalculateInternalBorder(const ZC_GUI_Border& outer_border)
{
    if (outer_border.bl == outer_border.tr)     //   outer border is made for discard, as in the end of current function. So make same
    {
        pBorder->tr = pBorder->bl;
        return;
    }
    ZC_Vec2<float> bl = Get_bl_Obj();
        //  set bl and check with window border bl
    ZC_Vec2<float> border_bl = upFrame ? bl + upFrame->VGetWidth_Obj() : bl;   //  if have frame move on fame border size
    if (border_bl[0] < outer_border.bl[0]) border_bl[0] = outer_border.bl[0];
    if (border_bl[1] < outer_border.bl[1]) border_bl[1] = outer_border.bl[1];
        //  set tr and check with window border tr
    ZC_Vec2<float> border_tr(bl[0] + this->VGetWidth_Obj(), bl[1] + this->GetHeight());   //  calculate tr from new position
    if (upFrame) border_tr -= upFrame->VGetWidth_Obj();   //  if have frame move on fame border size
    if (outer_border.tr[0] < border_tr[0]) border_tr[0] = outer_border.tr[0];
    if (outer_border.tr[1] < border_tr[1]) border_tr[1] = outer_border.tr[1];

    pBorder->bl = border_bl;
        //  check border size, if X or Y of bl greater then tr, border is not valid, set same data in bl and tr for check in geometry shader,
        //  to path only one fragment in fragment shader (can't discard in vertex and geometry shaders, so send to fragment shader only one fragment(pixel))
    pBorder->tr = border_tr[0] < border_bl[0] || border_tr[1] < border_bl[1] ? border_bl : border_tr;
}

void ZC_GUI_ObjBorder::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    *(this->pBL) = _bl;
        //  calculate border
    if (this->pObjHolder) CalculateInternalBorder(this->pObjHolder->VGetBorder_Obj());    //  if not window, just a border in window, so calculate border for drawing object
    else *pBorder = ZC_GUI_Border{ .bl = upFrame ? _bl + upFrame->VGetWidth_Obj() : _bl, .tr{ _bl[0] + this->pObjData->width - (upFrame ? upFrame->VGetWidth_Obj() : 0.f),
        _bl[1] + this->pObjData->height - (upFrame ? upFrame->VGetWidth_Obj() : 0.f) } };
        //  tl take from calculated border (that includes frame border if it is)
    ZC_Vec2<float> tl(pBorder->bl[0], _bl[1] + this->GetHeight() - (upFrame ? upFrame->VGetWidth_Obj() : 0.f) + (upScroll ? upScroll->scroll_y : 0));
    for (Row& row : rows)
        row.CalculateObjs_bl(tl, this->VGetWidth_Obj());

    if (upScroll) upScroll->VSet_pBL_Obj(_bl);  //  calls after all, caurse scroll need top/buttom
    if (upFrame) upFrame->VSet_pBL_Obj(_bl);    //  no metter where to call (don't include in scroll)
}

void ZC_GUI_ObjBorder::VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount)
{
    ++rBordersCount;
    ++rObjsCount;

    if (upFrame) upFrame->VConf_GetBordersAndObjsCount_Obj(rBordersCount, rObjsCount);
    if (upScroll) upScroll->VConf_GetBordersAndObjsCount_Obj(rBordersCount, rObjsCount);

    for (Row& row : rows)
        for (ZC_GUI_Obj* pObj : row.objs)
            pObj->VConf_GetBordersAndObjsCount_Obj(rBordersCount, rObjsCount);
}

void ZC_GUI_ObjBorder::VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas,
    int borderIndex, std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs)
{
    this->pObjData->borderIndex = static_cast<int>(rBorder.size());    //  minus one is not needed because we get the size before adding border

    ZC_GUI_Border* pBorder_temp = &rBorder.emplace_back(*pBorder);
    ZC_Vec2<float>* pBL_temp = &rBLs.emplace_back(*pBL);
    ZC_GUI_ObjData* pObjData_temp = &rObjDatas.emplace_back(*pObjData);
    if (this->isFirstGetDataCall)
    {
        delete pBorder;
        delete this->pBL;
        delete this->pObjData;
        isFirstGetDataCall = false;
    }
    pBorder = pBorder_temp;
    this->pBL = pBL_temp;
    this->pObjData = pObjData_temp;

    if (VIsButtonKeyboard_Obj()) rButtonKeyboard_objs.emplace_front(this);
                                                //  if have obj holder it's frame into the window, otherwise it's window frame and must have index_border = -1
    if (upFrame) upFrame->VConf_GetData_Obj(rBorder, rBLs, rObjDatas, this->pObjHolder ? this->pObjData->borderIndex : -1, rButtonKeyboard_objs);

    for (Row& row: rows)
        for (ZC_GUI_Obj* pObj : row.objs)
            pObj->VConf_GetData_Obj(rBorder, rBLs, rObjDatas, this->pObjData->borderIndex, rButtonKeyboard_objs);

    if (upScroll) upScroll->VConf_GetData_Obj(rBorder, rBLs, rObjDatas, this->pObjData->borderIndex, rButtonKeyboard_objs);     //  must be after all (for after all drawing)
}

void ZC_GUI_ObjBorder::VConf_SetTextUV_Obj()
{
    for (Row& row: rows)
        for (ZC_GUI_Obj* pObj : row.objs) pObj->VConf_SetTextUV_Obj();
}

bool ZC_GUI_ObjBorder::VIsUseScrollEvent_Obj() const noexcept
{
    return upScroll != nullptr && upScroll->IsActive();
}

bool ZC_GUI_ObjBorder::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    if (!VCheckCursorCollision_Obj(x, y)) return false;
    
    if (!(this->pObjHolder)) rpObj = this;     //  top level, it's window
    if (VIsUseScrollEvent_Obj())
    {
        rpScroll = this;
        if (upScroll->VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)) return true;   //  must be before all, at first make scroll buttons collision
    }
    
    for (Row& row : rows)    //  window collision
        for (ZC_GUI_Obj* pObj : row.objs)
            if (pObj->VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)) return true;
    return true;
}

bool ZC_GUI_ObjBorder::VCheckCursorCollision_Obj(float x, float y)
{    //  collision makes with with border, not with drawing elemnt
    return VIsDrawing_Obj() && pBorder->CursorCollision(x, y);
}

void ZC_GUI_ObjBorder::VScroll_Obj(float vertical, float time)
{
    if (upScroll) upScroll->MakeScroll(vertical, 20.f, true);
}

void ZC_GUI_ObjBorder::VNewScrollObj_underCursor_Obj(ZC_GUI_Obj* pObj_underCursor)
{
    if (upScroll) upScroll->ChangeDrawState(pObj_underCursor == this);
}

void ZC_GUI_ObjBorder::VMoveBL_Obj(float rel_x, float rel_y, int& update_borders)
{
    MoveVec2(rel_x, rel_y, *(this->pBL));
    if (this->pObjHolder)   //  not a window border, recalculate border
    {
        CalculateInternalBorder(this->pObjHolder->VGetBorder_Obj());
    }
    else    //  window border, just move border
    {
        MoveVec2(rel_x, rel_y, pBorder->bl);
        MoveVec2(rel_x, rel_y, pBorder->tr);
    }
    ++update_borders;
    if (upFrame) upFrame->VMoveBL_Obj(rel_x, rel_y, update_borders);
    if (upScroll) upScroll->VMoveBL_Obj(rel_x, rel_y, update_borders);
    for (Row& row : rows)    //  window collision
        for (ZC_GUI_Obj* pObj : row.objs)
            pObj->VMoveBL_Obj(rel_x, rel_y, update_borders);
}



    //  ZC_GUI_ObjBorder::Row ZC_GUI_Row

ZC_GUI_Row::Row(const RowParams& _rowParams, std::list<ZC_GUI_Obj*> _objs)
    : rowParams(_rowParams),
    objs(std::move(_objs))
{}

bool ZC_GUI_Row::operator == (const Row* pRow) const noexcept
{
    return this == pRow;
}

bool ZC_GUI_Row::AddObj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev)
{
    if (!pObj_prev) objs.emplace_back(pObj);
    else
    {
        auto iter = std::find(objs.begin(), objs.end(), pObj_prev);
        if (iter == objs.end()) return false;
        objs.insert(iter, pObj);
    }
    return true;
}

void ZC_GUI_Row::SetObjHolder(ZC_GUI_Obj* pObjHolder)
{
    for (ZC_GUI_Obj* pObj : objs) pObj->SetObjHolder(pObjHolder);
}

void ZC_GUI_Row::CalculateObjs_bl(ZC_Vec2<float>& border_tl, float border_width)
{
    border_tl[1] -= rowParams.indent_y;
        //  calculates bl for objects going from left to right
    auto lambCalc_bl_fromLeft = [this, border_tl](float cur_x)
    {
        for (ZC_GUI_Obj* pObj : objs)
        {
            cur_x += pObj == objs.front() ? rowParams.indent_x : rowParams.distance_x;
            float compositeHeight = pObj->VGetHeightComposite_Obj();
            if (compositeHeight > rowParams.height) rowParams.height = compositeHeight; //  update the row height to a higher one
            pObj->VSet_pBL_Obj({ cur_x, border_tl[1] - compositeHeight });
            cur_x += pObj->VGetWidthComposite_Obj();
        }
    };
    typedef typename RowParams::Indent_X Indent_XFlag;
    switch (rowParams.indentFlag_X)
    {
    case Indent_XFlag::Left: lambCalc_bl_fromLeft(border_tl[0]); break;
    case Indent_XFlag::Right:
    {
        float cur_x = border_tl[0] + border_width;
        for (ZC_GUI_Obj* pObj : objs)
        {
            cur_x -= pObj == objs.front() ? rowParams.indent_x : rowParams.distance_x;
            float compositeHeight = pObj->VGetHeightComposite_Obj();
            if (compositeHeight > rowParams.height) rowParams.height = compositeHeight; //  update the row height to a higher one
            pObj->VSet_pBL_Obj({ cur_x, border_tl[1] - compositeHeight });
            cur_x -= pObj->VGetWidthComposite_Obj();
        }
    } break;
    case Indent_XFlag::Center:
    {
        float indent_x = 0.f;
        for (ZC_GUI_Obj* pObj : objs) indent_x += pObj->VGetWidthComposite_Obj();   //  sum of objects width
        indent_x += (rowParams.distance_x * objs.size()) - rowParams.distance_x;    //  plus distance between objects
        indent_x = (border_width - indent_x) / 2.f;     //  final indent_x in the border calculates from Border's width
        lambCalc_bl_fromLeft(border_tl[0] + indent_x);
    } break;
    }
    border_tl[1] -= rowParams.height;
}



    //  ZC_GUI_ObjBorder::Row ZC_GUI_Row::RowParams

ZC_GUI_RowParams::RowParams(float _indent_x, Indent_X _indentFlag_X, float _indent_y, float _distance_x)
    : indent_x(_indent_x),
    indentFlag_X(_indentFlag_X),
    indent_y(_indent_y),
    distance_x(_distance_x)
{}



    //  ZC_GUI_ObjBorder::Scroll

ZC_GUI_ObjBorder::Scroll::Scroll(float height, const ColorsScroll& colorsScroll)
    : ZC_GUI_ObjComposite(ZC_GUI_ObjData(scroll_width, height, colorsScroll.color_scroll_background, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons)),
    caret(colorsScroll)
{
    this->VAddObj_Obj(&caret, nullptr);
    this->VSetDrawState_Obj(false, false);
}

void ZC_GUI_ObjBorder::Scroll::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    ZC_GUI_ObjBorder* pOB = dynamic_cast<ZC_GUI_ObjBorder*>(this->pObjHolder);
    *(this->pBL) = ZC_Vec2<float>(_bl[0] + pOB->VGetWidth_Obj() - (pOB->upFrame ? pOB->upFrame->VGetWidth_Obj() : 0.f) - this->pObjData->width, pOB->upFrame ? _bl[1] + pOB->upFrame->VGetWidth_Obj() : _bl[1]);
    CalculateScrollData(false);
}

bool ZC_GUI_ObjBorder::Scroll::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    return caret.MakeCursorCollision_Obj(x, y, rpObj, rpScroll) || MakeCursorCollision_Obj(x, y, rpObj, rpScroll);
}

bool ZC_GUI_ObjBorder::Scroll::VMouseButtonLeftDown_Obj(float time)
{
    if (isMBL_pressed) return false;
    isMBL_pressed = true;
    float caret_center = caret.Get_bl_Obj()[1] + (caret.GetHeight() / 2.f);
    float cursor_y = GetCursor_Y();
    MakeScroll((cursor_y - caret_center) / caret.height_coef, 1.f, true);
    return true;
}

void ZC_GUI_ObjBorder::Scroll::VMouseButtonLeftUp_Obj(float time)
{
    isMBL_pressed = false;
}

void ZC_GUI_ObjBorder::Scroll::CalculateScrollData(bool updateGPU)
{
    ZC_GUI_ObjBorder* pOB = dynamic_cast<ZC_GUI_ObjBorder*>(this->pObjHolder);
    if (pOB->rows.empty()) return;

    float total_top = 0.f;
    for (auto& row : pOB->rows)     //  cicle to avoid rows with empty objs
    {
        total_top += row.rowParams.indent_y;
        if (!row.objs.empty())
        {
            total_top += row.objs.front()->VGetTop_Obj();
            break;
        }
    }

    float total_bottom = total_top;     //  set something higher then bottom
    for (auto iter = --(pOB->rows.end()); ; --iter)
    {
        for (ZC_GUI_Obj* pObj : iter->objs)
        {
            if (pObj->VIsDrawing_Obj())
            {
                float bottom = pObj->VGetBottom_Obj();
                if (total_bottom > bottom) total_bottom = bottom;
            }
        }
        if (total_bottom != total_top) break;
        if (iter == pOB->rows.begin())
        {
            caret.height_coef = 0.f;
            caret_move_max = 0.f;
            this->VSetDrawState_Obj(false, updateGPU);
            return;
        }
    }
    float total_height = total_top - total_bottom;
    caret.height_coef = this->GetHeight() / total_height;
    if (caret.height_coef >= 1.f)   //  total height less or equal border (window) height, no need scroll
    {
        caret.height_coef = 0.f;
        caret_move_max = 0.f;
        this->VSetDrawState_Obj(false, updateGPU);
        return;
    }
        //  update caret height
    float caret_height = this->GetHeight() * caret.height_coef;
    if (this->VIsConfigured_Obj() && caret.VIsDrawing_Obj()) caret.pObjData->height = caret_height;   //  need update height if obj is drawing. Must be before changing caret.actual_height, caurse: caret.actual_height must be 0.f or equal caret.height
    caret.actual_height = caret_height;   //  must be updated actual_height

    if (caret.VIsDrawing_Obj() && updateGPU)
        VMapObjData_Obj(caret.pObjData, offsetof(ZC_GUI_ObjData, height), sizeof(ZC_GUI_ObjData::height), &(caret.pObjData->height));      //  if need gpu update and caret drawing, update height
    caret_move_max = total_height - this->GetHeight();
    caret.VSet_pBL_Obj({ (*(this->pBL))[0], (*(this->pBL))[1] + (this->GetHeight() - caret.GetHeight()) - (scroll_y * caret.height_coef) });
    if (updateGPU) VSubDataBL_Obj(this->Get_pBL_start(), caret.VGet_pBL_end());     //  if need gpu update, update scroll and caret bls
}

bool ZC_GUI_ObjBorder::Scroll::IsActive() const
{
    return caret_move_max != 0.f;
}

void ZC_GUI_ObjBorder::Scroll::MakeScroll(float vertical, float speed, bool callCursorCollisiton)
{
    if (!IsActive()) return;
    float scroll_move = -vertical * speed;
    float newScroll_y = scroll_y + scroll_move;

    auto lambScrollObjs = [this](float rel_y)
    {
        ZC_GUI_ObjBorder* pOB = dynamic_cast<ZC_GUI_ObjBorder*>(this->pObjHolder);
        int changed_borders = 0;
        ZC_Vec2<float>* pBL_start = nullptr;    //  to avoid empty rows on star take in next cicle
        ZC_Vec2<float>* pBL_end = nullptr;  //  to avoid empty rows on end take in next cicle
        for(auto row : pOB->rows)
        {
            for (ZC_GUI_Obj* pObj : row.objs)
            {
                pObj->VMoveBL_Obj(0.f, rel_y, changed_borders);
                if (!pBL_start) pBL_start = pObj->Get_pBL_start();
                pBL_end = pObj->VGet_pBL_end();
            }
        }
        this->VSubDataBL_Obj(pBL_start, pBL_end);
        if (changed_borders > 0)    //  where changed border into pOB, need sub data
            this->VSubDataBorder_Obj(pOB->pBorder + 1, pOB->pBorder + changed_borders);   //  pOB->pBorder must not be changed, must be changed border inside that border
    };

    if (newScroll_y < 0)
    {
        if (scroll_y == 0) return;  //  Y move length = 0
        lambScrollObjs(-scroll_y);  //  move objs to 0
        scroll_y = 0.f;
    }
    else if (newScroll_y > caret_move_max)
    {
        float rel_y = caret_move_max - scroll_y;
        if (rel_y == 0) return;  //  Y move length = 0
        lambScrollObjs(rel_y);  //  move obj to caret max value
        scroll_y = caret_move_max;
    }
    else
    {
        lambScrollObjs(newScroll_y - scroll_y);
        scroll_y = newScroll_y;
    }
        //  update caret pos
    caret.VSet_pBL_Obj({ (*(this->pBL))[0], (*(this->pBL))[1] + (this->GetHeight() - caret.GetHeight()) - (scroll_y * caret.height_coef) });
    VSubDataBL_Obj(this->Get_pBL_start(), caret.VGet_pBL_end());     //  if need gpu update, update scroll and caret bls

    if (callCursorCollisiton) ZC_GUI::pGUI->eventManager.UpdateCursorCollision();
}

void ZC_GUI_ObjBorder::Scroll::ChangeDrawState(bool needDraw)
{
    if (IsActive()) this->VSetDrawState_Obj(needDraw, true);
}

float ZC_GUI_ObjBorder::Scroll::GetCursor_Y()
{
    float x = 0,
        y = 0;
    ZC_SWindow::GetCursorPosition(x, y);
    return y;
}


    //  ZC_GUI_ObjBorder::Scroll::ColorsScroll

ZC_GUI_ObjBorder::Scroll::ColorsScroll::ColorsScroll(uint _color_scroll_background, const ZC_GUI_ButtonBase::ColorsButton& _colors_caret_scroll)
    : color_scroll_background(_color_scroll_background),
    colors_caret_scroll(_colors_caret_scroll)
{}


    //  ZC_GUI_ObjBorder::Scroll::Caret

ZC_GUI_ObjBorder::Scroll::Caret::Caret(const ColorsScroll& colorsScroll)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(Scroll::scroll_width, 0.f, 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF_M__CursorMoveOnMBLPress, colorsScroll.colors_caret_scroll),
    ZC_GUI_ButtonMouse(Scroll::scroll_width, 0.f, ZC_GUI_BF_M__CursorMoveOnMBLPress)
{}

void ZC_GUI_ObjBorder::Scroll::Caret::VCursorCollisionEnd_Obj(float time)
{
    if (this->pObjData->color == colorsButton.color_button_pressed) return;     //  button pressed, wait while up
    this->pObjData->color = colorsButton.color_button;
    if (ZC_GUI::pGUI->eventManager.pObj_scroll != this->VGetObjBorder_Obj())
    {
        this->pObjHolder->VSetDrawState_Obj(false, false);
        this->VSubDataObjData_Obj(this->pObjHolder->pObjData, this->pObjData);
    }
    else this->VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
}

void ZC_GUI_ObjBorder::Scroll::Caret::VCursorMove_Obj(float rel_x, float rel_y)
{
    if (rel_y == 0.f) return;
    ZC_GUI_Obj* border = this->VGetObjBorder_Obj();
    float cursor_y = Scroll::GetCursor_Y();
    if (border->VGetTop_Obj() >= cursor_y && cursor_y >= border->VGetBottom_Obj())    //  cursor Y somewhere in caret height (cursor X may not belong caret width)
        dynamic_cast<Scroll*>(pObjHolder)->MakeScroll(rel_y / height_coef, 1.f, true);
}


    //  ZC_GUI_ObjBorder::ColorsObjBorder

ZC_GUI_ObjBorder::ColorsObjBorder::ColorsObjBorder(const Scroll::ColorsScroll& _colorsScroll, uint _color_frame)
    : colorsScroll(_colorsScroll),
    color_frame(_color_frame)
{}


    //  ZC_GUI_ObjBorder::Frame

ZC_GUI_ObjBorder::Frame::Frame(float frame_width, float objBorder_width, float objBorder_height, uint color)
    : ZC_GUI_ObjComposite(ZC_GUI_ObjData(frame_width, objBorder_height, color, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons)),
    top(ZC_GUI_ObjData(objBorder_width, frame_width, color, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons)),
    right(ZC_GUI_ObjData(frame_width, objBorder_height, color, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons)),
    bottom(ZC_GUI_ObjData(objBorder_width, frame_width, color, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons))
{
    this->VAddObj_Obj(&top, nullptr);
    this->VAddObj_Obj(&right, nullptr);
    this->VAddObj_Obj(&bottom, nullptr);
}

void ZC_GUI_ObjBorder::Frame::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{       //  frame width is left(this) or right width, or top or bottom height
    (*this->pBL) = _bl;
    top.VSet_pBL_Obj({ _bl[0], _bl[1] + this->GetHeight() - top.GetHeight() });
    right.VSet_pBL_Obj({ _bl[0] + top.VGetWidth_Obj() - right.VGetWidth_Obj(), _bl[1] });
    bottom.VSet_pBL_Obj(_bl);
}