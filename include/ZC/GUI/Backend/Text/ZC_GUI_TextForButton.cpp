#include "ZC_GUI_TextForButton.h"

#include <cmath>

ZC_GUI_TextForButton::ZC_GUI_TextForButton(const Indent& _indent, const std::wstring& wstr, bool _isImmutable, int reserveWidth, ZC_GUI_TextAlignment textAlignment, unsigned int color)
    : ZC_GUI_Text(wstr, _isImmutable, reserveWidth, textAlignment, color),
    indent(_indent)
{}

float ZC_GUI_TextForButton::VGetWidthComposite_Obj()
{
    return indent.indent_x + this->VGetWidth_Obj();
}

void ZC_GUI_TextForButton::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    switch (indent.indentFlag_X)
    {
    case Indent::Left:
    {
        *pBL = ZC_Vec2<float>(std::round(_bl[0] + indent.indent_x), std::round(_bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f)));
    } break;
    case Indent::Right:
    {
        *pBL = ZC_Vec2<float>(std::round(_bl[0] + pObjHolder->VGetWidth_Obj() - indent.indent_x - this->VGetWidth_Obj()), std::round(_bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f)));
    } break;
    case Indent::Center:
    {
        *pBL = ZC_Vec2<float>(std::round(_bl[0] + ((pObjHolder->VGetWidth_Obj() - this->VGetWidth_Obj()) / 2.f)), std::round(_bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f)));
    } break;
    case Indent::OutOfButton:
    {
        *pBL = ZC_Vec2<float>(std::round(_bl[0] + pObjHolder->VGetWidth_Obj() + indent.indent_x), std::round(_bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f)));
    } break;
    }
}


    //  ZC_GUI_TextForButton::Indent

ZC_GUI_TextForButton::Indent::Indent(float _indent_x, Location _indentFlag_X)
    : indent_x(_indent_x < 0.f || _indentFlag_X == Center ? 0.f : _indent_x),
    indentFlag_X(_indentFlag_X)
{}