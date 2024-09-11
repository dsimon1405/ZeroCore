#pragma once

#include "ZC_GUI_ButtonMouseText.h"

#include <ZC/GUI/Backend/Text/ZC_GUI_TextManager.h>
#include <ZC/GUI/Backend/Text/ZC_GUI_TextForButton.h>
#include <ZC/GUI/Backend/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/ZC_GUI_Bindings.h>
#include <ZC/GUI/Backend/ZC_cGUI_Number.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_TextInputWindow.h>

//  Button for manipulation with number. TNum type of storing number.
template <ZC_GUI_Number::cNumber TNum>
struct ZC_GUI_ButtonNumber : public ZC_GUI_ButtonMouseText
{
    struct ColorsButtonNumber
    {
        ColorsButton colorsButton;
        uint color_text;
        ColorsButton colorsArrow;

        ColorsButtonNumber(const ColorsButton& _colorsButton = ColorsButton(ZC_GUI_Colors::number_button, ZC_GUI_Colors::number_button_under_cursor, ZC_GUI_Colors::number_button_pressed),
                uint _color_text = ZC_GUI_Colors::number_text, const ColorsButton& _colorsArrow = ColorsButton(ZC_GUI_Colors::number_arrow, ZC_GUI_Colors::number_arrow_under_cursor, ZC_GUI_Colors::number_arrow_pressed));
    };

    /*
    Button consist from 3 buttons: left arrow, button with number, right arrow.

    Params:
    - width - button with number pixel width (can't be less then texture with number length).
    - height - button with number pixel height (can't be less then font height).
    - _number - storing number.
    - _number_min - min number.
    - _number_max - max number.
    - _step - number will change on that value on left button mouse click.
    - _step_fast - number will change on that valut on left button mouse press.
    - _afterDot_count - if number is float/double/ldouble count of shown values after dot.
    - textAlignment - alignment of drawn number in texture. Have affect if texture with number larger then current number in the texture. Default Center alignment.
    - colorsButtonNumber - specific colors for button's objects and states.
    */
    ZC_GUI_ButtonNumber(uint width, uint height, TNum _number, TNum _number_min, TNum _number_max, TNum _step, TNum _step_fast, unsigned char _afterDot_count, ZC_GUI_TextAlignment textAlignment,
            const ColorsButtonNumber& colorsButtonNumber = {});

    ZC_GUI_ButtonNumber(ZC_GUI_ButtonNumber&& bn);

    virtual void VNumberChanged_BN(TNum _number) {}

    /*
    Update number. If trying to set number outside the max/min limit, the max/min limit is set accordingly.

    Params:
    - _number - new value.
    */
    void UpdateNumber(TNum _number);
    TNum GetNumber() const noexcept;
    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;

protected:
    ZC_GUI_ButtonBase GetButtonBase_BN(float width, float height, const ZC_GUI_ButtonBase::ColorsButton& _colorsbutton = {});
    void Set_pBL_BN(const ZC_Vec2<float>& _bl);
    float GetWidthComposite_BN();

private:
    struct ButtonArrow : public ZC_GUI_ButtonMouse
    {
        bool leftArrow;

        ButtonArrow(float size, bool _leftAarrow, const ColorsButton& colors);

        void VLeftButtonDown_BM(float time) override;
        void VLeftButtonPressed_BM(float time) override;

    private:
        ZC_GUI_UV GetArrowLeftUV();
    };

    TNum number_min;
    TNum number_max;
    unsigned char afterDot_count;     //  numbers after the decimal point (coma)
    TNum number;
    TNum step;      //  number changing step on events
    TNum step_fast;     //  number changing step on fast event (button pressed)
    int max_symbols;    //  max count of symbols in number include min/max values (sets in CalculateNumberMaxWidth())

    ButtonArrow buttonArrow_left;     //  button with text of number
    ButtonArrow buttonArrow_right;     //  button with text of number
    
