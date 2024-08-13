#include <ZC/GUI/ZC_GUI_ButtonMouseAndKeyboard.h>

#include "ZC_GUI_IconUV.h"
#include <ZC/GUI/ZC_GUI_Bindings.h>

ZC_GUI_ButtonMouseAndKeyboard::ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ColorsButton& _colorsbutton)
    : ZC_GUI_ButtonMouseAndKeyboard(width, height, _buttonFlags, _buttonId, ZC_GUI_IconUV::button, _colorsbutton)
{}

ZC_GUI_ButtonMouseAndKeyboard::ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ZC_GUI_UV &uv, const ColorsButton& _colorsbutton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uv, ZC_GUI_Bindings::bind_tex_Icons), _buttonFlags, _colorsbutton),
    ZC_GUI_ButtonMouse(width, height, _buttonFlags),
    ZC_GUI_ButtonKeyboard(_buttonId, width, height, _buttonFlags)
{}

bool ZC_GUI_ButtonMouseAndKeyboard::VIsDrawing_Obj() const noexcept
{
    return true;
}

void ZC_GUI_ButtonMouseAndKeyboard::VStopEventActivity_Obj()
{
    this->StopEventActivity_BS();   //  same in ZC_GUI_ButtonKeyboard
}