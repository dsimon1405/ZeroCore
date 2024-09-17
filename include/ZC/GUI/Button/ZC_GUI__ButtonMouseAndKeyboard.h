#pragma once

#include "ZC_GUI__Button.h"
#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseAndKeyboard.h>

struct ZC_GUI__BMAK : public ZC_GUI_ButtonMouseAndKeyboard
{
    ZC_GUI__Button<ZC_GUI__BMAK>* pHolder;

    ZC_GUI__BMAK(ZC_GUI__Button<ZC_GUI__BMAK>* _pBMAK, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ZC_GUI_UV &uv, const ZC_GUI_ColorsButton& _colorsbutton);

    void VScroll_Obj(float vertical, float time) override;
    void VCursorMove_Obj(float rel_x, float rel_y) override;
    void VLeftButtonDown_BM(float time) override;
    void VLeftButtonDoubleClick_BM(float time) override;
    void VLeftButtonPressed_BM(float time) override;
    void VLeftButtonUp_BM(float time) override;
    void VKeyboardButtonDown_BK(float time) override;
    void VKeyboardButtonUp_BK(float time) override;
    void VKeyboardButtonPressed_BK(float time) override;
};

/*
Creates button with mouse evetns.
May be override:
- void VScroll(float vertical, float time) {}
- void VCursorMove(float rel_x, float rel_y) {}
- void VLeftButtonDown(float time) {}
- void VLeftButtonDoubleClick(float time) {}
- void VLeftButtonPressed(float time) {}
- void VLeftButtonUp(float time) {}
- void VKeyboardButtonDown(float time) {}
- void VKeyboardButtonUp(float time) {}
- void VKeyboardButtonPressed(float time) {}
*/
class ZC_GUI__ButtonMouseAndKeyboard : public ZC_GUI__Button<ZC_GUI__BMAK>
{
public:
    /*
    Params:
    - width - width in pixels.
    - height - height in pixels.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ZC_GUI_ColorsButton& _colorsButton = {});

    /*
    Params:
    - width - width in pixels.
    - height - height in pixels.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - uv - custom uv if uses custom icons texture (not default).
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ZC_GUI_UV &uv, const ZC_GUI_ColorsButton& _colorsButton = {});
};