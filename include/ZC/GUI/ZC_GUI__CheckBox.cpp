#include "ZC_GUI__CheckBox.h"

ZC_GUI__CheckBox::ZC_GUI__CheckBox(const std::wstring& name, bool _isOn, ZC_Function<void(bool)> _callback, const ZC_GUI_ColorsCheckBox& colorsCheckBox)
    : ZC_GUI__Obj<ZC_GUI_CheckBox>(ZC_GUI_CheckBox(name, _isOn, std::move(_callback), colorsCheckBox))
{}

bool ZC_GUI__CheckBox::GetState()
{
    return this->obj.GetState_ChB();
}

void ZC_GUI__CheckBox::ChangeState(bool use_callback)
{
    this->obj.ChangeState_ChB(use_callback);
}