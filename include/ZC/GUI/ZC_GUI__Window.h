#pragma once

#include <ZC/GUI/Backend/Window/ZC_GUI_WindowWrap.h>

#include <ZC/GUI/Backend/Window/ZC_GUI_WinImmutable.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_WinMutable.h>

#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_TextInputWindow.h>


/*
Each window consist from rows (adds with AddRow() - they are invisible) in wich adds objects. Objects (buttons, texts and so on) adds in rows with AddObj(). Configuration set up all objects added in window.
Configuration makes once in ZC_SWindow::RunMainCycle() method. Mutable window may be reconfigured after configuration. Reconfiguration need when some object adds/erases in window after configuratin.
Recomended to use ZC_GUI__WinImmutable if don't need to add/erase objects after configuration. Object ZC_GUI__Tree may may add/erase branches (objects) after configuration, if uses that behaviour, tree must be used
into mutable window.
*/
struct ZC_GUI__WinMutable : public ZC_GUI_WindowWrap<ZC_GUI_WinMutable>
{
    ZC_GUI__WinMutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow = {});
    ZC_GUI__WinMutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow = {});
};

/*
Each window consist from rows (adds with AddRow() - they are invisible) in wich adds objects. Objects (buttons, texts and so on) adds in rows with AddObj(). Configuration set up all objects added in window.
Configuration makes once in ZC_SWindow::RunMainCycle() method. Immutable window can't be reconfigured after configuration and all add/erase methods have no effect after configuration.
*/
struct ZC_GUI__WinImmutable : public ZC_GUI_WindowWrap<ZC_GUI_WinImmutable>
{
    ZC_GUI__WinImmutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow = {});
    ZC_GUI__WinImmutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow = {});
};


    //  ZC_GUI__InputWindow

struct ZC_GUI__InputWindow
{
    //  Sets color for Input window.
    static void SetColors(uint textInput_background, uint textInput_text, uint textInput_caret, uint texInput_highlight);

    /*
    Start drawing input window.

    Params:
    - bl_x - window position X.
    - bl_y - window positioin Y.
    - win_width - window pixel width.
    - _max_symbols - max symbols to be input.
    - wstr - start text.
    - _callBack - function to call on input end, if text was changed.
    - highlight_text - highlight wstr on start or not.

    Return:
    false on fail, otherwise true.
    */
    static bool StartInputWindow(float bl_x, float bl_y, int win_width, int _max_symbols, const std::wstring& wstr, ZC_Function<void(const std::wstring&)>&& _callBack, bool highlight_text);
    
    /*
    Starts drawing a number input window,
    
    Params:
    - bl_x - window position X.
    - bl_y - window positioin Y.
    - win_width - window pixel width.
    - numberInput - look ZC_GUI_TextInputWindow::NumberInput() ctr.
    - highlight_text - highlight wstr on start or not.
    - _max_symbols - if less or equal 0, takes some valid count.

    Return:
    false on fail, otherwise true.
    */
    template <ZC_GUI_Number::cNumber TNum>
    static bool StartInputNumberWindow(float bl_x, float bl_y, int win_width, ZC_GUI_TextInputWindow::NumberInput<TNum>&& numberInput, bool highlight_text, int _max_symbols)
    {
        return ZC_GUI_TextInputWindow::StartInputNumberWindow(bl_x, bl_y, win_width, std::move(numberInput), highlight_text, _max_symbols);
    }
};