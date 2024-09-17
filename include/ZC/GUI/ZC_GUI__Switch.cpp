#include "ZC_GUI__Switch.h"

    //  ZC_GUI__SwitchUV

ZC_GUI__SwitchUV::ZC_GUI__SwitchUV(const std::vector<ZC_GUI_UV>& variants, float width, float height, bool orientation_horizontal, float distance, ZC_Function<void(uint)> _callback,
        uint active_variant, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__Switch<ZC_GUI_Switch<ZC_GUI_ButtonMouse>>(ZC_GUI_Switch<ZC_GUI_ButtonMouse>(variants, width, height, orientation_horizontal, distance, std::move(_callback), active_variant, _colorsButton))
{}

    //  ZC_GUI__SwitchKeyboardUV

ZC_GUI__SwitchKeyboardUV::ZC_GUI__SwitchKeyboardUV(const std::vector<ZC_GUI_KeyboardUV>& variants, float width, float height, bool orientation_horizontal, float distance,
        ZC_Function<void(uint)> _callback, uint active_variant, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__Switch<ZC_GUI_Switch<ZC_GUI_ButtonMouseAndKeyboard>>(ZC_GUI_Switch<ZC_GUI_ButtonMouseAndKeyboard>(variants, width, height, orientation_horizontal, distance, std::move(_callback), active_variant, _colorsButton))
{}

    //  ZC_GUI__SwitchText

ZC_GUI__SwitchText::ZC_GUI__SwitchText(const std::vector<std::wstring>& variants, float width, float height, bool orientation_horizontal, float distance,
        ZC_Function<void(uint)> _callback, uint active_variant, const ZC_GUI_ColorsButton& _colorsButton, uint _color_text)
    : ZC_GUI__Switch<ZC_GUI_Switch<ZC_GUI_ButtonMouseText>>(ZC_GUI_Switch<ZC_GUI_ButtonMouseText>(variants, width, height, orientation_horizontal, distance, std::move(_callback), active_variant, _colorsButton, _color_text))
{}

    //  ZC_GUI__SwitchDropDown

ZC_GUI__SwitchDropDown::ZC_GUI__SwitchDropDown(const std::vector<std::wstring>& variants, uint active_variant, float width, float height, ZC_Function<void(uint)> _callback, const ZC_GUI_ColorsDropDown& colorsDropDownSwitch)
    : ZC_GUI__Switch<ZC_GUI_SwitchDropDown>(ZC_GUI_SwitchDropDown(variants, active_variant, width, height, std::move(_callback), colorsDropDownSwitch))
{}