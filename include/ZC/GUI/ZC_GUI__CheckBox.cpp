#include "ZC_GUI__CheckBox.h"

    //  ZC_GUI__ChB

ZC_GUI__ChB::ZC_GUI__ChB(ZC_GUI__CheckBox* _pHolder, ZC_GUI_CheckBox&& checkBox)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(checkBox)),
    ZC_GUI_CheckBox(std::move(checkBox)),
    pHolder(_pHolder)
{}

void ZC_GUI__ChB::VStateChanged_CB(bool state)
{
    pHolder->VStateChanged(state);
}


    //  ZC_GUI__CheckBox

ZC_GUI__CheckBox::ZC_GUI__CheckBox(const std::wstring& name, bool _isOn, const ZC_GUI_ColorsCheckBox& colorsCheckBox)
    : ZC_GUI__Obj<ZC_GUI__ChB>(ZC_GUI__ChB(this, ZC_GUI_CheckBox(name, _isOn, colorsCheckBox)))
{}