    float VGetWidthComposite_Obj() override;
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VScroll_Obj(float vertical, float time) override;
    void VCursorMove_Obj(float rel_x, float rel_y) override;
    void ChangeAndUpdateNumber(ldouble offset);
    void VLeftButtonDoubleClick_BM(float time) override;
    void UpdateNumberFromArrow(TNum _number);
    float CalculateNumberMaxWidth(TNum _number, TNum _number_min, TNum _number_max, unsigned char _afterDot_count);
    void UpdateNumber(TNum _number, bool call_VNumberChanged_BN);
};


    //  ZC_GUI_ButtonNumber<TNum>

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_ButtonNumber<TNum>::ZC_GUI_ButtonNumber(uint width, uint height, TNum _number, TNum _number_min, TNum _number_max, TNum _step, TNum _step_fast, unsigned char _afterDot_count, ZC_GUI_TextAlignment textAlignment,
        const ColorsButtonNumber& colorsButtonNumber)
    : ZC_GUI_ButtonBase(GetButtonBase_BN(width, height, colorsButtonNumber.colorsButton)),
    ZC_GUI_ButtonMouseText(width, height, 0,
        ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(0.f, ZC_GUI_TextForButton::Indent::Location::Center), ZC_GUI_Number::NumberToWstr(number, afterDot_count), false,
            CalculateNumberMaxWidth(_number, _number_min, _number_max, _afterDot_count), textAlignment, colorsButtonNumber.color_text)),
    step(_step < 0 ? - _step : _step),    //  must be positive
    step_fast(_step_fast < 0 ? - _step_fast : _step_fast),    //  must be positive
    buttonArrow_left(this->GetHeight(), true, colorsButtonNumber.colorsArrow),
    buttonArrow_right(this->GetHeight(), false, colorsButtonNumber.colorsArrow)
{
    this->VAddObj_Obj(&buttonArrow_left, nullptr);
    this->VAddObj_Obj(&buttonArrow_right, nullptr);
}

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_ButtonNumber<TNum>::ZC_GUI_ButtonNumber(ZC_GUI_ButtonNumber&& bn)
    : ZC_GUI_ButtonBase(dynamic_cast<ZC_GUI_ButtonBase&&>(bn)),
    ZC_GUI_ButtonMouseText(dynamic_cast<ZC_GUI_ButtonMouseText&&>(bn)),
    number_min(bn.number_min),
    number_max(bn.number_max),
    afterDot_count(bn.afterDot_count),
    number(bn.number),
    step(bn.step),
    step_fast(bn.step_fast),
    max_symbols(bn.max_symbols),
    buttonArrow_left(std::move(bn.buttonArrow_left)),
    buttonArrow_right(std::move(bn.buttonArrow_right))
{
    this->VAddObj_Obj(&buttonArrow_left, nullptr);
    this->VAddObj_Obj(&buttonArrow_right, nullptr);
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::UpdateNumber(TNum _number)
{
    UpdateNumber(_number, false);
}

template <ZC_GUI_Number::cNumber TNum>
TNum ZC_GUI_ButtonNumber<TNum>::GetNumber() const noexcept
{
    return number;
}

template <ZC_GUI_Number::cNumber TNum>
bool ZC_GUI_ButtonNumber<TNum>::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    return buttonArrow_left.MakeCursorCollision_Obj(x, y, rpObj, rpScroll) || MakeCursorCollision_Obj(x, y, rpObj, rpScroll) || buttonArrow_right.MakeCursorCollision_Obj(x, y, rpObj, rpScroll);
}

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_ButtonBase ZC_GUI_ButtonNumber<TNum>::GetButtonBase_BN(float width, float height, const ZC_GUI_ButtonBase::ColorsButton& _colorsbutton)
{
    return ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons),
        ZC_GUI_BF_M__CursorMoveOnMBLPress | ZC_GUI_BF_M__Scroll | ZC_GUI_BF_M__DoubleCLick, _colorsbutton);
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::Set_pBL_BN(const ZC_Vec2<float>& _bl)
{       //  left button
    buttonArrow_left.VSet_pBL_Obj(_bl);
        //  text button
    ZC_Vec2<float> next_bl(_bl[0] + buttonArrow_left.VGetWidth_Obj(), _bl[1]);
    (*(this->pBL)) = next_bl;
    this->textForButton.VSet_pBL_Obj(next_bl);
        //  right button
    buttonArrow_right.VSet_pBL_Obj({ next_bl[0] + this->VGetWidth_Obj(), next_bl[1] });
}

template <ZC_GUI_Number::cNumber TNum>
float ZC_GUI_ButtonNumber<TNum>::GetWidthComposite_BN()
{
    return buttonArrow_left.VGetWidth_Obj() + this->VGetWidth_Obj() + buttonArrow_right.VGetWidth_Obj();
}

