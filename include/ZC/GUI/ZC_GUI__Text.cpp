#include "ZC_GUI__Text.h"

ZC_GUI__Text::ZC_GUI__Text(const std::wstring& wstr, bool _isImmutable, int reserveWidth, ZC_GUI_TextAlignment textAlignment, unsigned int color)
    : ZC_GUI__Obj<ZC_GUI_Text>(ZC_GUI_Text(wstr, _isImmutable, reserveWidth, textAlignment, color))
{}

bool ZC_GUI__Text::UpdateText(const std::wstring& wstr, bool brootForceUpdate)
{
    return this->obj.UpdateText(wstr, brootForceUpdate);
}

const std::wstring& ZC_GUI__Text::GetWStr()
{
    return this->obj.GetWStr();
}

void ZC_GUI__Text::SetColor(uint color)
{
    this->obj.SetColor_Obj(color, true);
}