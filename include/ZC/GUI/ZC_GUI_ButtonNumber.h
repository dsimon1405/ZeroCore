#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouseText.h>

#include <ZC/GUI/Text/ZC_GUI_TextManager.h>
#include <ZC/GUI/ZC_GUI_IconUV.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>
#include "ZC_cGUI_Number.h"
#include <ZC/GUI/Text/ZC_GUI_TextInputWindow.h>

template <ZC_GUI_Number::cNumber TNum>
struct ZC_GUI_ButtonNumber : public ZC_GUI_ButtonMouseText
{
    struct ColorsButtonNumber
    {
        ColorsButton colorsButton;
        uint color_text;
        ColorsButton colorsArrow;

        ColorsButtonNumber(const ColorsButton& _colorsButton = ColorsButton(ZC_GUI_Colors::number_button, ZC_GUI_Colors::number_button_under_cursor, ZC_GUI_Colors::number_button_pressed),
                uint _color_text = ZC_GUI_Colors::number_text, const ColorsButton& _colorsArrow = ColorsButton(ZC_GUI_Colors::number_arrow, ZC_GUI_Colors::number_arrow_under_cursor, ZC_GUI_Colors::number_arrow_pressed))
            : colorsButton(_colorsButton),
            color_text(_color_text),
            colorsArrow(_colorsArrow)
        {}
    };

    ZC_GUI_ButtonNumber(uint width, uint height, TNum _number, TNum _number_min, TNum _number_max, TNum _step, TNum _step_fast, unsigned char _afterDot_count, ZC_GUI_TextAlignment textAlignment,
            const ColorsButtonNumber& colorsButtonNumber = {})
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons),
            ZC_GUI_BF_M__CursorMoveOnMBLPress | ZC_GUI_BF_M__Scroll | ZC_GUI_BF_M__DoubleCLick, colorsButtonNumber.colorsButton),
        ZC_GUI_ButtonMouseText(width, height, 0,
            ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(0.f, ZC_GUI_TextForButton::Indent::Indent_X::Center), ZC_GUI_Number::NumberToWstr(number, afterDot_count), false, CalculateNumberMaxWidth(_number, _number_min, _number_max, _afterDot_count),
                textAlignment, colorsButtonNumber.color_text)),
        step(_step < 0 ? - _step : _step),    //  must be positive
        step_fast(_step_fast < 0 ? - _step_fast : _step_fast),    //  must be positive
        buttonArrow_left(this->GetHeight(), true, colorsButtonNumber.colorsArrow),
        buttonArrow_right(this->GetHeight(), false, colorsButtonNumber.colorsArrow)
    {
        this->VAddObj_Obj(&buttonArrow_left, nullptr);
        this->VAddObj_Obj(&buttonArrow_right, nullptr);
    }

    virtual void VNumberChanged_BN(TNum _number)
    {}

    /*
    Update number. If trying to set number outside the max/min limit, the max/min limit is set accordingly.

    Params:
    - _number - new value.
    */
    void ChangeNumber(TNum _number)
    {
        ChangeNumber(_number, false);
    }

