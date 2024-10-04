#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonNumberText.h>
#include <ZC/GUI/ZC_GUI__Obj.h>

//  Button with number
template <ZC_GUI_Number::cNumber TNum>
struct ZC_GUI__ButtonNumberText : public ZC_GUI__Obj<ZC_GUI_ButtonNumberText<TNum>>
{
    /*
    Params:
    - buttonNumber - button number.
    - _tfb_name - name struct.
    */
    ZC_GUI__ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, ZC_GUI_TextForButton&& _tfb_name);

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

    /*
    Update name in wstring. Data updates only if pixel width of new string less or equal current width. Text width don't change in any case.

    Params:
    - wstr - new text.
    - brootForceUpdate - if false -> at first trying to find same text in existing Immutable texts and use it, if can't find, creates new text from wstr.
        If _brrotForceMap is true -> just update data in existing space (it's usefull if created changing number, more profit to update data, then trying to find in existing texts...).

    Return:
    true on success, otherwise false.
    */
    bool UpdateText(const std::wstring& wstr, bool brootForceUpdate);
};


template <ZC_GUI_Number::cNumber TNum>
ZC_GUI__ButtonNumberText<TNum>::ZC_GUI__ButtonNumberText(ZC_GUI_ButtonNumber<TNum>&& buttonNumber, ZC_GUI_TextForButton&& _tfb_name)
    : ZC_GUI__Obj<ZC_GUI_ButtonNumberText<TNum>>(ZC_GUI_ButtonNumberText<TNum>(std::move(buttonNumber), std::move(_tfb_name)))
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

template <ZC_GUI_Number::cNumber TNum>
bool ZC_GUI__ButtonNumberText<TNum>::UpdateText(const std::wstring& wstr, bool brootForceUpdate)
{
    return this->obj.UpdateText_BNT(wstr, brootForceUpdate);
}