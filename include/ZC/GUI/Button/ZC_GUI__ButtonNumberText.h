#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonNumberText.h>
#include <ZC/GUI/ZC_GUI__Obj.h>

template <ZC_GUI_Number::cNumber TNum>
class ZC_GUI__ButtonNumberText;

template <ZC_GUI_Number::cNumber TNum>
struct ZC_GUI__BNT : public ZC_GUI_ButtonNumberText<TNum>
{
    ZC_GUI__ButtonNumberText<TNum>* pBNT;

    ZC_GUI__BNT(ZC_GUI__ButtonNumberText<TNum>* _pBNT, ZC_GUI_ButtonNumber<TNum>&& buttonNumber, ZC_GUI_Text&& _text, float _text_button_distance);

    void VNumberChanged_BN(TNum _number) override;
};

/*
Button with number
Must be overrided:
- virtual void VNumberChanged(TNum _number)
*/
template <ZC_GUI_Number::cNumber TNum>
class ZC_GUI__ButtonNumberText : public ZC_GUI__Obj<ZC_GUI__BNT<TNum>>
{
public:
    /*
    Params:
    - buttonNumber - nutton number.
    - _text - name of a button.
    - _text_button_distance - pixel distance betwwen buttonNumber and _text.
    */
    ZC_GUI__ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, ZC_GUI_Text&& _text, float _text_button_distance);

    virtual void VNumberChanged(TNum _number) {}

    /*
    Update number. If trying to set number outside the max/min limit, the max/min limit is set accordingly.

    Params:
    - _number - new value.
    */
    void UpdateNumber(TNum _number);
        //  Return number.
    TNum GetNumber() const noexcept;
        //  Retrun button's name.
    const std::wstring& GetName_BNT();
};


    //  ZC_GUI__ButtonNumberText

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI__ButtonNumberText<TNum>::ZC_GUI__ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, ZC_GUI_Text&& _text, float _text_button_distance)
    : ZC_GUI__Obj<ZC_GUI__BNT<TNum>>(ZC_GUI__BNT<TNum>(this, std::move(buttonNumber), std::move(_text), _text_button_distance))
{}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI__ButtonNumberText<TNum>::UpdateNumber(TNum _number)
{
    this->obj.UpdateNumber(_number);
}

template <ZC_GUI_Number::cNumber TNum>
TNum ZC_GUI__ButtonNumberText<TNum>::GetNumber() const noexcept
{
    return this->obj.GetNumber();
}

template <ZC_GUI_Number::cNumber TNum>
const std::wstring& ZC_GUI__ButtonNumberText<TNum>::GetName_BNT()
{
    return this->obj.GetName_BNT();
}


    //  ZC_GUI__ButtonNumberText

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI__BNT<TNum>::ZC_GUI__BNT(ZC_GUI__ButtonNumberText<TNum>* _pBNT, ZC_GUI_ButtonNumber<TNum>&& buttonNumber, ZC_GUI_Text&& _text, float _text_button_distance)
    : ZC_GUI_ButtonBase(dynamic_cast<ZC_GUI_ButtonBase&&>(buttonNumber)),
    ZC_GUI_ButtonNumberText<TNum>(std::move(buttonNumber), std::move(_text), _text_button_distance),
    pBNT(_pBNT)
{}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI__BNT<TNum>::VNumberChanged_BN(TNum _number)
{
    pBNT->VNumberChanged(_number);
}