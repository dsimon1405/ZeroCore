#include "ZC_GUI__ButtonMouseText.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>

ZC_GUI__ButtonMouseText::ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ButtonMouseText(width, height, _buttonFlags, std::move(text), ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI__ButtonMouseText::ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text, const ZC_GUI_UV& uv, const ZC_GUI_ColorsButton& _colorsButton)
    :ZC_GUI__Button<ZC_GUI__BM<ZC_GUI_ButtonMouseText>>(ZC_GUI__BM<ZC_GUI_ButtonMouseText>(this, ZC_GUI_ButtonMouseText(width, height, _buttonFlags, std::move(text), uv, _colorsButton)))
{}

ZC_GUI__ButtonMouseText::ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const std::wstring& name, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ButtonMouseText(width, height, _buttonFlags, name, ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI__ButtonMouseText::ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const std::wstring& name, const ZC_GUI_UV& uv, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ButtonMouseText(width, height, _buttonFlags, ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(0, ZC_GUI_TFB_Indent::Center), name, true, 0, ZC_GUI_TextAlignment::Center), uv, _colorsButton)
{}

bool ZC_GUI__ButtonMouseText::UpdateText(const std::wstring& wstr, bool brootForceUpdate)
{
    return this->obj.UpdateText_BMT(wstr, brootForceUpdate);
}

const std::wstring& ZC_GUI__ButtonMouseText::GetWStr()
{
    return this->obj.GetWStr_BMT();
}