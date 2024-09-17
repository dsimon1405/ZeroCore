#include "ZC_GUI__Click.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>

    //  ZC_GUI__ClickMouse

ZC_GUI__ClickMouse::ZC_GUI__ClickMouse(float width, float height, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ClickMouse(width, height, ZC_GUI_IconUV::button, std::move(_callback_button_down), std::move(_callback_button_up), _colorsButton)
{}

ZC_GUI__ClickMouse::ZC_GUI__ClickMouse(float width, float height, const ZC_GUI_UV& uv, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__Click<ZC_GUI__ButtonMouse>(ZC_GUI__ButtonMouse(width, height, ZC_GUI_BF__None, uv,_colorsButton), std::move(_callback_button_down), std::move(_callback_button_up))
{}


    //  ZC_GUI__ClickMouseText

ZC_GUI__ClickMouseText::ZC_GUI__ClickMouseText(float width, float height, ZC_GUI_TextForButton&& text, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ClickMouseText(width, height, ZC_GUI_IconUV::button, std::move(text), std::move(_callback_button_down), std::move(_callback_button_up), _colorsButton)
{}

ZC_GUI__ClickMouseText::ZC_GUI__ClickMouseText(float width, float height, const ZC_GUI_UV& uv, ZC_GUI_TextForButton&& text, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__Click<ZC_GUI__ButtonMouseText>(ZC_GUI__ButtonMouseText(width, height, ZC_GUI_BF__None, std::move(text), uv,_colorsButton),
        std::move(_callback_button_down), std::move(_callback_button_up))
{}

ZC_GUI__ClickMouseText::ZC_GUI__ClickMouseText(float width, float height, const std::wstring& name, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ClickMouseText(width, height, ZC_GUI_IconUV::button, name, std::move(callback_button_down), std::move(callback_button_up), _colorsButton)
{}

ZC_GUI__ClickMouseText::ZC_GUI__ClickMouseText(float width, float height, const ZC_GUI_UV& uv, const std::wstring& name, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ClickMouseText(width, height, uv, ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(0.f, ZC_GUI_TFB_Indent::Center), name, true, 0, ZC_GUI_TextAlignment::Center),
        std::move(callback_button_down), std::move(callback_button_up), _colorsButton)
{}


    //  ZC_GUI__ClickKeyboard

ZC_GUI__ClickKeyboard::ZC_GUI__ClickKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ClickKeyboard(_buttonId, width, height, ZC_GUI_IconUV::button, std::move(_callback_button_down), std::move(_callback_button_up),_colorsButton)
{}

ZC_GUI__ClickKeyboard::ZC_GUI__ClickKeyboard(ZC_ButtonID _buttonId, float width, float height, const ZC_GUI_UV& uv, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__Click<ZC_GUI__ButtonKeyboard>(ZC_GUI__ButtonKeyboard(_buttonId, width, height, ZC_GUI_BF__None, uv, _colorsButton), std::move(_callback_button_down), std::move(_callback_button_up))
{}


    //  ZC_GUI__ClickMouseAndKeyboard

ZC_GUI__ClickMouseAndKeyboard::ZC_GUI__ClickMouseAndKeyboard(float width, float height, ZC_ButtonID _buttonId, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__ClickMouseAndKeyboard(width, height, _buttonId, ZC_GUI_IconUV::button, std::move(_callback_button_down), std::move(_callback_button_up),_colorsButton)
{}

ZC_GUI__ClickMouseAndKeyboard::ZC_GUI__ClickMouseAndKeyboard(float width, float height, ZC_ButtonID _buttonId, const ZC_GUI_UV &uv, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, const ZC_GUI_ColorsButton& _colorsButton)
    : ZC_GUI__Click<ZC_GUI__ButtonMouseAndKeyboard>(ZC_GUI__ButtonMouseAndKeyboard(width, height, ZC_GUI_BF__None, _buttonId, uv, _colorsButton), std::move(_callback_button_down), std::move(_callback_button_up))
{}