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

    ZC_GUI_CheckBox(const std::wstring& name, bool _isOn, ZC_Function<void(bool)> _callback, const ColorsCheckBox& colorsCheckBox = {});

    ZC_GUI_CheckBox(ZC_GUI_CheckBox&& chB);
        //  Return chaeck box state.
    bool GetState_ChB();
        //  Changes check box state. If use_callback true, use callback.
    void ChangeState_ChB(bool use_callback);
    
private:
    bool isOn;
    ZC_GUI_Obj objArrow;
    ZC_Function<void(bool)> callback;
    
    void VSetDrawState_Obj(bool neeDraw, bool updateGPU) override;
    void VLeftButtonUp_BM(float time) override;
};

typedef typename ZC_GUI_CheckBox::ColorsCheckBox ZC_GUI_ColorsCheckBox;