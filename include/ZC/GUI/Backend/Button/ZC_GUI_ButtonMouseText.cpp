#include "ZC_GUI_ButtonMouseText.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>

ZC_GUI_ButtonMouseText::ZC_GUI_ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& _textForButton, const ColorsButton& _colorsButton)
    : ZC_GUI_ButtonMouseText(width, height, _buttonFlags, std::move(_textForButton), ZC_GUI_IconUV::button, _colorsButton)
{}

ZC_GUI_ButtonMouseText::ZC_GUI_ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& _textForButton, const ZC_GUI_UV& uv, const ColorsButton& _colorsButton)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uv, ZC_GUI_Bindings::bind_tex_Icons), _buttonFlags, _colorsButton),
    ZC_GUI_ButtonMouse(width, height, _buttonFlags),
    textForButton(std::move(_textForButton))
{
        //  button's width can't be less then _textForButton width (if _textForButton inside the button)
    float textForButton_width = this->textForButton.VGetWidthComposite_Obj();
    if ((textForButton.indent.indentFlag_X != ZC_GUI_TFB_Indent::OutOfButtonRight || textForButton.indent.indentFlag_X != ZC_GUI_TFB_Indent::OutOfButtonLeft))
    {
        if (width < textForButton_width) this->pObjData->width = textForButton_width;
        if (height < textForButton.GetHeight()) this->SetHeight_Obj(textForButton.GetHeight());
    }
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

void ZC_GUI_ButtonMouseText::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    this->VAddObj_Obj(&textForButton, nullptr);
    
    if (textForButton.indent.indentFlag_X == ZC_GUI_TFB_Indent::OutOfButtonLeft)    //  first goes textForButton, set bl for them first
    {
        textForButton.VSet_pBL_Obj(_bl);
        *(this->pBL) = ZC_Vec2<float>({ _bl[0] + textForButton.VGetWidthComposite_Obj(), _bl[1] });
    }
    else    //  first goes button, then textForButton, set our bl first
    {
        *(this->pBL) = _bl;
        textForButton.VSet_pBL_Obj(_bl);    //  textForButton calculate bl for themselt from holder's (our) bl
    }
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
    return textForButton.indent.indentFlag_X == ZC_GUI_TFB_Indent::OutOfButtonRight ? this->VGetWidth_Obj() + this->objs.front()->VGetWidthComposite_Obj() : this->VGetWidth_Obj();
}

void ZC_GUI_ButtonMouseText::VConf_SetTextUV_Obj()
{
    textForButton.VConf_SetTextUV_Obj();
}