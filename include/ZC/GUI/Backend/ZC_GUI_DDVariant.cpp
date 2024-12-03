#include "ZC_GUI_DDVariant.h"

ZC_GUI_ColorsDropDown::ZC_GUI_ColorsDropDown(const ZC_GUI_ColorsButton& _colorsButton, uint _color_text, uint _color_arrow)
    : colorsButton(_colorsButton),
    color_text(_color_text),
    color_arrow(_color_arrow)
{}


ZC_GUI_DropDownIcon::ZC_GUI_DropDownIcon(const ZC_GUI_Font* pFont, uint color)
    : ZC_GUI_Obj(CalculateWidth(pFont), pFont->GetHeight() / 3.f, 0.f, color, ZC_GUI_IconUV::arrow_down, 0, ZC_GUI_Bindings::location_tex_Icons)
{}

float ZC_GUI_DropDownIcon::CalculateDistance(const ZC_GUI_Font* pFont)
{
    return pFont->GetHeight() / 5.f;
}

float ZC_GUI_DropDownIcon::CalculateWidth(const ZC_GUI_Font* pFont)
{
    return pFont->GetHeight() * 0.6f;
}