private:
    struct ButtonArrow : public ZC_GUI_ButtonMouse
    {
        bool leftArrow;

        ButtonArrow(float size, bool _leftAarrow, const ColorsButton& colors)
            : ZC_GUI_ButtonBase(ZC_GUI_ObjData(size, size, 0,
                _leftAarrow ? GetArrowLeftUV() : ZC_GUI_IconUV::arrowRight, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__MBLPress, colors),
            ZC_GUI_ButtonMouse(0.f, 0.f, 0),
            leftArrow(_leftAarrow)
        {}

        void VLeftButtonDown_BM(float time) override;
        void VLeftButtonPressed_BM(float time) override;

    private:
        ZC_GUI_UV GetArrowLeftUV()
        {
            using namespace ZC_GUI_IconUV;
            return ZC_GUI_UV{ .bl = { arrowRight.tr[0], arrowRight.bl[1] }, .tr = { arrowRight.bl[0], arrowRight.tr[1] } };
        }
    };

    TNum number_min;
    TNum number_max;
    unsigned char afterDot_count;     //  numbers after the decimal point (coma)
    TNum number;
    TNum step;      //  number changing step on events
    TNum step_fast;     //  number changing step on fast event (button pressed)

    ButtonArrow buttonArrow_left;     //  button with text of number
    ButtonArrow buttonArrow_right;     //  button with text of number
    
    float VGetWidthComposite_Obj() override
    {
        return buttonArrow_left.VGetWidth_Obj() + this->VGetWidth_Obj() + buttonArrow_right.VGetWidth_Obj();
    }

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
    {       //  left button
        buttonArrow_left.VSet_pBL_Obj(_bl);
            //  text button
        ZC_Vec2<float> next_bl(_bl[0] + buttonArrow_left.VGetWidth_Obj(), _bl[1]);
        (*(this->pBL)) = next_bl;
        this->textForButton.VSet_pBL_Obj(next_bl);
            //  right button
        buttonArrow_right.VSet_pBL_Obj({ next_bl[0] + this->VGetWidth_Obj(), next_bl[1] });
    }

    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override
    {
        return buttonArrow_left.MakeCursorCollision_Obj(x, y, rpObj, rpScroll) || MakeCursorCollision_Obj(x, y, rpObj, rpScroll) || buttonArrow_right.MakeCursorCollision_Obj(x, y, rpObj, rpScroll);
    }

    void VScroll_Obj(float vertical, float time) override
    {
        ChangeNumber(number + (TNum(vertical) * step), true);
    }

    void VCursorMove_Obj(float rel_x, float rel_y) override
    {
        ChangeNumber(number + (TNum(rel_x) * step), true);
    }

    void VLeftButtonDoubleClick_BM(float time) override
    {
        ZC_GUI_TextInputWindow::StartInputWindow<TNum>((*(this->pBL))[0], (*(this->pBL))[1], this->VGetWidth_Obj(),
            ZC_GUI_TextInputWindow::NumberInput<TNum>(number, { &ZC_GUI_ButtonNumber<TNum>::ChangeNumber, this }, afterDot_count), false);
    }

    float CalculateNumberMaxWidth(TNum _number, TNum _number_min, TNum _number_max, unsigned char _afterDot_count)
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

        if (std::same_as<TNum, float> || std::same_as<TNum, double> || std::same_as<TNum, long double>)
        {
            float minNumber_sybmolsBeforeDot(str_number_min.find('.'));
            float number_min_length = number_min < TNum(0) ?
                minus_length + ((minNumber_sybmolsBeforeDot - 1.f) * numb_length) + dot_length + (afterDot_count * numb_length)
                                                //  first symbol withought left_offset
                : ((minNumber_sybmolsBeforeDot * numb_length) - numb_left_offset) + dot_length + (afterDot_count * numb_length);
            
            float maxNumber_sybmolsBeforeDot(str_number_max.find('.'));
            float number_max_length = number_max < TNum(0) ?
                minus_length + ((maxNumber_sybmolsBeforeDot - 1.f) * numb_length) + dot_length + (afterDot_count * numb_length)
                                                //  first symbol withought left_offset
                : ((maxNumber_sybmolsBeforeDot * numb_length) - numb_left_offset) + dot_length + (afterDot_count * numb_length);
            
            return number_min_length > number_max_length ? number_min_length : number_max_length;
        }
        else
        {
            float minNumber_size(str_number_min.size());
            float number_min_length = number_min < TNum(0) ?
                minus_length + ((minNumber_size - 1.f) * numb_length)
                : (minNumber_size * numb_length) - numb_left_offset;    //  first symbol withought left_offset
            
            float maxNumber_size(str_number_max.size());
            float number_max_length = number_max < TNum(0) ?
                minus_length + ((maxNumber_size - 1.f) * numb_length)
                : maxNumber_size * numb_length - numb_left_offset;    //  first symbol withought left_offset

            return number_min_length > number_max_length ? number_min_length : number_max_length;
        }
    }

    void ChangeNumber(TNum _number, bool call_VNumberChanged_BN)
    {
        TNum temp_number = _number < number_min ? number_min
            : _number > number_max ? number_max
            : _number;
        if (number == temp_number) return;
        number = temp_number;
        this->UpdateText_BMT(ZC_GUI_Number::NumberToWstr(number, afterDot_count), true);
        if (call_VNumberChanged_BN) VNumberChanged_BN(number);
    }
};



template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::ButtonArrow::VLeftButtonDown_BM(float time)
{
    ZC_GUI_ButtonNumber<TNum>* pBN = dynamic_cast<ZC_GUI_ButtonNumber<TNum>*>(this->pObjHolder);
    pBN->ChangeNumber(pBN->number + (leftArrow ? - pBN->step : pBN->step), true);
}

template <ZC_GUI_Number::cNumber TNum>
void ZC_GUI_ButtonNumber<TNum>::ButtonArrow::VLeftButtonPressed_BM(float time)
{
    ZC_GUI_ButtonNumber<TNum>* pBN = dynamic_cast<ZC_GUI_ButtonNumber<TNum>*>(this->pObjHolder);
    pBN->ChangeNumber(pBN->number + (leftArrow ? - pBN->step_fast : pBN->step_fast), true);
}