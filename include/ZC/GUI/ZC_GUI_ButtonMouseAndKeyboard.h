#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouse.h>
#include <ZC/GUI/ZC_GUI_ButtonKeyboard.h>

struct ZC_GUI_ButtonMouseAndKeyboard : public ZC_GUI_ButtonMouse, public ZC_GUI_ButtonKeyboard
{
    ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ColorsButton& _colorsbutton = {});
    ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ZC_GUI_UV &uv, const ColorsButton& _colorsbutton = {});

    bool VIsDrawing_Obj() const noexcept override;
    void VStopEventActivity_Obj() override;
};