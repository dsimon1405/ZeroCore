#pragma once

#include "ZC_GUI_ButtonMouse.h"
#include "ZC_GUI_ButtonKeyboard.h"

struct ZC_GUI_ButtonMouseAndKeyboard : public ZC_GUI_ButtonMouse, public ZC_GUI_ButtonKeyboard
{
    ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ColorsButton& _colorsButton = {});
    ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ZC_GUI_UV &uv, const ColorsButton& _colorsButton = {});

    bool VIsDrawing_Obj() const noexcept override;
    void VStopEventActivity_Obj() override;
};