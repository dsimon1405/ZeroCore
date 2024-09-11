#pragma once

#include "ZC_GUI_BranchSimple.h"

struct ZC_GUI_BranchOpenable : public ZC_GUI_BranchSimple
{
    struct ColorsBranchOpenable
    {
        ZC_GUI_BranchSimple::ColorsBranch colorsBranch;
        ZC_GUI_ButtonBase::ColorsButton colorsArrowButton;

        ColorsBranchOpenable(const ZC_GUI_BranchSimple::ColorsBranch& _colorsBranch = {}, const ZC_GUI_ButtonBase::ColorsButton& _colorsArrowButton =
                ZC_GUI_ButtonBase::ColorsButton(ZC_GUI_Colors::tree_branch_arrow, ZC_GUI_Colors::tree_branch_arrow_under_cursor, ZC_GUI_Colors::tree_branch_arrow_pressed));
    };

    struct ArrowButton : public ZC_GUI_ButtonMouse
    {
        ArrowButton(float size, bool isOpen, const ZC_GUI_ButtonBase::ColorsButton& colorsArrowButton);

        bool IsOpen();
        void ChangeArrow();
        void VLeftButtonUp_BM(float time) override;
    };

    ArrowButton arrowButton;

    ZC_GUI_BranchOpenable(const std::wstring& name, bool isOpen, const ColorsBranchOpenable& branchOpenableColors = {});

    ZC_GUI_BranchOpenable(ZC_GUI_BranchOpenable&& bo);

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VLeftButtonDoubleClick_BM(float time) override;
    bool VIsOpenable_Br() override;
    bool VIsOpen_Br() override;
    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
};

typedef typename ZC_GUI_BranchOpenable::ColorsBranchOpenable ZC_GUI_ColorsBranchOpenable;