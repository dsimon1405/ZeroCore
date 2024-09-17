#pragma once

#include "ZC_GUI_Text.h"

    //  Struct for indent from button border to texture with text.
struct ZC_GUI_TFB_Indent
{
    enum Location
    {
        Left,       //  indent X calculates from the left border
        Right,      //  indent X calculates from right border
        Center,     //  ignore ZC_GUI_MBTParams::indent_x
        OutOfButton //  out of button (from right side)
    };
    float indent_x = 0.f;       //  indent from buttons left/right border
    Location indentFlag_X = Left;   //  indent specifier for indent_x

        //  ZC_GUI_TFB_Indent from button border to texture with text.
    ZC_GUI_TFB_Indent(float _indent_x, Location _indentFlag_X);
};

struct ZC_GUI_TextForButton : public ZC_GUI_Text
{
    const ZC_GUI_TFB_Indent indent;

    /*
    Pararms:
    - _indent - indent from button border(left/right/out of button) to text texture into the button.
    - wstr - button's name.
    - _isImmutable - if true text couldn't be changed.
    - reserveWidth - may be reserved texture with larger width then pixel width of current wstr.
    - textAlignment - if text texture width larger then wstr pixel width then text into the texture may have not Left (default) alignment.
    - color - text's color.
    */
    ZC_GUI_TextForButton(const ZC_GUI_TFB_Indent& _indent, const std::wstring& wstr, bool _isImmutable, int reserveWidth, ZC_GUI_TextAlignment textAlignment, unsigned int color = ZC_GUI_Colors::window_text);
    
    float VGetWidthComposite_Obj() override;
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
};