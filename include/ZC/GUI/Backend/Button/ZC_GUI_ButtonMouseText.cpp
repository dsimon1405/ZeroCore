#include "ZC_GUI_ButtonMouseText.h"

#include <ZC/GUI/Backend/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/ZC_GUI_Bindings.h>

ZC_GUI_ButtonMouseText::ZC_GUI_ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text, const ColorsButton& _colorsButton)
    : ZC_GUI_ButtonMouseText(width, height, _buttonFlags, std::move(text), ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI_ButtonMouseText::ZC_GUI_ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text, const ZC_GUI_UV& uv, const ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uv, ZC_GUI_Bindings::bind_tex_Icons), _buttonFlags, _colorsButton),
    ZC_GUI_ButtonMouse(width, height, _buttonFlags),
    textForButton(std::move(text))
{
    this->VAddObj_Obj(&textForButton, nullptr);
        //  width buttons with can't be less then text width (if text inside the button)
    float textForButton_width = this->objs.front()->VGetWidthComposite_Obj();
    if (textForButton.indent.indentFlag_X != ZC_GUI_TextForButton::Indent::OutOfButton && width < textForButton_width) this->pObjData->width = textForButton_width;
    if (height < textForButton.GetHeight()) this->SetHeight_Obj(textForButton.GetHeight());
}

ZC_GUI_ButtonMouseText::ZC_GUI_ButtonMouseText(ZC_GUI_ButtonMouseText&& bmt)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(bmt)),
    ZC_GUI_ButtonMouse(static_cast<ZC_GUI_ButtonMouse&&>(bmt)),
    textForButton(std::move(bmt.textForButton))
{
    this->VAddObj_Obj(&textForButton, nullptr);
}

bool ZC_GUI_ButtonMouseText::UpdateText_BMT(const std::wstring& wstr, bool brootForceUpdate)
{
    return textForButton.UpdateText(wstr, brootForceUpdate);
}

const std::wstring& ZC_GUI_ButtonMouseText::GetWStr_BMT()
{
    return textForButton.GetWStr();
}

void ZC_GUI_ButtonMouseText::UpdateText_BMT(ZC_GUI_TextManager::Text* pText)
{
    textForButton.UpdateText(pText);
}

float ZC_GUI_ButtonMouseText::VGetWidthComposite_Obj()
{
    return textForButton.indent.indentFlag_X == ZC_GUI_TextForButton::Indent::OutOfButton ? this->VGetWidth_Obj() + this->objs.front()->VGetWidthComposite_Obj() : this->VGetWidth_Obj();
}

void ZC_GUI_ButtonMouseText::VConf_SetTextUV_Obj()
{
    textForButton.VConf_SetTextUV_Obj();
}