#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouseText.h>

#include <ZC/GUI/ZC_GUI_IconUV.h>
#include <ZC/GUI/ZC_GUI.h>

#include <iostream>
struct ZC_GUI_CheckBox : public ZC_GUI_ButtonMouseText
{
    ZC_GUI_CheckBox(const std::wstring& name, bool isOn)
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData((float)ZC_GUI::pGUI->textManager.GetFontHeight(), (float)ZC_GUI::pGUI->textManager.GetFontHeight(), 0,
            isOn ? ZC_GUI_IconUV::checkBox : ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None),
        ZC_GUI_ButtonMouseText(ZC_GUI::pGUI->textManager.GetFontHeight(), ZC_GUI::pGUI->textManager.GetFontHeight(), ZC_GUI_BF__None,
            ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(ZC_GUI::pGUI->textManager.GetFontHeight() * 0.5f, ZC_GUI_TextForButton::Indent::OutOfButton), name, true, 0),
            isOn ? ZC_GUI_IconUV::checkBox : ZC_GUI_IconUV::button)
    {}

    /*
    Overrides in heir to get check box changes.

    Params:
    - state - if true check box is on, otherwise off.
    */
    virtual void VChangeState_CB(bool state)
        { std::cout<<"state "<<(state ? "ON" : "OFF")<<std::endl; }
    // = 0;

private:
    void VLeftButtonUp_BM(float time) override
    {
        bool new_state = ZC_GUI_IconUV::button.bl == this->pObjData->uv.bl;
        this->pObjData->uv = new_state ? ZC_GUI_IconUV::checkBox : ZC_GUI_IconUV::button;
        VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, uv), sizeof(ZC_GUI_ObjData::uv), &(this->pObjData->uv));
        VChangeState_CB(new_state);
    }
};