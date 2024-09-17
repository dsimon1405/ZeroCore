#include "ZC_GUI_ButtonKeyboard.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>

ZC_GUI_ButtonKeyboard::ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ColorsButton& _colorsButton)
    : ZC_GUI_ButtonKeyboard(_buttonId, width, height, _buttonFlags, ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI_ButtonKeyboard::ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV &uv, const ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uv, ZC_GUI_Bindings::bind_tex_Icons), _buttonFlags, _colorsButton),
    buttonId(_buttonId)
{}

bool ZC_GUI_ButtonKeyboard::operator == (ZC_ButtonID _buttonId) const noexcept
{
    return buttonId == _buttonId;
}

bool ZC_GUI_ButtonKeyboard::VIsButtonKeyboard_Obj()
{
    return true;
}

void ZC_GUI_ButtonKeyboard::VStopEventActivity_Obj()
{
    this->StopEventActivity_BS();   //  same in ZC_GUI_ButtonMouse
}

bool ZC_GUI_ButtonKeyboard::VKeyboardButtonDown_Obj(float time)
{
    if (this->bs_keyboardButton == BS_HoldUntilRelease) return false;  //  don't do anything while uses another button down event
    if (this->bs_mouseButton != BS_Released)
    {
        this->bs_keyboardButton = BS_HoldUntilRelease;
        return false;
    }
    if (this->bs_keyboardButton == BS_Released) //  first press
    {
        this->pObjData->color = colorsButton.color_button_pressed;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
        this->bs_keyboardButton = BS_Pressed;

        VKeyboardButtonDown_BK(time);
        if (this->buttonFlags & ZC_GUI_BF__MBLPress)
        {
            pressed_time += time;
            this->clock.Start();
        }
    }   //  pressed event
    else if (this->buttonFlags & ZC_GUI_BF__MBLPress && this->clock.Time<ZC_Nanoseconds>() >= waitPressLimit_nanosec)
    {
        pressed_time += time;
        if (pressed_time >= pressedInterval_nanosec)
        {
            VKeyboardButtonPressed_BK(time);
            pressed_time -= pressedInterval_nanosec;
        }
    }
    return true;
}

void ZC_GUI_ButtonKeyboard::VKeyboardButtonUp_Obj(float time)
{
    if (this->bs_keyboardButton == BS_Pressed)
    {
        this->pObjData->color = colorsButton.color_button;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
        VKeyboardButtonUp_BK(time);

        pressed_time = 0;
    }
    this->bs_keyboardButton = BS_Released;
}