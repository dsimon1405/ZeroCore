#include "ZC_GUI__ButtonKeyboard.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>

    //  ZC_GUI__BK

ZC_GUI__BK::ZC_GUI__BK(ZC_GUI__Button<ZC_GUI__BK>* _pHolder, ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uv, ZC_GUI_Bindings::bind_tex_Icons), _buttonFlags, _colorsButton),
    ZC_GUI_ButtonKeyboard(_buttonId, width, height, _buttonFlags, colorsButton),
    pHolder(_pHolder)
{}

void ZC_GUI__BK::VKeyboardButtonDown_BK(float time)
{
    pHolder->VKeyboardButtonDown(time);
}

void ZC_GUI__BK::VKeyboardButtonUp_BK(float time)
{
    pHolder->VKeyboardButtonUp(time);
}

void ZC_GUI__BK::VKeyboardButtonPressed_BK(float time)
{
    pHolder->VKeyboardButtonPressed(time);
}


    //  ZC_GUI__ButtonKeyboard

ZC_GUI__ButtonKeyboard::ZC_GUI__ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ButtonKeyboard(_buttonId, width, height, _buttonFlags, ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI__ButtonKeyboard::ZC_GUI__ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__Button(ZC_GUI__BK(this, _buttonId, width, height, _buttonFlags, uv, _colorsButton))
{}