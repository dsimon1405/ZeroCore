#include "ZC_GUI_Tree.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/GUI/Backend/System/ZC_GUI.h>

ZC_GUI_Tree::ZC_GUI_Tree(float width, float height, const ColorsTree& colorsTree)
    : ZC_GUI_ObjBorder(ZC_GUI_ObjData(width, height, colorsTree.color_tree_background, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons), true, false, colorsTree.colorsScroll)
{}

bool ZC_GUI_Tree::operator == (ZC_ButtonID _buttonId) const noexcept
{
    return ZC_ButtonID::K_F2 == _buttonId;  //  uses for rename
}

void ZC_GUI_Tree::AddBranch(ZC_GUI_BranchSimple* pBranchAdd, ZC_GUI_BranchSimple* pBranchAddTo)
{
    if (!(pBranchAdd->isFirstGetDataCall)) return;    //  branch was allready configured some where
    if (pBranchAddTo)   //  need find row (branche openable) and add under it new row (branch)
    {   //  try to add in not openable branch           win immutable and configured
        if (!(pBranchAddTo->VIsOpenable_Br()) || (!(VIsMutableWin_Obj()) && VIsConfigured_Obj())) return;
        for (auto iter = rows.begin(); iter != rows.end(); ++iter)
        {
            if (iter->objs.front() == pBranchAddTo)     //  found branch to add
            {
                auto next_iter = iter;
                Row& newRow = *(rows.emplace(++next_iter, Row(ZC_GUI_RowParams(iter->rowParams.indent_x + newRowIndent, ZC_GUI_RowParams::Left, 0.f, 0.f), { pBranchAdd })));
                if (!(pBranchAddTo->VIsDrawing_Obj()) || !(pBranchAddTo->VIsOpen_Br()))
                    newRow.objs.front()->VSetDrawState_Obj(false, true);     //  edited branch is not drawn or close, new branch should not be drawn too
                newRow.SetObjHolder(this);      //  set objHolder (must be after VSetDrawState_Obj)
                if (VIsConfigured_Obj()) VConfigure_Obj();    //  need reconfigure
                break;
            }
        }
    }
    else this->AddRow(Row(ZC_GUI_RowParams(0.f, ZC_GUI_RowParams::Left, 0.f, 0.f), { pBranchAdd }));
}

void ZC_GUI_Tree::AddBranches(const std::vector<ZC_GUI_BranchSimple*>& branchesAdd, ZC_GUI_BranchSimple* pBranchAddTo)
{
    if (pBranchAddTo)
    {   //  try to add in not openable branch           win immutable and configured
        if (!(pBranchAddTo->VIsOpenable_Br()) || (!(VIsMutableWin_Obj()) && VIsConfigured_Obj())) return;
        for (auto iter = rows.begin(); iter != rows.end(); ++iter)
        {
            if (iter->objs.front() == pBranchAddTo)     //  found branch to add
            {
                float indent_x = iter->rowParams.indent_x + newRowIndent;
                bool newBranches_notDrawing = !(pBranchAddTo->VIsDrawing_Obj()) || !(pBranchAddTo->VIsOpen_Br());
                ++iter; //  go to next iter for emplace
                for (ZC_GUI_BranchSimple* pBranchAdd : branchesAdd)
                {
                    if (!(pBranchAdd->isFirstGetDataCall)) continue;    //  branch was allready configured some where
                    Row& newRow = *(rows.emplace(iter, Row(ZC_GUI_RowParams(indent_x, ZC_GUI_RowParams::Left, 0.f, 0.f), { pBranchAdd })));
                    if (newBranches_notDrawing) newRow.objs.front()->VSetDrawState_Obj(false, true);     //  edited branch is not drawn or close, new branch should not be drawn too
                    newRow.SetObjHolder(this);      //  set objHolder (must be after VSetDrawState_Obj)
                }
                if (VIsConfigured_Obj()) VConfigure_Obj();    //  need reconfigure
                break;
            }
        }
    }
    else
    {
        for (ZC_GUI_BranchSimple* pBranchAdd : branchesAdd)
            if (pBranchAdd->isFirstGetDataCall) this->AddRow(Row(ZC_GUI_RowParams(0.f, ZC_GUI_RowParams::Left, 0.f, 0.f), { pBranchAdd }));
    }
}

