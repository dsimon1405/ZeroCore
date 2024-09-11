#include "ZC_GUI__ButtonMouseText.h"

#include <ZC/GUI/Backend/ZC_GUI_IconUV.h>

ZC_GUI__ButtonMouseText::ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ButtonMouseText(width, height, _buttonFlags, std::move(text), ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI__ButtonMouseText::ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text, const ZC_GUI_UV& uv, const ZC_GUI_ColorsButton& _colorsButton)
    :ZC_GUI__Button<ZC_GUI__BM<ZC_GUI_ButtonMouseText, ZC_GUI__ButtonMouseText>>(ZC_GUI__BM<ZC_GUI_ButtonMouseText, ZC_GUI__ButtonMouseText>(
        ZC_GUI_ButtonMouseText(width, height, _buttonFlags, std::move(text), uv, _colorsButton), this))
{}

bool ZC_GUI__ButtonMouseText::UpdateText(const std::wstring& wstr, bool brootForceUpdate)
{
    return this->obj.UpdateText_BMT(wstr, brootForceUpdate);
}

const std::wstring& ZC_GUI__ButtonMouseText::GetWStr()
{
    return this->obj.GetWStr_BMT();
}