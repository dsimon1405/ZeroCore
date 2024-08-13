#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouseText.h>
#include <ZC/GUI/ZC_GUI_Colors.h>

#include <ZC/GUI/ZC_GUI_IconUV.h>
#include <ZC/GUI/ZC_GUI.h>

#include <iostream>
struct ZC_GUI_CheckBox : public ZC_GUI_ButtonMouseText
{
    struct ColorsCheckBox
    {
        ZC_GUI_ButtonBase::ColorsButton colorsButton;
        uint color_arrow;
        uint color_text;

        ColorsCheckBox(const ZC_GUI_ButtonBase::ColorsButton& _colorsButton = {}, uint _color_arrow = ZC_GUI_Colors::text, uint _color_text = ZC_GUI_Colors::text)
            : colorsButton(_colorsButton),
            color_arrow(_color_arrow),
            color_text(_color_text)
        {}
    };

    ZC_GUI_CheckBox(const std::wstring& name, bool _isOn, const ColorsCheckBox& colorsCheckBox = {})
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData((float)ZC_GUI::pGUI->textManager.GetFontHeight(), (float)ZC_GUI::pGUI->textManager.GetFontHeight(), 0,
            ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, colorsCheckBox.colorsButton),
        ZC_GUI_ButtonMouseText(0, 0, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(ZC_GUI::pGUI->textManager.GetFontHeight() * 0.5f, ZC_GUI_TextForButton::Indent::OutOfButton),
                name, true, 0, ZC_GUI_TextAlignment::Left, colorsCheckBox.color_text)),
        isOn(_isOn),
        objArrow(ZC_GUI_ObjData(this->VGetWidth_Obj(), this->GetHeight(), colorsCheckBox.color_arrow, ZC_GUI_IconUV::arrowDown, ZC_GUI_Bindings::bind_tex_Icons))
    {
        this->VAddObj_Obj(&objArrow, nullptr);
        if (!isOn) objArrow.VSetDrawState_Obj(false, false);
    }

    /*
    Overrides in heir to get check box changes.

    Params:
    - state - if true check box is on, otherwise off.
    */
    virtual void VChangeState_CB(bool state)
        { std::cout<<"state "<<(state ? "ON" : "OFF")<<std::endl; }
    // = 0;

private:
    bool isOn;
    ZC_GUI_Obj objArrow;
    
    void VSetDrawState_Obj(bool neeDraw, bool updateGPU) override
    {
        if (neeDraw == VIsDrawing_Obj()) return;
        pObjData->height = neeDraw ? actual_height : 0.f;

        this->textForButton.VSetDrawState_Obj(neeDraw, false);

        objArrow.VSetDrawState_Obj(neeDraw ? isOn : false, false);    //  if checkbox start drawing, start drawing arrow only if state is ON

        if (updateGPU) VSubDataObjData_Obj(this->pObjData, objs.back()->pObjData);
    }

    void VLeftButtonUp_BM(float time) override
    {
        isOn = !isOn;
        objArrow.VSetDrawState_Obj(isOn, true);
        VChangeState_CB(isOn);
    }
};