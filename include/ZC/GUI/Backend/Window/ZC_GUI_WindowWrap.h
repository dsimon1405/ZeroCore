#pragma once

#include <ZC/GUI/Backend/ZC_GUI_ObjBorder.h>
#include "ZC_GUI_Window.h"

template <typename TWin>
struct ZC_GUI_WindowWrap
{
public:
    ZC_GUI_WindowWrap(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow = {});

    virtual ~ZC_GUI_WindowWrap() = default;

    //  Sets scroll speed for all scrollable windows and other objects use scroll (ZC_GUI__Tree). Default value is 40.
    static void SetScrollSpeed(uint speed = 40);
    
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
void ZC_GUI_WindowWrap<TWin>::SetScrollSpeed(uint speed)
{
    ZC_GUI_ObjBorder::Scroll::scroll_speed = speed;
}

template <typename TWin>
ZC_GUI_WindowWrap<TWin>::ZC_GUI_WindowWrap(const ZC_WOIData& _woiData, const ZC_GUI_UV& uv, ZC_GUI_WinFlags _winFlags, const ZC_GUI_ColorsWindow& colorsWindow)
    : win(_woiData, uv, _winFlags, colorsWindow)
{}
    
template <typename TWin>
const ZC_GUI_Row* ZC_GUI_WindowWrap<TWin>::AddRow(ZC_GUI_Row&& row, const ZC_GUI_Row* pRow_prev)
{
    return win.AddRow(std::move(row), pRow_prev);
}

template <typename TWin>
void ZC_GUI_WindowWrap<TWin>::EraseRow(const ZC_GUI_Row* row)
{
    win.EraseRow(row);
}

template <typename TWin>
bool ZC_GUI_WindowWrap<TWin>::AddObj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev)
{
    return win.VAddObj_Obj(pObj, pObj_prev);
}

template <typename TWin>
bool ZC_GUI_WindowWrap<TWin>::AddObjInRow(const ZC_GUI_Row* pRow, ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev)
{
    return win.AddObjInRow(pRow, pObj, pObj_prev);
}

template <typename TWin>
void ZC_GUI_WindowWrap<TWin>::EraseObj(ZC_GUI_Obj* pObj)
{
    win.EraseObj(pObj);
}

template <typename TWin>
bool ZC_GUI_WindowWrap<TWin>::IsDrawing()
{
    return win.VIsDrawing_Obj();
}

template <typename TWin>
void ZC_GUI_WindowWrap<TWin>::SetDrawState(bool needDraw)
{
    win.VSetDrawState_W(needDraw);
}