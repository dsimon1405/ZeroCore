#pragma once

#include "ZC_GUI__Obj.h"
#include <ZC/GUI/Backend/ZC_GUI_Switch.h>
#include <ZC/GUI/Backend/ZC_GUI_SwitchDropDown.h>

template <typename TSwitch>
struct ZC_GUI__Switch : public ZC_GUI__Obj<TSwitch>
{
    ZC_GUI__Switch(TSwitch&& sw);
    
    /*
    Make variant active and call callback with index.

    Params:
    - index - index (from variants vector) of variant to make active.
    - use_callback - if true will use callback setted in constructor.
    */
    void MakeVariantActive(uint index, bool use_callback);
};

//  Struct for creation switch (1 active variant) from textured (uv coords from iconsTexture (icons.png)) quads.
struct ZC_GUI__SwitchUV : public ZC_GUI__Switch<ZC_GUI_Switch<ZC_GUI_ButtonMouse>>
{
    /*
    Params:
    - variants - coords from texture (icons.png or custom) of variants. Indexes from that vector used as an active variant index.
    - width - pixel with of one variant.
    - height - pixel height of one variant.
    - orientation_horizontal - arrange variants in horizontal (true) or vertical (false) order.
    - distance - pixel distance between variants.
    - _callback - function calls on changing variant. Parameter - uint, will contain index (from variants vector) of variant that became active.
    - active_variant - index of variant from variants active on start. If stay default (UINT_MAX), no active variant on start.
    - _colorsButton - buttons collors (may stay default).
    */
    ZC_GUI__SwitchUV(const std::vector<ZC_GUI_UV>& variants, float width, float height, bool orientation_horizontal, float distance, ZC_Function<void(uint)> _callback, uint active_variant = UINT_MAX,
        const ZC_GUI_ColorsButton& _colorsButton = {});
};

//  Struct for creation switch (1 active variant) from textured (uv coords from iconsTexture (icons.png)) quads with support keyboard button click.
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
    - _callback - function calls on changing variant. Parameter - uint, will contain index (from variants vector) of variant that became active.
    - active_variant - index of variant from variants active on start. If stay default (UINT_MAX), no active variant on start.
    - _colorsButton - buttons collors (may stay default).
    */
    ZC_GUI__SwitchKeyboardUV(const std::vector<ZC_GUI_KeyboardUV>& variants, float width, float height, bool orientation_horizontal, float distance,
        ZC_Function<void(uint)> _callback, uint active_variant = UINT_MAX, const ZC_GUI_ColorsButton& _colorsButton = {});
};

//  Struct for creation switch (1 active variant) from texts (names).
struct ZC_GUI__SwitchText : public ZC_GUI__Switch<ZC_GUI_Switch<ZC_GUI_ButtonMouseText>>
{
    /*
    Params:
    - variants - text varinats (names).
    - width - pixel with of one variant (if less then longest names pixel length, takes longest length). All variants have same (longest) width.
    - height - pixel height of one variant (if less then font height, takes font height). All variants have same height.
    - orientation_horizontal - arrange variants in horizontal (true) or vertical (false) order.
    - distance - pixel distance between variants.
    - _callback - function calls on changing variant. Parameter - uint, will contain index (from variants vector) of variant that became active.
    - active_variant - index of variant from variants active on start. If stay default (UINT_MAX), no active variant on start.
    - _colorsButton - buttons collors (may stay default).
    - _color_text - text color (may stay default).
    */
    ZC_GUI__SwitchText(const std::vector<std::wstring>& variants, float width, float height, bool orientation_horizontal, float distance, ZC_Function<void(uint)> _callback, uint active_variant = UINT_MAX,
        const ZC_GUI_ColorsButton& _colorsButton = {}, uint _color_text = ZC_GUI_Colors::dropDownSwitch_text);
};

//  Struct for creation switch (1 active variant) from texts (names) in drop down style.
struct ZC_GUI__SwitchDropDown : public ZC_GUI__Switch<ZC_GUI_SwitchDropDown>
{
    /*
    Params:
    - variants - text varinats (names).
    - active_variant - index of the variant active at start.
    - width - pixel with of one variant (if less then longest names pixel length, takes longest length). All variants have same (longest) width.
    - height - pixel height of one variant (if less then font height, takes font height). All variants have same height.
    - _callback - function calls on changing variant. Parameter - uint, will contain index (from variants vector) of variant that became active.
    - colorsDropDownSwitch - buttons collors (may stay default).
    */
    ZC_GUI__SwitchDropDown(const std::vector<std::wstring>& variants, uint active_variant, float width, float height, ZC_Function<void(uint)> _callback, const ZC_GUI_ColorsDropDown& colorsDropDownSwitch = {});
};


    //  ZC_GUI__Switch

template <typename TSwitch>
ZC_GUI__Switch<TSwitch>::ZC_GUI__Switch(TSwitch&& sw)
    : ZC_GUI__Obj<TSwitch>(std::move(sw))
{}

template <typename TSwitch>
void ZC_GUI__Switch<TSwitch>::MakeVariantActive(uint index, bool use_callback)
{
    this->obj.MakeVariantActive(index, use_callback);
}