#include <ZC/GUI/ZC_GUI_ObjBorder.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/GUI/ZC_GUI.h>

ZC_GUI_ObjBorder::ZC_GUI_ObjBorder(const ZC_GUI_ObjData& _objData, bool _isScrollable)
    : ZC_GUI_Obj(_objData),
    pBorder{ new ZC_GUI_Border() },
    isScrollable(_isScrollable)
{
    // this->pObjData->depth = 0.f;  //  allways 0 in border, cause plussing with window depth in geom shader
}

ZC_GUI_ObjBorder::~ZC_GUI_ObjBorder()
{
    if (this->isFirstGetDataCall) if(pBorder) delete pBorder;
}

ZC_Vec2<float>* ZC_GUI_ObjBorder::VGet_pBL_end()
{
    return rows.empty() ? Get_pBL_start() : rows.back().objs.back()->VGet_pBL_end();
}

ZC_GUI_ObjData* ZC_GUI_ObjBorder::VGet_pObjData_end()
{
    return rows.empty() ? Get_pObjData_start() : rows.back().objs.back()->VGet_pObjData_end();
}

ZC_GUI_Obj* ZC_GUI_ObjBorder::VGet_pObj_end()
{
    return rows.empty() ? this : rows.back().objs.back()->VGet_pObj_end();
}

bool ZC_GUI_ObjBorder::VIsDrawing_Obj() const noexcept
{
    return this->pObjData->height == this->actual_height && pBorder->IsDrawing();
}

const ZC_GUI_Row* ZC_GUI_ObjBorder::AddRow(const ZC_GUI_Row& row, const ZC_GUI_Row* pRow_prev)
{
    if (VIsStacionar_Obj() && VIsConfigured_Obj()) return nullptr;  //  window is stacionar and allready configured
    ZC_GUI_Row* pResult = nullptr;
    if (!pRow_prev) pResult = &(rows.emplace_back(row));
    else
    {
        auto iter = std::find(rows.begin(), rows.end(), pRow_prev);
        if (iter == rows.end()) return nullptr;
        pResult = &*(rows.insert(iter, row));
    }
    pResult->SetObjHolder(this);
    if (!pResult->objs.empty() && VIsConfigured_Obj()) VConfigure_Obj();    //  adds not empty row in configured window, need reconfigure
    return pResult;
}

void ZC_GUI_ObjBorder::EraseRow(const Row* pRow)
{
    if (VIsStacionar_Obj() && VIsConfigured_Obj()) return;  //  window is stacionar and allready configured
    if (std::erase(rows, pRow) != 0 && VIsConfigured_Obj()) VConfigure_Obj();
}

bool ZC_GUI_ObjBorder::VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pPrevObj)
{
    if (VIsStacionar_Obj() && VIsConfigured_Obj()) return false;  //  window is stacionar and allready configured
    if (rows.empty()) rows.emplace_back(Row({}));   //  empty rows, add default row
    pObj->SetObjHolder(this);
    bool result = rows.back().AddObj(pObj, pPrevObj);
    if (result && VIsConfigured_Obj()) VConfigure_Obj();
    return result;
}

bool ZC_GUI_ObjBorder::AddObjInRow_Obj(const Row* pRow, ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prevObj)
{
    if (VIsStacionar_Obj() && VIsConfigured_Obj()) return false;  //  window is stacionar and allready configured
    Row* pRow_nonConst = ZC_Find(rows, pRow);
    pObj->SetObjHolder(this);
    bool result = false;
    if (pRow_nonConst) result = pRow_nonConst->AddObj(pObj, pObj_prevObj);
    if (result && VIsConfigured_Obj()) VConfigure_Obj();
    return result;
}

void ZC_GUI_ObjBorder::VEraseObj_Obj(ZC_GUI_Obj* pObj)
{
    if (VIsStacionar_Obj() && VIsConfigured_Obj()) return;  //  window is stacionar and allready configured
    for (Row& row : rows)
    {
        if (std::erase(row.objs, pObj) != 0)
        {
            VIsConfigured_Obj() ? VConfigure_Obj() : VEraseFrom__buttonKeyboard_objs_Obj(pObj);
            break;
        }
    }
}

const ZC_GUI_Border& ZC_GUI_ObjBorder::VGetBorder_Obj()
{
    return *pBorder;
}

ZC_GUI_Obj* ZC_GUI_ObjBorder::VGetObjBorder_Obj()
{
    return this;
}

