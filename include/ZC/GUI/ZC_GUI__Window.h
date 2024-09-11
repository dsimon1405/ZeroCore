#pragma once

#include <ZC/GUI/Backend/Window/ZC_GUI_WinImmutable.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_WinMutable.h>

#include <ZC/GUI/Backend/ZC_GUI_IconUV.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_TextInputWindow.h>

template <typename TWin>
struct ZC_GUI__Window
{
public:
    ZC_GUI__Window(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow = {});

    virtual ~ZC_GUI__Window() = default;
    
    /*
    Add new row in window. For immutable windows have effect only before configuration.

    Params:
    - row - row with objects or withought.
    - pRow_prev - row after wich will be added row. Not required.
    
    Return:
    Ob success - pointer on added row, otherwise nullptr.
    */
    const ZC_GUI_Row* AddRow(ZC_GUI_Row&& row, const ZC_GUI_Row* pRow_prev = nullptr);
    
    /*
    Erase row from window. For immutable windows have effect only before configuration.

    Pararms:
    - row - row to erase.
    */
    void EraseRow(const ZC_GUI_Row* row);

    /*
    Add object in last added row. If no row, creates default row. For immutable windows have effect only before configuration.

    Params:
    - pObj - object to add.
    - pObj_prev - object if it's in row, after wich to add pObj.

    Return:
    On success true, otherise false.
    */
    bool AddObj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev = nullptr);
    
    /*
    Add object to concrete row. For immutable windows have effect only before configuration.
    
    Params:
    - pRow - edit row.
    - pObj - object tot add.
    - pObj_prev - object if it's in row, after wich to add pObj.

    Retrun:
    On success true, otherise false.
    */
    bool AddObjInRow(const ZC_GUI_Row* pRow, ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev = nullptr);

    //  Erase object from row.
    void EraseObj(ZC_GUI_Obj* pObj);

    //  Return true if drawn, otherwise false.
    bool IsDrawing();

    //  Start/end draw window.
    void SetDrawState(bool needDraw);

private:
    TWin win;
};

template <typename TWin>
ZC_GUI__Window<TWin>::ZC_GUI__Window(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow)
    : win(_woiData, uv, _winFlags, colorsWindow)
{}
    
template <typename TWin>
const ZC_GUI_Row* ZC_GUI__Window<TWin>::AddRow(ZC_GUI_Row&& row, const ZC_GUI_Row* pRow_prev)
{
    return win.AddRow(std::move(row), pRow_prev);
}

template <typename TWin>
void ZC_GUI__Window<TWin>::EraseRow(const ZC_GUI_Row* row)
{
    win.EraseRow(row);
}

template <typename TWin>
bool ZC_GUI__Window<TWin>::AddObj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev)
{
    return win.VAddObj_Obj(pObj, pObj_prev);
}

template <typename TWin>
bool ZC_GUI__Window<TWin>::AddObjInRow(const ZC_GUI_Row* pRow, ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev)
{
    return win.AddObjInRow(pRow, pObj, pObj_prev);
}

template <typename TWin>
void ZC_GUI__Window<TWin>::EraseObj(ZC_GUI_Obj* pObj)
{
    win.EraseObj(pObj);
}

template <typename TWin>
bool ZC_GUI__Window<TWin>::IsDrawing()
{
    return win.VIsDrawing_Obj();
}

template <typename TWin>
void ZC_GUI__Window<TWin>::SetDrawState(bool needDraw)
{
    win.VSetDrawState_W(needDraw);
}


    //  ZC_GUI__WinMutable

struct ZC_GUI__WinMutable : public ZC_GUI__Window<ZC_GUI_WinMutable>
{
    ZC_GUI__WinMutable(const ZC_WOIData& _woiData, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow = {});
    ZC_GUI__WinMutable(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow = {});
};

    //  ZC_GUI__WinImmutable

struct ZC_GUI__WinImmutable : public ZC_GUI__Window<ZC_GUI_WinImmutable>
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