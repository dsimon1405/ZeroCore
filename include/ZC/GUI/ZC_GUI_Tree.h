#pragma once

#include <ZC/GUI/ZC_GUI_ObjBorder.h>
#include <ZC/GUI/ZC_GUI_IconUV.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>

#include <ZC/GUI/ZC_GUI_ButtonMouseText.h>
#include <ZC/GUI/ZC_GUI_Colors.h>

#include <ZC/GUI/ZC_GUI_IconUV.h>
#include <ZC/GUI/ZC_GUI.h>

#include <iostream>
struct ZC_GUI_Branch : public ZC_GUI_ButtonMouseText
{
    static inline int max_symbols = 20;   //  max symbols in text of each branch

    struct ColorsBranch
    {
        uint color_text = ZC_GUI_Colors::tree_branch_text;
        ZC_GUI_ButtonBase::ColorsButton colorsBranchButton;

        ColorsBranch(uint _color_text = ZC_GUI_Colors::tree_branch_text, const ZC_GUI_ButtonBase::ColorsButton& _colorsBranchButton =
                ZC_GUI_ButtonBase::ColorsButton(ZC_GUI_Colors::tree_branch_button, ZC_GUI_Colors::tree_branch_button_under_cursor, ZC_GUI_Colors::tree_branch_button_pressed))
            : color_text(_color_text),
            colorsBranchButton(_colorsBranchButton)
        {}
    };

    ZC_GUI_Branch(const std::wstring& name, const ColorsBranch& colorsBranch = {})
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData(0.f, 0.f, 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, colorsBranch.colorsBranchButton),
        ZC_GUI_ButtonMouseText(0.f, 0.f, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(0.f, ZC_GUI_TextForButton::Indent::Left), name, false, 0, ZC_GUI_TextAlignment::Left, colorsBranch.color_text))
    {}

    void VLeftButtonUp_BM(float time) override;

    virtual bool VIsOpenable_Br()
    {
        return false;
    }

    virtual bool VIsOpen_Br()
    {
        return false;
    }

    virtual void VBecameActive_Br()
    { std::cout<<"active branch: "<<this<<std::endl; }
    // {}

    void UpdateName(const std::wstring& name)
    {
        ZC_GUI_TextManager::Text* pText = ZC_GUI_TextManager::GetText(name, false, 0, ZC_GUI_TextAlignment::Left);
        this->UpdateText_BMT(pText);

        this->pObjData->width = pText->width;
        this->VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), sizeof(ZC_GUI_ObjData::width), &(this->pObjData->width));
    }

    static void SetMaxSymbolsCount(int count)
    {
        if (count > max_symbols) max_symbols = count;
    }
};

struct ZC_GUI_BranchOpenable : public ZC_GUI_Branch
{
    struct BranchOpenableColors
    {
        ZC_GUI_Branch::ColorsBranch colorsBranch;
        ZC_GUI_ButtonBase::ColorsButton colorsArrowButton;

        BranchOpenableColors(const ZC_GUI_Branch::ColorsBranch& _colorsBranch = {}, const ZC_GUI_ButtonBase::ColorsButton& _colorsArrowButton =
                ZC_GUI_ButtonBase::ColorsButton(ZC_GUI_Colors::tree_branch_arrow, ZC_GUI_Colors::tree_branch_arrow_under_cursor, ZC_GUI_Colors::tree_branch_arrow_pressed))
            : colorsBranch(_colorsBranch),
            colorsArrowButton(_colorsArrowButton)
        {}
    };

    struct ArrowButton : public ZC_GUI_ButtonMouse
    {
        ArrowButton(float size, bool isOpen, const ZC_GUI_ButtonBase::ColorsButton& colorsArrowButton)
            : ZC_GUI_ButtonBase(ZC_GUI_ObjData(size, size, 0, isOpen ? ZC_GUI_IconUV::arrow_down : ZC_GUI_IconUV::arrow_right, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, colorsArrowButton),
            ZC_GUI_ButtonMouse(size, size, ZC_GUI_BF__None)
        {}

        bool IsOpen()
        {
            return this->pObjData->uv.bl[0] == ZC_GUI_IconUV::arrow_down.bl[0];
        }

        void ChangeArrow()
        {
            this->pObjData->uv = IsOpen() ? ZC_GUI_IconUV::arrow_right : ZC_GUI_IconUV::arrow_down;
            this->VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, uv), sizeof(ZC_GUI_ObjData::uv), &(this->pObjData->uv));
        }

