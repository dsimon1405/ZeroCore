#pragma once

#include "ZC_GUI_BranchOpenable.h"
#include <ZC/GUI/Backend/ZC_GUI_ObjBorder.h>

struct ZC_GUI_Tree : public ZC_GUI_ObjBorder
{
    ZC_GUI_BranchSimple* pActiveBranch = nullptr;
    static inline const float newRowIndent = 20.f;
    
    struct ColorsTree
    {
        uint color_tree_background;
        Scroll::ColorsScroll colorsScroll;

        ColorsTree(uint _color_tree_background = ZC_GUI_Colors::tree_background, const Scroll::ColorsScroll& _colorsScroll = Scroll::ColorsScroll(ZC_GUI_Colors::tree_scroll_background,
                    ZC_GUI_ButtonBase::ColorsButton(ZC_GUI_Colors::tree_scroll_caret, ZC_GUI_Colors::tree_scroll_caret_under_cursor, ZC_GUI_Colors::tree_scroll_caret_pressed)));
    };

    ZC_GUI_Tree(float width, float height, const ColorsTree& colorsTree = {});

    bool operator == (ZC_ButtonID _buttonId) const noexcept;
    void AddBranch(ZC_GUI_BranchSimple* pBranchAdd, ZC_GUI_BranchSimple* pBranchAddTo);
    void AddBranches(const std::vector<ZC_GUI_BranchSimple*>& branchesAdd, ZC_GUI_BranchSimple* pBranchAddTo);
    void EraseBranch(ZC_GUI_BranchSimple* pBranchErase);
    void EraseBranches(const std::vector<ZC_GUI_BranchSimple*>& branchesErase);
        //  return true if branch became active
    bool MakeActive(ZC_GUI_BranchSimple* pBranch);
    void ChangeOpenableBranchState(ZC_GUI_BranchOpenable* pBO);

private:
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VKeyboardButtonUp_Obj(float time) override;
    bool VIsButtonKeyboard_Obj() override;
    void SetBranchBL(float parentBranch_indent_x, std::list<Row>::iterator& rRow_iter, ZC_Vec2<float>& rTL);
    void ChangeSubBranchState(bool needOpen, std::list<Row>::iterator& rRow_iter, float parentBranch_indent_x, float& rOffset_y);
    void BranchErase(ZC_GUI_BranchSimple* pBranchErase);
};

typedef typename ZC_GUI_Tree::ColorsTree ZC_GUI_ColorsTree;