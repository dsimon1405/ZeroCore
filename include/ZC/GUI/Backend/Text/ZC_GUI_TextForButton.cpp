#include "ZC_GUI_TextForButton.h"

#include <cmath>

    //  ZC_GUI_TextForButton

ZC_GUI_TextForButton::ZC_GUI_TextForButton(const ZC_GUI_TFB_Indent& _indent, const std::wstring& wstr, bool _isImmutable, int reserveWidth, ZC_GUI_TextAlignment textAlignment,
        const ZC_GUI_TFB_Colors& _tfb_colors)
    : ZC_GUI_Text(wstr, _isImmutable, reserveWidth, textAlignment, _tfb_colors.color),
    indent(_indent),
    tfb_colors(_tfb_colors)
{}

float ZC_GUI_TextForButton::VGetWidthComposite_Obj()
{
    return indent.indent_x + this->VGetWidth_Obj();
}

void ZC_GUI_TextForButton::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    switch (indent.indentFlag_X)
    {
    case ZC_GUI_TFB_Indent::Left:
    {
        *(this->pBL) = ZC_Vec2<float>(std::round(_bl[0] + indent.indent_x), std::round(_bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f)));
    } break;
    case ZC_GUI_TFB_Indent::Right:
    {
        *(this->pBL) = ZC_Vec2<float>(std::round(_bl[0] + pObjHolder->VGetWidth_Obj() - indent.indent_x - this->VGetWidth_Obj()), std::round(_bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f)));
    } break;
    case ZC_GUI_TFB_Indent::Center:
    {
        *(this->pBL) = ZC_Vec2<float>(std::round(_bl[0] + ((pObjHolder->VGetWidth_Obj() - this->VGetWidth_Obj()) / 2.f)), std::round(_bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f)));
    } break;
    case ZC_GUI_TFB_Indent::OutOfButtonRight:
    {
        *(this->pBL) = ZC_Vec2<float>(std::round(_bl[0] + pObjHolder->VGetWidth_Obj() + indent.indent_x), std::round(_bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f)));
    } break;
    case ZC_GUI_TFB_Indent::OutOfButtonLeft: *(this->pBL) = _bl; break;
    }
}

void ZC_GUI_TextForButton::VChangeObjectActivity_Obj(bool _isAvtive, bool changeGPU)
{
    this->pObjData->color = _isAvtive ? tfb_colors.color : tfb_colors.color_not_active;
    if (changeGPU) VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
}


    //  ZC_GUI_TFB_Indent

ZC_GUI_TFB_Indent::ZC_GUI_TFB_Indent(float _indent_x, Location _indentFlag_X)
    : indent_x(_indent_x < 0.f || _indentFlag_X == Center ? 0.f : _indent_x),
    indentFlag_X(_indentFlag_X)
{}


    //  ZC_GUI_TFB_Colors

ZC_GUI_TFB_Colors::ZC_GUI_TFB_Colors(uint _color, uint _color_not_active)
    : color(_color),
    color_not_active(_color_not_active)
{}