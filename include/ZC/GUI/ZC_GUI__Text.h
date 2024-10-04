#pragma once

#include "ZC_GUI__Obj.h"
#include <ZC/GUI/Backend/Text/ZC_GUI_Text.h>

class ZC_GUI__Text : public ZC_GUI__Obj<ZC_GUI_Text>
{
public:
    /*
    Createt text string.

    Params:
    - wstr - string in unicode.
    - _isImmutable - if true can't be used method UpdateText().
    - reserveWidth - may be reserve pixel width longer then in wstr (have effect only if _isImmutable = false). More info in method UpdateText().
    - textAlignment - if reservedWidth more then wstr pixel wisth, current wstr can have horizontal alignment in texture. In other case it's Left.
    - color - text's color (may stay default).
    */
    ZC_GUI__Text(const std::wstring& wstr, bool _isImmutable, int reserveWidth, ZC_GUI_TextAlignment textAlignment, unsigned int color = ZC_GUI_Colors::window_text);

    /*
    Update text in wstring. Data updates only if pixel width of new string less or equal current width. Text width don't change in any case.

    Params:
    - wstr - new text.
    - brootForceUpdate - if false -> at first trying to find same text in existing Immutable texts and use it, if can't find, creates new text from wstr.
        If _brrotForceMap is true -> just update data in existing space (it's usefull if created changing number, more profit to update data, then trying to find in existing texts...).

    Return:
    true on success, otherwise false
    */
    bool UpdateText(const std::wstring& wstr, bool brootForceUpdate);
    const std::wstring& GetWStr();
    void SetColor(uint color);
};