        void VLeftButtonUp_BM(float time) override;
    };

    ArrowButton arrowButton;

    ZC_GUI_BranchOpenable(const std::wstring& name, bool isOpen, const BranchOpenableColors& branchOpenableColors = {})
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData(0.f, 0.f, 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF_M__DoubleCLick, branchOpenableColors.colorsBranch.colorsBranchButton),
        ZC_GUI_Branch(name, branchOpenableColors.colorsBranch),
        arrowButton(float(ZC_GUI_TextManager::GetFontHeight()), isOpen, branchOpenableColors.colorsArrowButton)
    {
        this->VAddObj_Obj(&arrowButton, nullptr);
    }

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
    {
        arrowButton.VSet_pBL_Obj(_bl);      //  at first open/close button
        *pBL = { _bl[0] + arrowButton.VGetWidth_Obj(), _bl[1] };    //  text button righter, on half font height
        this->objs.front()->VSet_pBL_Obj(*this->pBL);   //  set texts bl
    }

    void VLeftButtonDoubleClick_BM(float time) override;

    bool VIsOpenable_Br() override
    {
        return true;
    }

    bool VIsOpen_Br() override
    {
        return arrowButton.IsOpen();
    }

    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override
    {
        return MakeCursorCollision_Obj(x, y, rpObj, rpScroll) || arrowButton.MakeCursorCollision_Obj(x, y, rpObj, rpScroll);
    }
};

struct ZC_GUI_Tree : public ZC_GUI_ObjBorder
{
    ZC_GUI_Branch* pActiveBranch = nullptr;
    static inline const float newRowIndent = 20.f;
    
    struct ColorsTree
    {
        uint color_tree_background;
        Scroll::ColorsScroll colorsScroll;

        ColorsTree(uint _color_tree_background = ZC_GUI_Colors::tree_background, const Scroll::ColorsScroll& _colorsScroll = Scroll::ColorsScroll(ZC_GUI_Colors::tree_scroll_background,
                    ZC_GUI_ButtonBase::ColorsButton(ZC_GUI_Colors::tree_scroll_caret, ZC_GUI_Colors::tree_scroll_caret_under_cursor, ZC_GUI_Colors::tree_scroll_caret_pressed)))
            : color_tree_background(_color_tree_background),
            colorsScroll(_colorsScroll)
        {}
    };

    ZC_GUI_Tree(float width, float height, const ColorsTree& colorsTree = {})
        : ZC_GUI_ObjBorder(ZC_GUI_ObjData(width, height, colorsTree.color_tree_background, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons), true, false, colorsTree.colorsScroll)
    {}

    bool operator == (ZC_ButtonID _buttonId) const noexcept
    {
        return ZC_ButtonID::K_F2 == _buttonId;  //  uses for rename
    }

    void AddBranch(ZC_GUI_Branch* pBranchAdd, ZC_GUI_Branch* pBranchAddTo)
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

