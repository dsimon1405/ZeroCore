#pragma once

#include "ZC_GUI__BM.h"
#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouse.h>

/*
Creates button with mouse evetns.
May be override:
- void VScroll(float vertical, float time) {}
- void VCursorMove(float rel_x, float rel_y) {}
- void VLeftButtonDown(float time) {}
- void VLeftButtonDoubleClick(float time) {}
- void VLeftButtonPressed(float time) {}
- void VLeftButtonUp(float time) {}
- void VFocuseChanged(bool isFocused) {};
*/
class ZC_GUI__ButtonMouse : public ZC_GUI__Button<ZC_GUI__BM<ZC_GUI_ButtonMouse>>
{
public:
    /*
    Params:
    - width - width in pixels.
    - height - height in pixels.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_ColorsButton& _colorsButton = {});

    /*
    Params:
    - width - width in pixels.
    - height - height in pixels.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - uv - custom uv if uses custom icons texture (not default).
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ZC_GUI_ColorsButton& _colorsButton = {});
};