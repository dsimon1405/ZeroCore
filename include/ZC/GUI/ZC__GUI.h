#pragma once

#include <ZC/GUI/Backend/Text/ZC_GUI_FontElement.h>

#include <string>

namespace ZC__GUI
{
        //  Set path to font file. Must be call before ZC_SWindow creation.
    void SetFontPath(const std::string& path);
        //  Set loading font height (default 14, but after load some symbols can have height larger ZC__GUI::GetFontHeight() return real height of largest symbol). Must be call before ZC_SWindow creation.
    void SetFontHeight(unsigned int pix_height);
        //  Set symbols loading from font (default ZC_GUI_FE__Symbols | ZC_GUI_FE__English | ZC_GUI_FE__Russian). Must be call before ZC_SWindow creation.
    void SetFontLoadingElements(ZC_GUI_FontElements fontElements);
        //  Return height in pixels of largest loaded symbols.
    int GetFontHeight();
        //  Return width in pixels of wstring.
    int CalculateWstrWidth(const std::wstring& wstr);
        //  Set gui state. If active true gui is drawing and processing events, otherwise not.
    void SetState(bool active);
        //  Return pixel width of longest number charagter
    int GetLongestNumberCharacterWidth();
}