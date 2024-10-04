#pragma once

#include "ZC_GUI_ButtonNumber.h"
#include <ZC/GUI/Backend/Text/ZC_GUI_TextForButton.h>

template <ZC_GUI_Number::cNumber TNum>
struct ZC_GUI_ButtonNumberText : public ZC_GUI_ButtonNumber<TNum>
{
    ZC_GUI_TextForButton tfb_name;
    
    ZC_GUI_ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, ZC_GUI_TextForButton&& _tfb_name);
    
    ZC_GUI_ButtonNumberText(ZC_GUI_ButtonNumberText&& bnt);

    const std::wstring& GetName_BNT();

    /*
    Update text in wstring. Data updates only if pixel width of new string less or equal current width. Text width don't change in any case.

    Params:
    - wstr - new text.
    - brootForceUpdate - if false -> at first trying to find same text in existing Immutable texts and use it, if can't find, creates new text from wstr.
        If _brrotForceMap is true -> just update data in existing space (it's usefull if created changing number, more profit to update data, then trying to find in existing texts...).

    Return:
    true on success, otherwise false.
    */
    bool UpdateText_BNT(const std::wstring& wstr, bool brootForceUpdate);

    float VGetWidthComposite_Obj() override;
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VConf_SetTextUV_Obj() override;
};


    
template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_ButtonNumberText<TNum>::ZC_GUI_ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, ZC_GUI_TextForButton&& _tfb_name)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(buttonNumber)),
    ZC_GUI_ButtonNumber<TNum>(std::move(buttonNumber)),
    tfb_name(std::move(_tfb_name))
{
    if (tfb_name.indent.indentFlag_X != ZC_GUI_TFB_Indent::OutOfButtonLeft || tfb_name.indent.indentFlag_X != ZC_GUI_TFB_Indent::OutOfButtonRight)
        tfb_name.indent.indentFlag_X = ZC_GUI_TFB_Indent::OutOfButtonLeft;
}

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_ButtonNumberText<TNum>::ZC_GUI_ButtonNumberText(ZC_GUI_ButtonNumberText&& bnt)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(bnt)),
    ZC_GUI_ButtonNumber<TNum>(static_cast<ZC_GUI_ButtonNumber<TNum>&&>(bnt)),
    tfb_name(std::move(bnt.tfb_name))
{}

template <ZC_GUI_Number::cNumber TNum>
const std::wstring& ZC_GUI_ButtonNumberText<TNum>::GetName_BNT()
{
    return tfb_name.GetWStr();
}

template <ZC_GUI_Number::cNumber TNum>
bool ZC_GUI_ButtonNumberText<TNum>::UpdateText_BNT(const std::wstring& wstr, bool brootForceUpdate)
{
    return tfb_name.UpdateText(wstr, brootForceUpdate);
}

template <ZC_GUI_Number::cNumber TNum>
float ZC_GUI_ButtonNumberText<TNum>::VGetWidthComposite_Obj()
{
    return tfb_name.VGetWidthComposite_Obj() + this->GetWidthComposite_BN();
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumberText<TNum>::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{       //  set here to avoid reset in move ctr
    this->VAddObj_Obj(&tfb_name, nullptr);

    if (tfb_name.indent.indentFlag_X == ZC_GUI_TFB_Indent::OutOfButtonLeft)    //  first goes textForButton, set bl for them first
    {
        tfb_name.VSet_pBL_Obj(_bl);
        this->Set_pBL_BN({ _bl[0] + tfb_name.VGetWidthComposite_Obj(), _bl[1] });
    }
    else    //  first goes button, then textForButton, set our bl first
    {
        this->Set_pBL_BN(_bl);
        tfb_name.VSet_pBL_Obj(_bl);    //  textForButton calculate bl for themselt from holder's (our) bl
    }
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumberText<TNum>::VConf_SetTextUV_Obj()
{
    this->textForButton.VConf_SetTextUV_Obj();  //  texture of number
    tfb_name.VConf_SetTextUV_Obj();     //  texture of name
}