template <ZC_GUI_Number::cNumber TNum>
float ZC_GUI_ButtonNumber<TNum>::VGetWidthComposite_Obj()
{
    return GetWidthComposite_BN();
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    Set_pBL_BN(_bl);
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::VScroll_Obj(float vertical, float time)
{
    ChangeAndUpdateNumber(vertical);
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::VCursorMove_Obj(float rel_x, float rel_y)
{
    ChangeAndUpdateNumber(rel_x);
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::ChangeAndUpdateNumber(ldouble offset)
{
    ldouble new_number = ldouble(number) + (offset * ldouble(step));
    offset > 0 ? UpdateNumber(ldouble(number_max) > new_number ? TNum(new_number) : number_max, true)
        : UpdateNumber(ldouble(number_min) < new_number ? TNum(new_number) : number_min, true);
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::VLeftButtonDoubleClick_BM(float time)
{
    ZC_Vec2<float> text_bl = this->textForButton.Get_bl_Obj();
    ZC_GUI_TextInputWindow::StartInputNumberWindow<TNum>(text_bl[0], text_bl[1], this->textForButton.VGetWidth_Obj(),
        ZC_GUI_TextInputWindow::NumberInput<TNum>(number, { &ZC_GUI_ButtonNumber<TNum>::UpdateNumberFromArrow, this }, afterDot_count), false, max_symbols);
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::UpdateNumberFromArrow(TNum _number)
{
    UpdateNumber(_number, true);
}

template <ZC_GUI_Number::cNumber TNum>
float ZC_GUI_ButtonNumber<TNum>::CalculateNumberMaxWidth(TNum _number, TNum _number_min, TNum _number_max, unsigned char _afterDot_count)
{       //  set params
    number_min =_number_min < _number_max ? _number_min : _number_max;
    number_max =_number_max > _number_min ? _number_max : _number_min;
    afterDot_count = std::same_as<TNum, float> || std::same_as<TNum, double> || std::same_as<TNum, long double> ?
        _afterDot_count < 1 ? 1   //  1 is min after point count
            : _afterDot_count > 6 ? 6     //  6 is max after point count
                : _afterDot_count     //  legal after point count
        : 0;   //  number is integer
    number = _number < number_min ? number_min
        : _number > number_max ? number_max
        : _number;
        //  set static part
    static float numb_left_offset = 0.f; 
    auto lambGetLongestNumbLength = []()
    {
        float max_length = 0.f;
        for (wchar_t numb = L'0'; numb <= L'9'; ++numb)
        {
            auto pCh_numb = ZC_GUI_TextManager::pTM->font.GetCharacter(numb);
            float length = pCh_numb->left_offset + pCh_numb->width;
            if (length > max_length)
            {
                numb_left_offset = pCh_numb->left_offset;
                max_length = length;
            }
        }
        return max_length;
    };
    auto lambGetDotLength = []()
    {
        auto pCh_minus = ZC_GUI_TextManager::pTM->font.GetCharacter(L'.');
        return pCh_minus->left_offset + pCh_minus->width;
    };
    static const float numb_length = lambGetLongestNumbLength();
    static const float minus_length = ZC_GUI_TextManager::pTM->font.GetCharacter(L'-')->width;  //  minus always first, don't need left_offset
    static const float dot_length = lambGetDotLength();
        //  calculate width
    std::string str_number_min = std::to_string(number_min);
    std::string str_number_max = std::to_string(number_max);

    float number_min_length = 0.f;
    float number_max_length = 0.f;
    int number_min_symbols = 0;
    int number_max_symbols = 0;
    if (std::same_as<TNum, float> || std::same_as<TNum, double> || std::same_as<TNum, long double>)
    {
        float minNumber_sybmolsBeforeDot(str_number_min.find('.'));
        if (number_min < TNum(0))
        {
            number_min_length = minus_length + (minNumber_sybmolsBeforeDot * numb_length) + dot_length + (afterDot_count * numb_length);
            number_min_symbols = 1 + minNumber_sybmolsBeforeDot + 1 + afterDot_count;   //  first (1) is minus, second (1) is dot
        }
        else
        {                                    //  first symbol withought left_offset
            number_min_length = ((minNumber_sybmolsBeforeDot * numb_length) - numb_left_offset) + dot_length + (afterDot_count * numb_length);
            number_min_symbols = minNumber_sybmolsBeforeDot + 1 + afterDot_count;   //  1 is dot
        }
        
        float maxNumber_sybmolsBeforeDot(str_number_max.find('.'));
        if (number_max < TNum(0))
        {
            number_max_length = minus_length + (maxNumber_sybmolsBeforeDot * numb_length) + dot_length + (afterDot_count * numb_length);
            number_max_symbols = 1 + maxNumber_sybmolsBeforeDot + 1 + afterDot_count;   //  first (1) is minus, second (1) is dot
        }
        else
        {                                    //  first symbol withought left_offset
            number_max_length = ((maxNumber_sybmolsBeforeDot * numb_length) - numb_left_offset) + dot_length + (afterDot_count * numb_length);
            number_max_symbols = maxNumber_sybmolsBeforeDot + 1 + afterDot_count;   //  1 is dot
        }
    }
    else
    {
        number_min_symbols = str_number_min.size();
        float minNumber_size(number_min_symbols);
        number_min_length = number_min < TNum(0) ?
            minus_length + ((minNumber_size - 1.f) * numb_length)
            : (minNumber_size * numb_length) - numb_left_offset;    //  first symbol withought left_offset
        
        number_max_symbols = str_number_max.size();
        float maxNumber_size(number_max_symbols);
        number_max_length = number_max < TNum(0) ?
            minus_length + ((maxNumber_size - 1.f) * numb_length)
            : maxNumber_size * numb_length - numb_left_offset;    //  first symbol withought left_offset
    }

    max_symbols = number_min_symbols > number_max_symbols ? number_min_symbols : number_max_symbols;

    return number_min_length > number_max_length ? number_min_length : number_max_length;
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::UpdateNumber(TNum _number, bool call_VNumberChanged_BN)
{
    TNum temp_number = _number < number_min ? number_min
        : _number > number_max ? number_max
        : _number;
    if (number == temp_number) return;
    number = temp_number;
    this->UpdateText_BMT(ZC_GUI_Number::NumberToWstr(number, afterDot_count), true);
    if (call_VNumberChanged_BN) VNumberChanged_BN(number);
}


    //  ZC_GUI_ButtonNumber<TNum>::ColorsButtonNumber

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_ButtonNumber<TNum>::ColorsButtonNumber::ColorsButtonNumber(const ColorsButton& _colorsButton, uint _color_text, const ColorsButton& _colorsArrow)
    : colorsButton(_colorsButton),
    color_text(_color_text),
    colorsArrow(_colorsArrow)
{}


    //  ZC_GUI_ButtonNumber<TNum>::ButtonArrow

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_ButtonNumber<TNum>::ButtonArrow::ButtonArrow(float size, bool _leftAarrow, const ColorsButton& colors)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(size, size, 0,
        _leftAarrow ? GetArrowLeftUV() : ZC_GUI_IconUV::arrow_right, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__MBLPress, colors),
    ZC_GUI_ButtonMouse(0.f, 0.f, 0),
    leftArrow(_leftAarrow)
{}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::ButtonArrow::VLeftButtonDown_BM(float time)
{
ZC_GUI_ButtonNumber<TNum>* pBN = dynamic_cast<ZC_GUI_ButtonNumber<TNum>*>(this->pObjHolder);
pBN->ChangeAndUpdateNumber(leftArrow ? - pBN->step : pBN->step);
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::ButtonArrow::VLeftButtonPressed_BM(float time)
{
ZC_GUI_ButtonNumber<TNum>* pBN = dynamic_cast<ZC_GUI_ButtonNumber<TNum>*>(this->pObjHolder);
pBN->ChangeAndUpdateNumber(leftArrow ? - pBN->step_fast : pBN->step_fast);
}

template <ZC_GUI_Number::cNumber TNum>
ZC_GUI_UV ZC_GUI_ButtonNumber<TNum>::ButtonArrow::GetArrowLeftUV()
{
    using namespace ZC_GUI_IconUV;
    return ZC_GUI_UV{ .bl = { arrow_right.tr[0], arrow_right.bl[1] }, .tr = { arrow_right.bl[0], arrow_right.tr[1] } };
}