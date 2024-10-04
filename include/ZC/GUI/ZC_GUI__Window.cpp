#include "ZC_GUI__Window.h"

    //  ZC_GUI__WinMutable

ZC_GUI__WinMutable::ZC_GUI__WinMutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow)
    : ZC_GUI__WinMutable(_woiData, ZC_GUI_IconUV::quad, _winFlags, colorsWindow)
{}

ZC_GUI__WinMutable::ZC_GUI__WinMutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow)
    : ZC_GUI_WindowWrap(_woiData, uv, _winFlags, colorsWindow)
{}


    //  ZC_GUI__WinImmutable

ZC_GUI__WinImmutable::ZC_GUI__WinImmutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow)
    : ZC_GUI__WinImmutable(_woiData, ZC_GUI_IconUV::quad, _winFlags, colorsWindow)
{}

ZC_GUI__WinImmutable::ZC_GUI__WinImmutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow)
    : ZC_GUI_WindowWrap(_woiData, uv, _winFlags, colorsWindow)
{}


    //  ZC_GUI__InputWindow

void ZC_GUI__InputWindow::SetColors(uint textInput_background, uint textInput_text, uint textInput_caret, uint texInput_highlight)
{
    ZC_GUI_TextInputWindow::SetColors(textInput_background, textInput_text, textInput_caret, texInput_highlight);
}

bool ZC_GUI__InputWindow::StartInputWindow(float bl_x, float bl_y, int win_width, int _max_symbols, const std::wstring& wstr, ZC_Function<void(const std::wstring&)>&& _callBack, bool highlight_text)
{
    return ZC_GUI_TextInputWindow::StartInputWindow(bl_x, bl_y, win_width, _max_symbols, wstr, std::move(_callBack), highlight_text);
}