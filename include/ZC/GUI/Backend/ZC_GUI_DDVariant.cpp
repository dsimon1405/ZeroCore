#include "ZC_GUI_DDVariant.h"

#include <ZC/GUI/ZC__GUI.h>

ZC_GUI_ColorsDropDown::ZC_GUI_ColorsDropDown(const ZC_GUI_ColorsButton& _colorsButton, uint _color_text, uint _color_arrow)
    : colorsButton(_colorsButton),
    color_text(_color_text),
    color_arrow(_color_arrow)
{}


ZC_GUI_DropDownIcon::ZC_GUI_DropDownIcon(uint color)
    : ZC_GUI_Obj(GetWidth(), ZC__GUI::GetFontHeight() / 3.f, 0.f, color, ZC_GUI_IconUV::arrow_down, 0, ZC_GUI_Bindings::bind_tex_Icons)
{}

float ZC_GUI_DropDownIcon::GetTextIndentX()
{
    static float indent = 0.f;
    if (indent == 0.f) indent = ZC_GUI_TextManager::GetFontHeight() / 5.f;
    return indent;
}

float ZC_GUI_DropDownIcon::GetWidth()
{
    return ZC__GUI::GetFontHeight() * 0.6f;
}