void ZC_GUI_Tree::EraseBranch(ZC_GUI_BranchSimple* pBranchErase)
{
    BranchErase(pBranchErase);
    if (this->VIsMutableWin_Obj() && this->VIsConfigured_Obj()) this->VConfigure_Obj();      //  reconfigure
}

void ZC_GUI_Tree::EraseBranches(const std::vector<ZC_GUI_BranchSimple*>& branchesErase)
{
    for (ZC_GUI_BranchSimple* pBranchErase : branchesErase)
        BranchErase(pBranchErase);
    if (this->VIsMutableWin_Obj() && this->VIsConfigured_Obj()) this->VConfigure_Obj();      //  reconfigure
}

bool ZC_GUI_Tree::MakeActive(ZC_GUI_BranchSimple* pBranch)
{
    if (pBranch == pActiveBranch)
    {
        pBranch->SetButtonColor_BS(pBranch->colorsButton.color_button_pressed, true);
        return false;
    }
    if (pActiveBranch) pActiveBranch->SetButtonColor_BS(pActiveBranch->colorsButton.color_button, true);
    pActiveBranch = pBranch;
    if (pActiveBranch) pActiveBranch->SetButtonColor_BS(pActiveBranch->colorsButton.color_button_pressed, true);
    return true;
}

void ZC_GUI_Tree::ChangeOpenableBranchState(ZC_GUI_BranchOpenable* pBO)
{
    if (!(pBO->VIsOpenable_Br())) return;
    bool needOpen = !(pBO->VIsOpen_Br());
    auto iter = rows.begin();
    while (true)
    {
        if (iter == rows.end()) return;     //  no such branch
        if (iter->objs.front() == pBO) break;   //  found brunch
        ++iter;
    }
    
    float branch_indent = iter->rowParams.indent_x;
    if (++iter == rows.end() && iter->rowParams.indent_x <= branch_indent) return;   //  no sub-branches, next branch doesn't exist or is at the same/higher lavel
    
    ZC_GUI_ObjData* pObjData_start = iter->objs.front()->Get_pObjData_start();   //  start object to change pObjData
    float offset_y = 0.f;
    ChangeSubBranchState(needOpen, iter, branch_indent, offset_y);
    this->VSubDataObjData_Obj(pObjData_start, (--iter)->objs.front()->VGet_pObjData_end());     //  sub data pObjData.height (draw state) for sub-branches
    if (offset_y != 0.f && ++iter != rows.end())      //  move down rest rows
    {
        ZC_Vec2<float>* pBL_start = iter->objs.front()->Get_pBL_start();
        ZC_Vec2<float>* pBL_end = rows.back().objs.front()->VGet_pBL_end();
        for (auto pBL_cur = pBL_start; pBL_cur <= pBL_end; ++pBL_cur)
            (*pBL_cur)[1] += needOpen ? - offset_y : offset_y;
        this->VSubDataBL_Obj(pBL_start, pBL_end);
    }

    if (!needOpen)  //  branch closed, need make scroll up for correct recalculation in CalculateScrollData()
    {
        for (auto iter = --(rows.end()); ; --iter)  //  find last drawing obj
        {
            ZC_GUI_Obj* pObj = iter->objs.front();
            if (pObj->VIsDrawing_Obj())     //  found last drawing obj
            {
                float drawingObj_buttom = pObj->VGetBottom_Obj();
                float offset_y = drawingObj_buttom - this->VGetBottom_Obj();
                if (offset_y > 0.f) this->upScroll->MakeScroll(offset_y, 1.f, false);
                break;
            }
        }
    }
    this->upScroll->CalculateScrollData(true);
    ZC_GUI::pGUI->eventManager.UpdateCursorCollision();
}

void ZC_GUI_Tree::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    *(this->pBL) = _bl;
        //  calculate border
    CalculateInternalBorder(this->pObjHolder->VGetBorder_Obj());    //  calculate border for drawing object
        //  calculate objs in rows
    ZC_Vec2<float> tl(_bl[0], _bl[1] + this->GetHeight() + this->upScroll->scroll_y);

    auto iter = rows.begin();
    SetBranchBL(-1.f, iter, tl);

    upScroll->VSet_pBL_Obj(_bl);  //  calls after all, caurse scroll need top/buttom
}

