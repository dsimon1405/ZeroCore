#pragma once

#include "ZC_GUI__BM.h"
#include "ZC_GUI__Button.h"
#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseText.h>

struct ZC_GUI_ColorsButtonText
{
    uint text_color;
    ZC_GUI_ColorsButton colorsButton;

    ZC_GUI_ColorsButtonText(uint _text_color = ZC_GUI_Colors::window_text, ZC_GUI_ColorsButton _colorsButton = {});
};

/*
Creates button with mouse evetns.
May be override:
- void VScroll(float vertical, float time) {}
- void VCursorMove(float rel_x, float rel_y) {}
- void VLeftButtonDown(float time) {}
- void VLeftButtonDoubleClick(float time) {}
- void VLeftButtonPressed(float time) {}
- void VLeftButtonUp(float time) {}
*/
class ZC_GUI__ButtonMouseText : public ZC_GUI__Button<ZC_GUI__BM<ZC_GUI_ButtonMouseText>>
{
public:
    /*
    Params:
    - width - width in pixels, if less then text pixels length, takes text pixels length.
    - height - height in pixels, if less then text pixels height, takes text pixels height.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - text - button name. Look ZC_GUI_TextForButton ctr.
    - _colorsButton - buttons collors (may stay default).
    */
    ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text, const ZC_GUI_ColorsButton& _colorsButton = {});
    /*
    Params:
    - width - width in pixels, if less then text pixels length, takes text pixels length.
    - height - height in pixels, if less then text pixels height, takes text pixels height.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - text - button name. Look ZC_GUI_TextForButton ctr.
    - uv - custom uv if uses custom icons texture (not default).
    - _colorsButton - buttons collors (may stay default).
    */
    ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text, const ZC_GUI_UV& uv,
        const ZC_GUI_ColorsButton& _colorsButton = {});
    /*
    Params:
    - width - width in pixels, if less then text pixels length, takes text pixels length.
    - height - height in pixels, if less then text pixels height, takes text pixels height.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - name - button name.
    - _colorsButton - buttons collors (may stay default).
    */
    ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const std::wstring& name, const ZC_GUI_ColorsButtonText& _colorsButtonText = {});
    /*
    Params:
    - width - width in pixels, if less then text pixels length, takes text pixels length.
    - height - height in pixels, if less then text pixels height, takes text pixels height.
    - _buttonFlags - flags determine wich virtual methods will be called on evetns.
    - name - button name.
    - uv - custom uv if uses custom icons texture (not default).
    - _colorsButton - buttons collors (may stay default).
    */
    ZC_GUI__ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const std::wstring& name, const ZC_GUI_UV& uv,
        const ZC_GUI_ColorsButtonText& _colorsButtonText = {});

    /*
    Update text in wstring. Data updates only if pixel width of new string less or equal current width. Text width don't change in any case.

    Params:
    - wstr - new text.
    - brootForceUpdate - if false -> at first trying to find same text in existing Immutable texts and use it, if can't find, creates new text from wstr.
        If _brrotForceMap is true -> just update data in existing space (it's usefull if created changing number, more profit to update data, then trying to find in existing texts...).

    Return:
    true on success, otherwise false.
    */
    bool UpdateText(const std::wstring& wstr, bool brootForceUpdate);

        //  Return current wstring.
    const std::wstring& GetWStr();
};