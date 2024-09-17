#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonNumberText.h>
#include <ZC/GUI/ZC_GUI__Obj.h>

//  Button with number
template <ZC_GUI_Number::cNumber TNum>
struct ZC_GUI__ButtonNumberText : public ZC_GUI__Obj<ZC_GUI_ButtonNumberText<TNum>>
{
    /*
    Params:
    - buttonNumber - nutton number.
    - name - name of a button.
    - _text_button_distance - pixel distance betwwen buttonNumber and _text.
    - color_name - name's color (may stay default).
    */
    ZC_GUI__ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, const std::wstring& name, float _text_button_distance, uint color_name = ZC_GUI_Colors::window_text);

    /*
    Update number. If trying to set number outside the max/min limit, the max/min limit is set accordingly.

    Params:
    - _number - new value.
    - use_callback - if true will use callback setted in ZC_GUI_ButtonNumber constructor.
    */
    void SetNumber(TNum _number, bool use_callback);
        //  Return number.
    TNum GetNumber() const noexcept;
        //  Retrun button's name.
    const std::wstring& GetName();
};


template <ZC_GUI_Number::cNumber TNum>
ZC_GUI__ButtonNumberText<TNum>::ZC_GUI__ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, const std::wstring& name, float _text_button_distance, uint color_name)
    : ZC_GUI__Obj<ZC_GUI_ButtonNumberText<TNum>>(ZC_GUI_ButtonNumberText<TNum>(std::move(buttonNumber), name, _text_button_distance, color_name))
{}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI__ButtonNumberText<TNum>::SetNumber(TNum _number, bool use_callback)
{
    this->obj.SetNumber(_number, use_callback);
}

template <ZC_GUI_Number::cNumber TNum>
TNum ZC_GUI__ButtonNumberText<TNum>::GetNumber() const noexcept
{
    return this->obj.GetNumber();
}

template <ZC_GUI_Number::cNumber TNum>
const std::wstring& ZC_GUI__ButtonNumberText<TNum>::GetName()
{
    return this->obj.GetName_BNT();
}