void ZC_GUI_Tree::VKeyboardButtonUp_Obj(float time)
{
    if (!pActiveBranch) return;
    ZC_GUI_TextInputWindow::StartInputWindow(this->Get_bl_Obj()[0], pActiveBranch->Get_bl_Obj()[1], this->VGetWidth_Obj(), ZC_GUI_BranchSimple::max_symbols,
        pActiveBranch->GetWStr_BMT(), { &ZC_GUI_BranchSimple::UpdateName, pActiveBranch }, false);
}

bool ZC_GUI_Tree::VIsButtonKeyboard_Obj()
{
    return true;
}

void ZC_GUI_Tree::SetBranchBL(float parentBranch_indent_x, std::list<Row>::iterator& rRow_iter, ZC_Vec2<float>& rTL)
{
    while (rRow_iter != rows.end() && parentBranch_indent_x < rRow_iter->rowParams.indent_x)
    {
        rRow_iter->CalculateObjs_bl(rTL, this->VGetWidth_Obj());
        ZC_GUI_BranchSimple* pBranch = dynamic_cast<ZC_GUI_BranchSimple*>(rRow_iter->objs.front());
        if (pBranch->VIsOpenable_Br())
        {
            float cur_indent_x = rRow_iter->rowParams.indent_x;
            ZC_Vec2<float> temp_tl = rTL;   //  if sub-branch open set update total rTL, otherwise only calculate bl of sub-branch with temp_tl
            SetBranchBL(cur_indent_x, ++rRow_iter, pBranch->VIsOpen_Br() ? rTL : temp_tl);
            continue;
        }
        ++rRow_iter;
    }
}

void ZC_GUI_Tree::ChangeSubBranchState(bool needOpen, std::list<Row>::iterator& rRow_iter, float parentBranch_indent_x, float& rOffset_y)
{
    while (rRow_iter != rows.end() && parentBranch_indent_x < rRow_iter->rowParams.indent_x)
    {
        ZC_GUI_BranchSimple* pBranch = dynamic_cast<ZC_GUI_BranchSimple*>(rRow_iter->objs.front());
        pBranch->VSetDrawState_Obj(needOpen, false);
        rOffset_y += rRow_iter->rowParams.indent_y + rRow_iter->rowParams.height;
        if (pBranch->VIsOpenable_Br())
        {
            float cur_indent_x = rRow_iter->rowParams.indent_x;
            if (pBranch->VIsOpen_Br()) 
                ChangeSubBranchState(needOpen, ++rRow_iter, cur_indent_x, rOffset_y);   //  one more opened sub-branche
            else
            {
                while (++rRow_iter != rows.end() && rRow_iter->rowParams.indent_x > cur_indent_x) {}     //  sub-branch is not open, miss all sub-branches in it
            }
            continue;
        }
        ++rRow_iter;
    }
}

void ZC_GUI_Tree::BranchErase(ZC_GUI_BranchSimple* pBranchErase)
{
    std::list<Row>::iterator iter_delete_start;
    std::list<Row>::iterator iter_delete_end;
    for (auto iter = rows.begin(); ; ++iter)      //  find branch
    {
        if (iter == rows.end()) return;     //  can't find branch
        ZC_GUI_Obj* pObj = iter->objs.front();
        if (pObj == pBranchErase)    //  branch found
        {       //  restore obj's default state
            pObj->SetObjHolder(nullptr);    //  null branchs objGolder
            iter_delete_start = iter;
            iter_delete_end = iter_delete_start;
            break;
        }
    }
    if (pBranchErase->VIsOpenable_Br())    //  branch openable, delete all sub-branches
    {
        float delete_branch_indent = iter_delete_end->rowParams.indent_x;
            //  if indent_x of current row is more then delete branch (row) indent, then object is in the deleted branch
        for (++iter_delete_end; iter_delete_end->rowParams.indent_x > delete_branch_indent || iter_delete_end != rows.end(); ++iter_delete_end)
            iter_delete_end->SetObjHolder(nullptr);     //  unset objHolder
    }
    iter_delete_start == iter_delete_end ? rows.erase(iter_delete_start) : rows.erase(iter_delete_start, iter_delete_end);
}


    //  ZC_GUI_Tree::ColorsTree

ZC_GUI_Tree::ColorsTree::ColorsTree(uint _color_tree_background, const Scroll::ColorsScroll& _colorsScroll)
    : color_tree_background(_color_tree_background),
    colorsScroll(_colorsScroll)
{}