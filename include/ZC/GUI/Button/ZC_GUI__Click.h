#pragma once

#include <ZC/Tools/Function/ZC_Function.h>

template <typename TButton>
class ZC_GUI__Click : public TButton
{
public:
    ZC_GUI__Click(TButton&& button, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        ZC_Function<void(bool)>&& _callback_focuse_changed);

    void UpdateCallbacks(ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up, ZC_Function<void(bool)>&& _callback_focus_changed);

protected:
    ZC_Function<void(float)> callback_button_down;
    ZC_Function<void(float)> callback_button_up;
    ZC_Function<void(bool)> callback_focus_changed;

private:
    void VLeftButtonDown(float time) override;
    void VLeftButtonUp(float time) override;
    void VKeyboardButtonDown(float time) override;
    void VKeyboardButtonUp(float time) override;
    void VFocuseChanged(bool isFocused) override;
};

template <typename TButton>
ZC_GUI__Click<TButton>::ZC_GUI__Click(TButton&& button, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        ZC_Function<void(bool)>&& _callback_focuse_changed)
    : TButton(std::move(button)),
    callback_button_down(std::move(_callback_button_down)),
    callback_button_up(std::move(_callback_button_up)),
    callback_focus_changed(std::move(_callback_focuse_changed))
{}

template <typename TButton>
void ZC_GUI__Click<TButton>::UpdateCallbacks(ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
    ZC_Function<void(bool)>&& _callback_focus_changed)
{
    callback_button_down = std::move(callback_button_down);
    callback_button_up = std::move(callback_button_up);
    callback_focus_changed = std::move(callback_focus_changed);
}

template <typename TButton>
void ZC_GUI__Click<TButton>::VLeftButtonDown(float time)
{
    if (callback_button_down) callback_button_down(time);
}

template <typename TButton>
void ZC_GUI__Click<TButton>::VLeftButtonUp(float time)
{
    if (callback_button_up) callback_button_up(time);
}

template <typename TButton>
void ZC_GUI__Click<TButton>::VKeyboardButtonDown(float time)
{
    if (callback_button_down) callback_button_down(time);
}

template <typename TButton>
void ZC_GUI__Click<TButton>::VKeyboardButtonUp(float time)
{
    if (callback_button_up) callback_button_up(time);
}

template <typename TButton>
void ZC_GUI__Click<TButton>::VFocuseChanged(bool isFocused)
{
    if (callback_focus_changed) callback_focus_changed(isFocused);
}


#include "ZC_GUI__ButtonMouse.h"
    //  Button for left mouse button click controll.
struct ZC_GUI__ClickMouse : public ZC_GUI__Click<ZC_GUI__ButtonMouse>
{
    /*
    Params:
    - width - width in pixels.
    - height - height in pixels.
    - _callback_button_down - the function is called when the left mouse button is down the button.
    - _callback_button_up - the function is called when the left mouse button is up and the cursor is still above the button.
    - _callback_focuse_changed - the function is called when the focus change. If true - in focus.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ClickMouse(float width, float height, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsButton& _colorsButton = {});

    /*
    Params:
    - width - width in pixels.
    - height - height in pixels.
    - uv - custom uv if uses custom icons texture (not default).
    - _callback_button_down - the function is called when the left mouse button is down the button.
    - _callback_button_up - the function is called when the left mouse button is up and the cursor is still above the button.
    - _callback_focuse_changed - the function is called when the focus change. If true - in focus.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ClickMouse(float width, float height, const ZC_GUI_UV& uv, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsButton& _colorsButton = {});
};


#include "ZC_GUI__ButtonMouseText.h"
    //  Button with text for left mouse button click controll.
struct ZC_GUI__ClickMouseText : public ZC_GUI__Click<ZC_GUI__ButtonMouseText>
{
    /*
    Params:
    - width - width in pixels, if less then text pixels length, takes text pixels length.
    - height - height in pixels, if less then text pixels height, takes text pixels height.
    - text - button name. Look ZC_GUI_TextForButton ctr.
    - _callback_button_down - the function is called when the left mouse button is down the button.
    - _callback_button_up - the function is called when the left mouse button is up and the cursor is still above the button.
    - _callback_focuse_changed - the function is called when the focus change. If true - in focus.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ClickMouseText(float width, float height, ZC_GUI_TextForButton&& text, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsButton& _colorsButton = {});

    /*
    Params:
    - width - width in pixels, if less then text pixels length, takes text pixels length.
    - height - height in pixels, if less then text pixels height, takes text pixels height.
    - uv - custom uv if uses custom icons texture (not default).
    - text - button name. Look ZC_GUI_TextForButton ctr.
    - _colorsButton - button colors (may stay default).
    - _callback_button_down - the function is called when the left mouse button is down the button.
    - _callback_button_up - the function is called when the left mouse button is up and the cursor is still above the button.
    */
    ZC_GUI__ClickMouseText(float width, float height, const ZC_GUI_UV& uv, ZC_GUI_TextForButton&& text, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsButton& _colorsButton = {});

