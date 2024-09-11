#pragma once

#include "ZC_GUI_IconUV.h"
#include "ZC_GUI_Bindings.h"
#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseText.h>

struct ZC_GUI_ColorsDropDown
{
    ZC_GUI_ColorsButton colorsButton;
    uint color_text;
    uint color_arrow;

    ZC_GUI_ColorsDropDown(const ZC_GUI_ColorsButton& _colorsButton = ZC_GUI_ColorsButton(ZC_GUI_Colors::dropDownSwitch_button, ZC_GUI_Colors::dropDownSwitch_button_under_cursor,
        ZC_GUI_Colors::dropDownSwitch_button_pressed), uint _color_text = ZC_GUI_Colors::dropDownSwitch_text, uint _color_arrow = ZC_GUI_Colors::dropDownSwitch_arrow);

};

struct ZC_GUI_DropDownIcon : public ZC_GUI_Obj
{
    static inline const float width = 12.f;
    static inline const float height = 6.f;

    ZC_GUI_DropDownIcon(uint color);

        //  indent from button's left border to text
    static float GetTextIndentX();
};

    //  ZC_GUI_SwitchDropDown::ColorsDropDownSwitch

template <typename THolder>
struct ZC_GUI_DDVariant : public ZC_GUI_ButtonMouseText
{
    THolder* pHolder;

    ZC_GUI_DDVariant(THolder* _pHolder, float width, float height, const std::wstring& _wstr);

    void VLeftButtonUp_BM(float time) override;
};

    //  ZC_GUI_SwitchDropDown::ZC_GUI_DDVariant

template <typename THolder>
ZC_GUI_DDVariant<THolder>::ZC_GUI_DDVariant(THolder* _pHolder, float width, float height, const std::wstring& _wstr)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None),
    ZC_GUI_ButtonMouseText(width, height, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(ZC_GUI_DropDownIcon::GetTextIndentX(), ZC_GUI_TextForButton::Indent::Left),_wstr, true, 0, ZC_GUI_TextAlignment::Left)),
    pHolder(_pHolder)
{}

template <typename THolder>
void ZC_GUI_DDVariant<THolder>::VLeftButtonUp_BM(float time)
{
    pHolder->VariantChoosed(this);
}