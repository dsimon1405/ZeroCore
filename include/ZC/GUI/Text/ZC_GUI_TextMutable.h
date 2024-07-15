#pragma once

#include <ZC/GUI/Text/ZC_GUI_TextImmutable.h>

//  Text can be change
struct ZC_GUI_TextMutable : public ZC_GUI_TextImmutable
{
    /*
    Params:
    - wstr - wstring of text.
    - reserveWidth - width that must be reserved in pixels. To avoid creation of new object, may be reserved total width and use UpdateText() to change text.
    */
    ZC_GUI_TextMutable(const std::wstring& wstr, int reserveWidth)
        : ZC_GUI_TextImmutable(wstr, false, reserveWidth)
    {}

    /*
    Update text in wstring. Data updates only if pixel width of new string less or equal current width. Text width don't change in any case.

    Return:
    true on success, otherwise false
    */ 
    bool UpdateText(const std::wstring& wstr)
    {
        return ZC_GUI_TextManager::UpdateText(pText, wstr);
    }

    bool UpdateText(const std::list<ZC_GUI_ChData>& chDatas)
    {
        return ZC_GUI_TextManager::UpdateText(pText, chDatas);
    }
};