    /*
    Params:
    - width - width in pixels, if less then text pixels length, takes text pixels length.
    - height - height in pixels, if less then text pixels height, takes text pixels height.
    - name - button name.
    - _callback_button_down - the function is called when the left mouse button is down the button.
    - _callback_button_up - the function is called when the left mouse button is up and the cursor is still above the button.
    - _callback_focuse_changed - the function is called when the focus change. If true - in focus.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ClickMouseText(float width, float height, const std::wstring& name, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsButtonText& _colorsButtonText = {});

    /*
    Params:
    - width - width in pixels, if less then text pixels length, takes text pixels length.
    - height - height in pixels, if less then text pixels height, takes text pixels height.
    - uv - custom uv if uses custom icons texture (not default).
    - name - button name.
    - _callback_button_down - the function is called when the left mouse button is down the button.
    - _callback_button_up - the function is called when the left mouse button is up and the cursor is still above the button.
    - _callback_focuse_changed - the function is called when the focus change. If true - in focus.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ClickMouseText(float width, float height, const ZC_GUI_UV& uv, const std::wstring& name, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsButtonText& _colorsButtonText = {});
};


#include "ZC_GUI__ButtonKeyboard.h"
    //  Button for keyboard button click controll.
struct ZC_GUI__ClickKeyboard : public ZC_GUI__Click<ZC_GUI__ButtonKeyboard>
{
    /*
    Params:
    - _buttonId - button id for click.
    - width - width in pixels.
    - height - height in pixels.
    - _callback_button_down - the function is called when the button with _buttonId is down.
    - _callback_button_up - the function is called when the button with _buttonId is up.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ClickKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_Function<void(float)>&& _callback_button_down, ZC_Function<void(float)>&& _callback_button_up,
        const ZC_GUI_ColorsButton& _colorsButton = {});

    /*
    Params:
    - _buttonId - button id for click.
    - width - width in pixels.
    - height - height in pixels.
    - uv - button uv coords from custom texture.
    - _callback_button_down - the function is called when the button with _buttonId is down.
    - _callback_button_up - the function is called when the button with _buttonId is up.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ClickKeyboard(ZC_ButtonID _buttonId, float width, float height, const ZC_GUI_UV& uv, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, const ZC_GUI_ColorsButton& _colorsButton = {});
};


#include "ZC_GUI__ButtonMouseAndKeyboard.h"
    //  Button for left mouse and keyboard buttons click controll.
struct ZC_GUI__ClickMouseAndKeyboard : public ZC_GUI__Click<ZC_GUI__ButtonMouseAndKeyboard>
{
    /*
    Params:
    - width - width in pixels.
    - height - height in pixels.
    - _callback_button_down - the function is called when the left mouse button is down the button.
    - _callback_button_up - the function is called when the left mouse button is up and the cursor is still above the button.
    - _callback_focuse_changed - the function is called when the focus change. If true - in focus.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ClickMouseAndKeyboard(float width, float height, ZC_ButtonID _buttonId, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsButton& _colorsButton = {});

    /*
    Params:
    - width - width in pixels.
    - height - height in pixels.
    - uv - custom uv if uses custom icons texture (not default).
    - _callback_button_down - the function is called when the left mouse button is down the button.
    - _callback_button_up - the function is called when the left mouse button is up and the cursor is still above the button.
    - _callback_focuse_changed - the function is called when the focus change. If true - in focus.
    - _colorsButton - button colors (may stay default).
    */
    ZC_GUI__ClickMouseAndKeyboard(float width, float height, ZC_ButtonID _buttonId, const ZC_GUI_UV &uv, ZC_Function<void(float)>&& _callback_button_down,
        ZC_Function<void(float)>&& _callback_button_up, ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsButton& _colorsButton = {});
};