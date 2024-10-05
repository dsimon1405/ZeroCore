#include "ZC_GUI_ButtonBase.h"

#include <ZC/Tools/Math/ZC_Math.h>

ZC_GUI_ButtonBase::ZC_GUI_ButtonBase(const ZC_GUI_ObjData& od, ZC_GUI_ButtonFlags _buttonFlags, const ColorsButton& _colorsButton)
    : ZC_GUI_ObjComposite(od),
    colorsButton(_colorsButton),
    buttonFlags(_buttonFlags)
{
    this->pObjData->color = colorsButton.color_button;
}

void ZC_GUI_ButtonBase::SetWaitPressLimit_BS(long _waitPressLimit_nanosec)
{
    if (_waitPressLimit_nanosec > 0) waitPressLimit_nanosec = _waitPressLimit_nanosec;
}

void ZC_GUI_ButtonBase::SetPressedInterval_BS(long _pressedInterval_nanosec)
{
    if (_pressedInterval_nanosec > 0) pressedInterval_nanosec = _pressedInterval_nanosec;
}

void ZC_GUI_ButtonBase::StopEventActivity_BS()
{
    if (bs_mouseButton == BS_Pressed) bs_mouseButton = BS_HoldUntilRelease;
    if (bs_keyboardButton == BS_Pressed) bs_keyboardButton = BS_HoldUntilRelease;
    if (this->pObjData->color == colorsButton.color_button) return;     //  nothing to change
    this->pObjData->color = colorsButton.color_button;
    VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
}

void ZC_GUI_ButtonBase::VChangeObjectActivity_Obj(bool _isAvtive, bool changeGPU)
{
    if (isButtonActive == _isAvtive) return;
    isButtonActive = _isAvtive;
    if (isButtonActive)
    {
        bs_mouseButton = BS_Released;
        bs_keyboardButton = BS_Released;
        this->SetColor_Obj(colorsButton.color_button, false);
    }
    else this->SetColor_Obj(colorsButton.color_not_active, false);

    for (ZC_GUI_Obj* pObj : this->objs) pObj->VChangeObjectActivity_Obj(_isAvtive, changeGPU);
    if (changeGPU) VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData) * this->objs.size() + sizeof(ZC_GUI_ObjData::color),
        &(this->pObjData->color));
}


    //  ColorsButton
ZC_GUI_ButtonBase::ColorsButton::ColorsButton(uint _color_button, uint _color_button_under_cursor, uint _color_button_pressed, uint _color_not_active)
    : color_button(_color_button),
    color_button_under_cursor(_color_button_under_cursor),
    color_button_pressed(_color_button_pressed),
    color_not_active(_color_not_active)
{}