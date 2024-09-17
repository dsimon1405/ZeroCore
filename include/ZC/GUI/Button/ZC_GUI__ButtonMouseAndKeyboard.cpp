#include "ZC_GUI__ButtonMouseAndKeyboard.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>

    //  ZC_GUI__BMAK

ZC_GUI__BMAK::ZC_GUI__BMAK(ZC_GUI__Button<ZC_GUI__BMAK>* _pHolder, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ZC_GUI_UV &uv, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uv, ZC_GUI_Bindings::bind_tex_Icons), _buttonFlags, _colorsButton),
    ZC_GUI_ButtonMouseAndKeyboard(width, height, _buttonFlags, _buttonId, uv, _colorsButton),
    pHolder(_pHolder)
{}

void ZC_GUI__BMAK::VScroll_Obj(float vertical, float time)
{
    pHolder->VScroll(vertical, time);
}

void ZC_GUI__BMAK::VCursorMove_Obj(float rel_x, float rel_y)
{
    pHolder->VCursorMove(rel_x, rel_y);
}

void ZC_GUI__BMAK::VLeftButtonDown_BM(float time)
{
    pHolder->VLeftButtonDown(time);
}

void ZC_GUI__BMAK::VLeftButtonDoubleClick_BM(float time)
{
    pHolder->VLeftButtonDoubleClick(time);
}

void ZC_GUI__BMAK::VLeftButtonPressed_BM(float time)
{
    pHolder->VLeftButtonPressed(time);
}

void ZC_GUI__BMAK::VLeftButtonUp_BM(float time)
{
    pHolder->VLeftButtonUp(time);
}

void ZC_GUI__BMAK::VKeyboardButtonDown_BK(float time)
{
    pHolder->VKeyboardButtonDown(time);
}

void ZC_GUI__BMAK::VKeyboardButtonUp_BK(float time)
{
    pHolder->VKeyboardButtonUp(time);
}

void ZC_GUI__BMAK::VKeyboardButtonPressed_BK(float time)
{
    pHolder->VKeyboardButtonPressed(time);
}


    //  ZC_GUI__ButtonMouseAndKeyboard

ZC_GUI__ButtonMouseAndKeyboard::ZC_GUI__ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ButtonMouseAndKeyboard(width, height, _buttonFlags, _buttonId, ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI__ButtonMouseAndKeyboard::ZC_GUI__ButtonMouseAndKeyboard(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_ButtonID _buttonId, const ZC_GUI_UV &uv, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__Button<ZC_GUI__BMAK>(ZC_GUI__BMAK(this, width, height, _buttonFlags, _buttonId, uv, _colorsButton))
{}