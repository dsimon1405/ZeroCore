#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonKeyboard.h>
#include "ZC_GUI__Button.h"

class ZC_GUI__ButtonKeyboard;

struct ZC_GUI__BK : public ZC_GUI_ButtonKeyboard
{
    ZC_GUI__ButtonKeyboard* pBK;

    ZC_GUI__BK(ZC_GUI__ButtonKeyboard* _pBK, ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ZC_GUI_ColorsButton& _colorsButton);

    void VKeyboardButtonDown_BK(float time) override;
    void VKeyboardButtonUp_BK(float time) override;
    void VKeyboardButtonPressed_BK(float time) override;
};

/*
Creates button with keyboard key evetns.
May be override:
- void VKeyboardButtonDown(float time) {}
- void VKeyboardButtonUp(float time) {}
- void VKeyboardButtonPressed(float time) {}
*/
class ZC_GUI__ButtonKeyboard : public ZC_GUI__Button<ZC_GUI__BK>
{
public:
    /*
    Params:
    - _buttonId - button id for call VKeyboardButton... methods on events.
    - width - pixel width.
    - height - pixel heihgt.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - uv - button uv coords from custom texture.
    - _colorsButton - button colors.
    */
    ZC_GUI__ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ZC_GUI_ColorsButton& _colorsButton = {});
    /*
    Params:
    - _buttonId - button id for call VKeyboardButton... methods on events.
    - width - pixel width.
    - height - pixel heihgt.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - _colorsButton - button colors.
    */
    ZC_GUI__ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_ColorsButton& _colorsButton = {});
};