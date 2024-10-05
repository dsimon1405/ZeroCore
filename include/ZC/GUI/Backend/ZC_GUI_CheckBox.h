#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseText.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Colors.h>
#include <ZC/Tools/Function/ZC_Function.h>

struct ZC_GUI_CheckBox : public ZC_GUI_ButtonMouseText
{
    struct ColorsCheckBox
    {
        uint color_arrow;
        uint color_text;
        ZC_GUI_ButtonBase::ColorsButton colorsButton;

        ColorsCheckBox(uint _color_arrow = ZC_GUI_Colors::checkBox_arrow, uint _color_text = ZC_GUI_Colors::checkBox_text, const ColorsButton& _colorsButton =
                ColorsButton(ZC_GUI_Colors::checkBox_button, ZC_GUI_Colors::checkBox_button_under_cursor, ZC_GUI_Colors::checkBox_button_pressed));
    };

    /*
    Params:
    - name_pos - position of the name relative to the button. May be only OutOfButtonRight or OutOfButtonLeft, trying to set other will set OutOfButtonRight.
    - name - checkbox name.
    - _isOn - start state.
    - _callback - function called when state changed.
    - _callback_focuse_changed - the function is called when the focus change. If true - in focus.
    - colorsCheckBox - colors.
    - immutable_name - must be false to change name after creation.
    - reserve_name_width - width in pixels of the longest name (ignores if immutable_name true)
    - name_alignment - name alignment in the texture if the name width in pixels is less then reserve_name_vidth.
    */
    ZC_GUI_CheckBox(ZC_GUI_TFB_Indent_Location name_pos, const std::wstring& name, bool _isOn, ZC_Function<void(bool)>&& _callback_state_changed,
        ZC_Function<void(bool)>&& _callback_focuse_changed, const ColorsCheckBox& colorsCheckBox = {}, bool immutable_name = true, int reserve_name_width = 0,
        ZC_GUI_TextAlignment name_alignment = ZC_GUI_TextAlignment::Left);

    ZC_GUI_CheckBox(ZC_GUI_CheckBox&& chB);
        //  Return chaeck box state.
    bool GetState_ChB();
        //  Changes check box state. If use_callback true, use callback.
    void ChangeState_ChB(bool use_callback);
        //  Update callbacks. Usefull if class from callback_number_chaned was moved and class have new address.
    void UpdateCallbacks(ZC_Function<void(bool)>&& _callback_state_changed, ZC_Function<void(bool)>&& _callback_focus_changed);
    
private:
    bool isOn;
    ZC_GUI_Obj objArrow;
    ZC_Function<void(bool)> callback_state_changed;
    ZC_Function<void(bool)> callback_focus_changed;
    
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VSetDrawState_Obj(bool neeDraw, bool updateGPU) override;
    void VLeftButtonUp_BM(float time) override;
    void VFocuseChanged_BM(bool isFocused) override;
};

typedef typename ZC_GUI_CheckBox::ColorsCheckBox ZC_GUI_ColorsCheckBox;