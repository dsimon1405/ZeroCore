#include "ZC_GUI_CheckBox.h"

#include <ZC/GUI/Backend/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/ZC_GUI_Bindings.h>

ZC_GUI_CheckBox::ZC_GUI_CheckBox(const std::wstring& name, bool _isOn, const ColorsCheckBox& colorsCheckBox)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData((float)ZC_GUI_TextManager::GetFontHeight(), (float)ZC_GUI_TextManager::GetFontHeight(), 0,
        ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, colorsCheckBox.colorsButton),
    ZC_GUI_ButtonMouseText(0, 0, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(ZC_GUI_TextManager::GetFontHeight() * 0.5f, ZC_GUI_TextForButton::Indent::OutOfButton),
            name, true, 0, ZC_GUI_TextAlignment::Left, colorsCheckBox.color_text)),
    isOn(_isOn),
    objArrow(ZC_GUI_ObjData(this->VGetWidth_Obj(), this->GetHeight(), colorsCheckBox.color_arrow, ZC_GUI_IconUV::arrow_down, ZC_GUI_Bindings::bind_tex_Icons))
{
    this->VAddObj_Obj(&objArrow, nullptr);
    if (!isOn) objArrow.VSetDrawState_Obj(false, false);
}

        #include <ZC/Tools/Time/ZC_Clock.h>
        #include <iostream>
ZC_GUI_CheckBox::ZC_GUI_CheckBox(ZC_GUI_CheckBox&& chB)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(chB)),
    ZC_GUI_ButtonMouseText(static_cast<ZC_GUI_ButtonMouseText&&>(chB)),
    isOn(chB.isOn),
    objArrow(static_cast<ZC_GUI_Obj&&>(chB.objArrow))
{
    this->VAddObj_Obj(&objArrow, nullptr);
    if (!isOn) objArrow.VSetDrawState_Obj(false, false);
}

void ZC_GUI_CheckBox::VSetDrawState_Obj(bool neeDraw, bool updateGPU)
{
    if (neeDraw == VIsDrawing_Obj()) return;
    pObjData->height = neeDraw ? actual_height : 0.f;

    this->textForButton.VSetDrawState_Obj(neeDraw, false);

    objArrow.VSetDrawState_Obj(neeDraw ? isOn : false, false);    //  if checkbox start drawing, start drawing arrow only if state is ON

    if (updateGPU) VSubDataObjData_Obj(this->pObjData, objs.back()->pObjData);
}

void ZC_GUI_CheckBox::VLeftButtonUp_BM(float time)
{
    isOn = !isOn;
    objArrow.VSetDrawState_Obj(isOn, true);
    VStateChanged_CB(isOn);
}


    //  ZC_GUI_CheckBox::ColorsCheckBox

ZC_GUI_CheckBox::ColorsCheckBox::ColorsCheckBox(uint _color_arrow, uint _color_text, const ColorsButton& _colorsButton)
    : color_arrow(_color_arrow),
    color_text(_color_text),
    colorsButton(_colorsButton)
{}