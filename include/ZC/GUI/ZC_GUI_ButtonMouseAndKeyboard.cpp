#include <ZC/GUI/ZC_GUI_ButtonMouseAndKeyboard.h>

#include "ZC_GUI_IconUV.h"

ZC_GUI_ButtonMouseAndKeyboard::ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_MB__Flags _mb_flags, ZC_ButtonID _buttonId, bool useKeyboardButtonPress)
    : ZC_GUI_ButtonMouseAndKeyboard(width, height, _mb_flags, _buttonId, useKeyboardButtonPress, ZC_GUI_IconUV::button)
{}

ZC_GUI_ButtonMouseAndKeyboard::ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_MB__Flags _mb_flags, ZC_ButtonID _buttonId, bool useKeyboardButtonPress, const ZC_GUI_UV &uv)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData{ .width = width, .height = height, .uv = uv }),
    ZC_GUI_ButtonMouse(width, height, _mb_flags, uv),
    ZC_GUI_ButtonKeyboard(_buttonId, width, height, useKeyboardButtonPress, uv)
{}

bool ZC_GUI_ButtonMouseAndKeyboard::VIsDrawing_Obj() const noexcept
{
    return true;
}

void ZC_GUI_ButtonMouseAndKeyboard::VStopEventActivity_Obj()
{
    this->StopEventActivity_BS();   //  same in ZC_GUI_ButtonKeyboard
}