    void AddBranches(const std::vector<ZC_GUI_Branch*> branchesAdd, ZC_GUI_Branch* pBranchAddTo)
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
                    for (ZC_GUI_Branch* pBranchAdd : branchesAdd)
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
            for (ZC_GUI_Branch* pBranchAdd : branchesAdd)
                if (pBranchAdd->isFirstGetDataCall) this->AddRow(Row(ZC_GUI_RowParams(0.f, ZC_GUI_RowParams::Left, 0.f, 0.f), { pBranchAdd }));
        }
    }

    void EraseBranch(ZC_GUI_Branch* pBranchDelete, bool reconfigure)
    {
        std::list<Row>::iterator iter_delete_start;
        std::list<Row>::iterator iter_delete_end;
        for (auto iter = rows.begin(); ; ++iter)      //  find branch
        {
            if (iter == rows.end()) return;     //  can't find branch
            ZC_GUI_Obj* pObj = iter->objs.front();
            if (pObj == pBranchDelete)    //  branch found
            {       //  restore obj's default state
                pObj->SetObjHolder(nullptr);    //  null branchs objGolder
                iter_delete_start = iter;
                iter_delete_end = iter_delete_start;
                break;
            }
        }
        if (pBranchDelete->VIsOpenable_Br())    //  branch openable, delete all sub-branches
        {
            float delete_branch_indent = iter_delete_end->rowParams.indent_x;
                //  if indent_x of current row is more then delete branch (row) indent, then object is in the deleted branch
            for (++iter_delete_end; iter_delete_end->rowParams.indent_x > delete_branch_indent || iter_delete_end != rows.end(); ++iter_delete_end)
                iter_delete_end->SetObjHolder(nullptr);     //  unset objHolder
        }
        iter_delete_start == iter_delete_end ? rows.erase(iter_delete_start) : rows.erase(iter_delete_start, iter_delete_end);
        if (reconfigure && this->VIsMutableWin_Obj() && this->VIsConfigured_Obj()) this->VConfigure_Obj();      //  reconfigure
    }

        //  return true if branch became active
    bool MakeActive(ZC_GUI_Branch* pBranch)
    {
        if (pBranch == pActiveBranch)
        {
            pBranch->SetButtonColor_BS(pBranch->colorsButton.color_button_pressed);
            return false;
        }
        if (pActiveBranch) pActiveBranch->SetButtonColor_BS(pActiveBranch->colorsButton.color_button);
        pActiveBranch = pBranch;
        if (pActiveBranch) pActiveBranch->SetButtonColor_BS(pActiveBranch->colorsButton.color_button_pressed);
        return true;
    }

    void ChangeOpenableBranchState(ZC_GUI_BranchOpenable* pBO)
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

private:
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
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

    void VKeyboardButtonUp_Obj(float time) override
    {
        if (!pActiveBranch) return;
        ZC_GUI_TextInputWindow::StartInputWindow(this->Get_bl_Obj()[0], pActiveBranch->Get_bl_Obj()[1], this->VGetWidth_Obj(), ZC_GUI_Branch::max_symbols,
            pActiveBranch->GetWStr_BMT(), { &ZC_GUI_Branch::UpdateName, pActiveBranch }, false);
    }

    bool VIsButtonKeyboard_Obj() override
    {
        return true;
    }

    void SetBranchBL(float parentBranch_indent_x, std::list<Row>::iterator& rRow_iter, ZC_Vec2<float>& rTL)
    {
        while (rRow_iter != rows.end() && parentBranch_indent_x < rRow_iter->rowParams.indent_x)
        {
            rRow_iter->CalculateObjs_bl(rTL, this->VGetWidth_Obj());
            ZC_GUI_Branch* pBranch = dynamic_cast<ZC_GUI_Branch*>(rRow_iter->objs.front());
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

    void ChangeSubBranchState(bool needOpen, std::list<Row>::iterator& rRow_iter, float parentBranch_indent_x, float& rOffset_y)
    {
        while (rRow_iter != rows.end() && parentBranch_indent_x < rRow_iter->rowParams.indent_x)
        {
            ZC_GUI_Branch* pBranch = dynamic_cast<ZC_GUI_Branch*>(rRow_iter->objs.front());
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
};




void ZC_GUI_Branch::VLeftButtonUp_BM(float time)
{
    if (dynamic_cast<ZC_GUI_Tree*>(this->pObjHolder)->MakeActive(this)) VBecameActive_Br();
}







void ZC_GUI_BranchOpenable::VLeftButtonDoubleClick_BM(float time)
{
    dynamic_cast<ZC_GUI_Tree*>(this->pObjHolder)->ChangeOpenableBranchState(this);
    arrowButton.ChangeArrow();
}


void ZC_GUI_BranchOpenable::ArrowButton::VLeftButtonUp_BM(float time)
{
    dynamic_cast<ZC_GUI_Tree*>(this->pObjHolder->pObjHolder)->ChangeOpenableBranchState(dynamic_cast<ZC_GUI_BranchOpenable*>(this->pObjHolder));
    ChangeArrow();
}