#include "ZC_GUI__ButtonMouse.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>

ZC_GUI__ButtonMouse::ZC_GUI__ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ButtonMouse(width, height, _buttonFlags, ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI__ButtonMouse::ZC_GUI__ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__Button<ZC_GUI__BM<ZC_GUI_ButtonMouse>>(ZC_GUI__BM<ZC_GUI_ButtonMouse>(this, ZC_GUI_ButtonMouse(width, height, _buttonFlags, uv, _colorsButton)))
{}