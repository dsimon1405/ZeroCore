#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouse.h>
#include <ZC/GUI/ZC_GUI_ButtonKeyboard.h>

struct ZC_GUI_ButtonMouseAndKeyboard : public ZC_GUI_ButtonMouse, public ZC_GUI_ButtonKeyboard
{
    ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_MB__Flags _mb_flags, ZC_ButtonID _buttonId, bool useKeyboardButtonPress);
    ZC_GUI_ButtonMouseAndKeyboard(float width, float height, ZC_GUI_MB__Flags _mb_flags, ZC_ButtonID _buttonId, bool useKeyboardButtonPress, const ZC_GUI_UV &uv);

    bool VIsDrawing_Obj() const noexcept override;
    void VStopEventActivity_Obj() override;
};