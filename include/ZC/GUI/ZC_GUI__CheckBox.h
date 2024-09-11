#pragma once

#include "ZC_GUI__Obj.h"

#include <ZC/GUI/Backend/ZC_GUI_CheckBox.h>

class ZC_GUI__CheckBox;

struct ZC_GUI__ChB : public ZC_GUI_CheckBox
{
    ZC_GUI__CheckBox* pHolder;

    ZC_GUI__ChB(ZC_GUI__CheckBox* _pHolder, ZC_GUI_CheckBox&& checkBox);

    void VStateChanged_CB(bool state) override;
};

/*
CheckBox struct.
Must be overrid:
- void VStateChanged(bool state) -> Call on state changing.
*/
struct ZC_GUI__CheckBox : public ZC_GUI__Obj<ZC_GUI__ChB>
{
    /*
    Params:
    - name - check box name.
    - _isOn - if true, on start on, otherwise off.
    - colorsCheckBox - buttons collors (may stay default).
    */
    ZC_GUI__CheckBox(const std::wstring& name, bool _isOn, const ZC_GUI_ColorsCheckBox& colorsCheckBox = {});

    /*
    Calls on state changing.
    
    Params:
    - state - true if check box on, otherwise false.
    */
    virtual void VStateChanged(bool state) {}
};