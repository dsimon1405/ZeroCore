#pragma once

#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
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
    ZC_GUI_DropDownIcon(const ZC_GUI_Font* pFont, uint color);

        //  calculate distance between items into the button (including borders). Created to get same values in ZC_GUI_DropDown and ZC_GUI_SwitchDropDown from font_height, in ctr class time 
    static float CalculateDistance(const ZC_GUI_Font* pFont);
        //  created to get same values in ZC_GUI_DropDown and ZC_GUI_SwitchDropDown from font_height, in ctr class time
    static float CalculateWidth(const ZC_GUI_Font* pFont);
};

    //  ZC_GUI_SwitchDropDown::ColorsDropDownSwitch

template <typename THolder>
struct ZC_GUI_DDVariant : public ZC_GUI_ButtonMouseText
{
    THolder* pHolder;

    ZC_GUI_DDVariant(THolder* _pHolder, const ZC_GUI_Font* pFont, float width, float height, const std::wstring& _wstr,
        const ColorsButton& _colorsButton = {}, unsigned int color_text = 0);

    void VLeftButtonUp_BM(float time) override;
        //  overrides for THolder = ZC_GUI_DropDown
    void VCursorCollisionEnd_Obj(float time) override;
};

    //  ZC_GUI_SwitchDropDown::ZC_GUI_DDVariant

template <typename THolder>
ZC_GUI_DDVariant<THolder>::ZC_GUI_DDVariant(THolder* _pHolder, const ZC_GUI_Font* pFont, float width, float height, const std::wstring& _wstr,
        const ColorsButton& _colorsButton, unsigned int color_text)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::location_tex_Icons), ZC_GUI_BF__None),
    ZC_GUI_ButtonMouseText(width, height, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(ZC_GUI_DropDownIcon::CalculateDistance(pFont),
        ZC_GUI_TFB_Indent::Left), pFont, _wstr, true, 0, ZC_GUI_TextAlignment::Left, color_text), _colorsButton),
    pHolder(_pHolder)
{}

template <typename THolder>
void ZC_GUI_DDVariant<THolder>::VLeftButtonUp_BM(float time)
{
    pHolder->VariantChoosed(this);
}

struct ZC_GUI_DropDown;

template <typename THolder>
void ZC_GUI_DDVariant<THolder>::VCursorCollisionEnd_Obj(float time)
{
    if (this->pObjData->color == this->colorsButton.color_button_pressed) return;     //  button pressed, wait while up
    this->SetColor_Obj(this->colorsButton.color_button, true);
    if constexpr (std::same_as<THolder, ZC_GUI_DropDown>)
    {
        if (pHolder->isUnderCursorFlag)     //  ZC_GUI_DropDown (pHolder) uses ZC_GUI_DDF__UnderCursor. Current button not under cursor any more, need to check is under cursor: ddWindow (pObjHolder) or drop down button (pHolder) -> if not stop drawing ddWidnow.
        {
            if ((this->pObjHolder->CheckCursorCollision_Obj()) || pHolder->CheckCursorCollision_Obj()) return;  //  cursor still above ddWindow (pObjHolder) or button (pHolder)
                //  cursor somwhere else, stop drawing window
            this->pObjHolder->VSetDrawState_Obj(false, true);     //  stop drawing ddWindow
            pHolder->SetColor_Obj(pHolder->colorsButton.color_button, true);   //  set default color to button
        }
    }
}