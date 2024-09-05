#pragma once

#include <ZC/GUI/ZC_GUI_ButtonNumber.h>
#include <ZC/GUI/Text/ZC_GUI_Text.h>

template <ZC_GUI_Number::cNumber TNum>
struct ZC_GUI_ButtonNumberText : public ZC_GUI_ButtonNumber<TNum>
{
    ZC_GUI_Text text;
    float text_button_distance;
    
    ZC_GUI_ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, ZC_GUI_Text&& _text, float _text_button_distance)
        : ZC_GUI_ButtonBase(dynamic_cast<ZC_GUI_ButtonBase&&>(buttonNumber)),
        ZC_GUI_ButtonNumber<TNum>(std::move(buttonNumber)),
        text(std::move(_text)),
        text_button_distance(_text_button_distance)
    {
        this->VAddObj_Obj(&text, nullptr);
    }

    const std::wstring& GetName_BNT()
    {
        return text.GetWStr();
    }

    float VGetWidthComposite_Obj() override
    {
        return text.VGetWidth_Obj() + text_button_distance + this->GetWidthComposite_BN();
    }

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
    {
        float y_offset = (this->GetHeight() - text.GetHeight()) / 2.f;
        y_offset == 0.f ? text.VSet_pBL_Obj(_bl) : text.VSet_pBL_Obj({ _bl[0], _bl[1] + y_offset });

        this->Set_pBL_BN({ _bl[0] + text.VGetWidth_Obj() + text_button_distance, _bl[1] });
    }

    void VConf_SetTextUV_Obj() override
    {
        this->textForButton.VConf_SetTextUV_Obj();
        text.VConf_SetTextUV_Obj();
    }
};