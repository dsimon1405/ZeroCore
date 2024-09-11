#pragma once

#include "ZC_GUI__VariantChoosed.h"
#include "ZC_GUI__Obj.h"
#include <ZC/GUI/Backend/ZC_GUI_Switch.h>
#include <ZC/GUI/Backend/ZC_GUI_SwitchDropDown.h>

template <typename TSwitch>
struct ZC_GUI__Switch : public ZC_GUI__Obj<ZC_GUI__VariantChoosed<ZC_GUI__Switch<TSwitch>, TSwitch>>
{
    ZC_GUI__Switch(TSwitch&& sw);
    /*
    Method must be averride. Calls on changing active variant.
    
    Params:
    - variant_index - index (from variants vector) of variant that became active.
    */
    virtual void VVariantChoosed(uint variant_index) {}
    
    /*
    Make variant active. Calls method -> void VVariantChoosed(uint variant_index).

    Params:
    - index - index (from variants vector) of variant to make active.
    */
    void MakeVariantActive(uint index);
};

/*
Struct for creation switch (1 active variant) from textured (uv coords from iconsTexture (icons.png)) quads.
Heir must override -> void VVariantChoosed(uint variant_index) from ZC_GUI__Switch struct. That method calls if active variant changes from gui. Parameter - variant_index will contain index of variant that became active
(index from vector with variants setted in ctr, starts from 0).
*/
struct ZC_GUI__SwitchUV : public ZC_GUI__Switch<ZC_GUI_Switch<ZC_GUI_ButtonMouse>>
{
    /*
    Params:
    - variants - coords from texture (icons.png or custom) of variants. Indexes from that vector used as an active variant index.
    - width - pixel with of one variant.
    - height - pixel height of one variant.
    - orientation_horizontal - arrange variants in horizontal (true) or vertical (false) order.
    - distance - pixel distance between variants.
    - _colorsButton - buttons collors (may be default).
    */
    ZC_GUI__SwitchUV(const std::vector<ZC_GUI_UV>& variants, float width, float height, bool orientation_horizontal, float distance, const ZC_GUI_ColorsButton& _colorsButton = {});
};

/*
Struct for creation switch (1 active variant) from textured (uv coords from iconsTexture (icons.png)) quads with support keyboard button click.
Heir must override -> void VVariantChoosed(uint variant_index) from ZC_GUI__Switch struct. That method calls if active variant changes from gui. Parameter - variant_index will contain index of variant that became active
(index from vector with variants setted in ctr, starts from 0).
*/
struct ZC_GUI__SwitchKeyboardUV : public ZC_GUI__Switch<ZC_GUI_Switch<ZC_GUI_ButtonMouseAndKeyboard>>
{
    /*
    Params:
    - variants.uv - coords from texture (icons.png or custom) of variants. Indexes from that vector used as an active variant index.
    - variants.buttonId - unique button id, to make variant active on click.
    - width - pixel with of one variant.
    - height - pixel height of one variant.
    - orientation_horizontal - arrange variants in horizontal (true) or vertical (false) order.
    - distance - pixel distance between variants.
    - _colorsButton - buttons collors (may be default).
    */
    ZC_GUI__SwitchKeyboardUV(const std::vector<ZC_GUI_KeyboardUV>& variants, float width, float height, bool orientation_horizontal, float distance, const ZC_GUI_ColorsButton& _colorsButton = {});
};

/*
Struct for creation switch (1 active variant) from texts (names).
Heir must override -> void VVariantChoosed(uint variant_index) from ZC_GUI__Switch struct. That method calls if active variant changes from gui. Parameter - variant_index will contain index of variant that became active
(index from vector with variants setted in ctr, starts from 0).
*/
struct ZC_GUI__SwitchText : public ZC_GUI__Switch<ZC_GUI_Switch<ZC_GUI_ButtonMouseText>>
{
    /*
    Params:
    - variants - text varinats (names).
    - width - pixel with of one variant (if less then longest names pixel length, takes longest length). All variants have same (longest) width.
    - height - pixel height of one variant (if less then font height, takes font height). All variants have same height.
    - orientation_horizontal - arrange variants in horizontal (true) or vertical (false) order.
    - distance - pixel distance between variants.
    - _colorsButton - buttons collors (may stay default).
    - _color_text - text color (may stay default).
    */
    ZC_GUI__SwitchText(const std::vector<std::wstring>& variants, float width, float height, bool orientation_horizontal, float distance, const ZC_GUI_ColorsButton& _colorsButton = {}, uint _color_text = ZC_GUI_Colors::dropDownSwitch_text);
};

/*
Struct for creation switch (1 active variant) from texts (names) in drop down style.
Heir must override -> void VVariantChoosed(uint variant_index) from ZC_GUI__Switch struct. That method calls if active variant changes from gui. Parameter - variant_index will contain index of variant that became active
(index from vector with variants setted in ctr, starts from 0).
*/
struct ZC_GUI__SwitchDropDown : public ZC_GUI__Switch<ZC_GUI_SwitchDropDown>
{
    /*
    Params:
    - variants - text varinats (names).
    - active_variant - index of the variant active at start.
    - width - pixel with of one variant (if less then longest names pixel length, takes longest length). All variants have same (longest) width.
    - height - pixel height of one variant (if less then font height, takes font height). All variants have same height.
    - colorsDropDownSwitch - buttons collors (may stay default).
    */
    ZC_GUI__SwitchDropDown(const std::vector<std::wstring>& variants, uint active_variant, float width, float height, const ZC_GUI_ColorsDropDown& colorsDropDownSwitch = {});
};


    //  ZC_GUI__Switch

template <typename TSwitch>
ZC_GUI__Switch<TSwitch>::ZC_GUI__Switch(TSwitch&& sw)
    : ZC_GUI__Obj<ZC_GUI__VariantChoosed<ZC_GUI__Switch<TSwitch>, TSwitch>>(ZC_GUI__VariantChoosed<ZC_GUI__Switch<TSwitch>, TSwitch>(this, std::move(sw)))
{}

template <typename TSwitch>
void ZC_GUI__Switch<TSwitch>::MakeVariantActive(uint index)
{
    this->obj.MakeVariantActive(index);
}