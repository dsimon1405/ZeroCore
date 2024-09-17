#pragma once

#include "ZC_GUI_ButtonNumber.h"
#include <ZC/GUI/Backend/Text/ZC_GUI_Text.h>

template <ZC_GUI_Number::cNumber TNum>
struct ZC_GUI_ButtonNumberText : public ZC_GUI_ButtonNumber<TNum>
{
    ZC_GUI_Text text;
    float text_button_distance;
    
    ZC_GUI_ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, const std::wstring& name, float _text_button_distance, uint color_name = ZC_GUI_Colors::window_text);
    
    ZC_GUI_ButtonNumberText(ZC_GUI_ButtonNumberText&& bnt);

    const std::wstring& GetName_BNT();

    float VGetWidthComposite_Obj() override;
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VConf_SetTextUV_Obj() override;
};


    
template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_ButtonNumberText<TNum>::ZC_GUI_ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, const std::wstring& name, float _text_button_distance, uint color_name)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(buttonNumber)),
    ZC_GUI_ButtonNumber<TNum>(std::move(buttonNumber)),
    text(name, true, 0, ZC_GUI_TextAlignment::Left, color_name),
    text_button_distance(_text_button_distance)
{}

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_ButtonNumberText<TNum>::ZC_GUI_ButtonNumberText(ZC_GUI_ButtonNumberText&& bnt)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(bnt)),
    ZC_GUI_ButtonNumber<TNum>(std::move(bnt)),
    text(std::move(bnt.text)),
    text_button_distance(bnt.text_button_distance)
{}

template <ZC_GUI_Number::cNumber TNum>
const std::wstring& ZC_GUI_ButtonNumberText<TNum>::GetName_BNT()
{
    return text.GetWStr();
}

template <ZC_GUI_Number::cNumber TNum>
float ZC_GUI_ButtonNumberText<TNum>::VGetWidthComposite_Obj()
{
    return text.VGetWidth_Obj() + text_button_distance + this->GetWidthComposite_BN();
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumberText<TNum>::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{       //  set here to avoid reset in move ctr
    this->VAddObj_Obj(&text, nullptr);

    float y_offset = (this->GetHeight() - text.GetHeight()) / 2.f;
    y_offset == 0.f ? text.VSet_pBL_Obj(_bl) : text.VSet_pBL_Obj({ _bl[0], _bl[1] + y_offset });

    this->Set_pBL_BN({ _bl[0] + text.VGetWidth_Obj() + text_button_distance, _bl[1] });
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumberText<TNum>::VConf_SetTextUV_Obj()
{
    this->textForButton.VConf_SetTextUV_Obj();
    text.VConf_SetTextUV_Obj();
}