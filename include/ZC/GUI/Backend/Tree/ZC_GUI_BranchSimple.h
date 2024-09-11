#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseText.h>

struct ZC_GUI_BranchSimple : public ZC_GUI_ButtonMouseText
{
    static inline int max_symbols = 20;   //  max symbols in text of each branch

    struct ColorsBranch
    {
        uint color_text = ZC_GUI_Colors::tree_branch_text;
        ZC_GUI_ButtonBase::ColorsButton colorsBranchButton;

        ColorsBranch(uint _color_text = ZC_GUI_Colors::tree_branch_text, const ZC_GUI_ButtonBase::ColorsButton& _colorsBranchButton =
                ZC_GUI_ButtonBase::ColorsButton(ZC_GUI_Colors::tree_branch_button, ZC_GUI_Colors::tree_branch_button_under_cursor, ZC_GUI_Colors::tree_branch_button_pressed));
    };

    ZC_GUI_BranchSimple(const std::wstring& name, const ColorsBranch& colorsBranch = {});

    ZC_GUI_BranchSimple(ZC_GUI_BranchSimple&& bs);

    virtual void VBecameActive_Br() {}

    void VLeftButtonUp_BM(float time) override;
    virtual bool VIsOpenable_Br();
    virtual bool VIsOpen_Br();
    void UpdateName(const std::wstring& name);
    static void SetMaxSymbolsCount(int count);
};

typedef typename ZC_GUI_BranchSimple::ColorsBranch ZC_GUI_ColorsBranch;