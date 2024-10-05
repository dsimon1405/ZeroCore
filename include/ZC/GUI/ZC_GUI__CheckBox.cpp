#include "ZC_GUI__CheckBox.h"

ZC_GUI__CheckBox::ZC_GUI__CheckBox(ZC_GUI_TFB_Indent_Location name_pos, const std::wstring& name, bool _isOn, ZC_Function<void(bool)>&& _callback_state_changed,
        ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsCheckBox& colorsCheckBox, bool immutable_name,
        int reserve_name_width, ZC_GUI_TextAlignment name_alignment)
    : ZC_GUI__Obj<ZC_GUI_CheckBox>(ZC_GUI_CheckBox(name_pos, name, _isOn, std::move(_callback_state_changed), std::move(_callback_focuse_changed),
        colorsCheckBox, immutable_name, reserve_name_width, name_alignment)),
    callback_focus_changed(std::move(_callback_focuse_changed))
{}

bool ZC_GUI__CheckBox::GetState()
{
    return this->obj.GetState_ChB();
}

void ZC_GUI__CheckBox::ChangeState(bool use_callback)
{
    this->obj.ChangeState_ChB(use_callback);
}

bool ZC_GUI__CheckBox::UpdateText(const std::wstring& wstr, bool brootForceUpdate)
{
    return this->obj.UpdateText_BMT(wstr, brootForceUpdate);
}