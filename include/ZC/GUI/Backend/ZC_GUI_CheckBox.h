#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseText.h>
#include <ZC/GUI/Backend/ZC_GUI_Colors.h>

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

    ZC_GUI_CheckBox(const std::wstring& name, bool _isOn, const ColorsCheckBox& colorsCheckBox = {});

    ZC_GUI_CheckBox(ZC_GUI_CheckBox&& chB);

    /*
    Overrides in heir to get check box changes.

    Params:
    - state - if true check box is on, otherwise off.
    */
    virtual void VStateChanged_CB(bool state)
    //  = 0;
    {}
private:
    bool isOn;
    ZC_GUI_Obj objArrow;
    
    void VSetDrawState_Obj(bool neeDraw, bool updateGPU) override;
    void VLeftButtonUp_BM(float time) override;
};

typedef typename ZC_GUI_CheckBox::ColorsCheckBox ZC_GUI_ColorsCheckBox;