void ZC_GUI_ObjBorder::VRecalculateBorder_Obj(const ZC_GUI_Border& outer_border)
{
    if (!(this->pObjHolder)) CalculateInternalBorder(outer_border);
    for (Row& row : rows)
        for (ZC_GUI_Obj* pObj : row.objs) pObj->VRecalculateBorder_Obj(*pBorder);
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
    ZC_Vec2<float> border_bl = bl;
    if (border_bl[0] < outer_border.bl[0]) border_bl[0] = outer_border.bl[0];
    if (border_bl[1] < outer_border.bl[1]) border_bl[1] = outer_border.bl[1];
        //  set tr and check with window border tr
    ZC_Vec2<float> border_tr(bl[0] + this->pObjData->width, bl[1] + this->pObjData->height);   //  calculate tr from new position
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
    else *pBorder = ZC_GUI_Border{ .bl = _bl, .tr{ _bl[0] + this->pObjData->width, _bl[1] + this->pObjData->height } };
        //  calculate objs in rows
    ZC_Vec2<float> tl(_bl[0], _bl[1] + this->GetHeight() + scroll_y);
    for (Row& row : rows) row.CalculateObjs_bl(tl, this->GetWidth());
}

void ZC_GUI_ObjBorder::VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount)
{
    ++rBordersCount;
    ++rObjsCount;
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

    for (Row& row: rows)
        for (ZC_GUI_Obj* pObj : row.objs) pObj->VConf_GetData_Obj(rBorder, rBLs, rObjDatas, this->pObjData->borderIndex, rButtonKeyboard_objs);
}

bool ZC_GUI_ObjBorder::VChangeObjsDrawState_Obj(bool needDraw, ZC_GUI_Obj* pObj_start, ZC_GUI_Obj* pObj_end, bool& mustBeChanged)
{
    bool isEnd = false;
    float offset_y = 0.f;
    for (Row& row : rows)
    {
        if (isEnd)
        {
            if (offset_y != 0.f)
            {       //  all pBL is pointers from the large ZC_GUI_Win*** array (after configuration), so we can itarate with them
                ZC_Vec2<float>* pBL_start_index = row.objs.front()->Get_pBL_start();    //  get pBL of the first element in the border
                ZC_Vec2<float>* pBL_end_index = rows.back().objs.back()->VGet_pBL_end();   //  get pBL of the last element in the border
                ++pBL_end_index;    // and incriment to the next (like .end() in stl containers)
                for (ZC_Vec2<float>* i = pBL_start_index; i != pBL_end_index; ++i)
                    (*i)[1] += needDraw ? - offset_y : offset_y;
                VSubDataBL_Obj(pBL_start_index, --pBL_end_index);   //  --pBL_end_index -> to return in borders array range
                ZC_GUI::pGUI->eventManager.UpdateCursorCollision();
            }
            return false;
        }
        for (ZC_GUI_Obj* pObj : row.objs)
        {
            if (!(pObj->VChangeObjsDrawState_Obj(needDraw, pObj_start, pObj_end, mustBeChanged)))
            {
                isEnd = true;
                if (offset_y != 0.f && pObj != row.objs.back())
                    offset_y -= row.rowParams.height + row.rowParams.indent_y;    //  end object not reached end of the row, exclude height of that row from offset_y if height was inluded
                VSubDataObjData_Obj(pObj_start->Get_pObjData_start(), pObj_end->VGet_pObjData_end());
            }
            if (mustBeChanged && pObj == row.objs.front()) offset_y += row.rowParams.height + row.rowParams.indent_y;   //  change draw state from first element of the row, need include rows height in offset_y
            if (isEnd) break;
        }
    }

    return true;
}

bool ZC_GUI_ObjBorder::VIsUseScrollEvent_Obj() const noexcept
{
    return isScrollable;
}

bool ZC_GUI_ObjBorder::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    if (!VCheckCursorCollision_Obj(x, y)) return false;
    
    if (!(this->pObjHolder)) rpObj = this;     //  top level, it's window
    if (VIsUseScrollEvent_Obj()) rpScroll = this;

    for (Row& row : rows)    //  window collision
        for (ZC_GUI_Obj* pObj : row.objs)
            if (pObj->VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)) return true;
    return true;
}

bool ZC_GUI_ObjBorder::VCheckCursorCollision_Obj(float x, float y)
{    //  collision makes with with border, not with drawing elemnt
    return VIsDrawing_Obj() && pBorder->CursorCollision(x, y);
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
            if (pObj->GetHeight() > rowParams.height) rowParams.height = pObj->GetHeight(); //  update the row height to a higher one
            pObj->VSet_pBL_Obj({ cur_x, border_tl[1] - pObj->GetHeight() });
            cur_x += pObj->GetWidth();
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
            if (pObj->GetHeight() > rowParams.height) rowParams.height = pObj->GetHeight(); //  update the row height to a higher one
            pObj->VSet_pBL_Obj({ cur_x, border_tl[1] - pObj->GetHeight() });
            cur_x -= pObj->GetWidth();
        }
    } break;
    case Indent_XFlag::Center:
    {
        float indent_x = 0.f;
        for (ZC_GUI_Obj* pObj : objs) indent_x += pObj->GetWidth();   //  summ of objects width
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
