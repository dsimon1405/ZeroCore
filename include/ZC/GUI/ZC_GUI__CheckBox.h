#pragma once

#include "ZC_GUI__Obj.h"

#include <ZC/GUI/Backend/ZC_GUI_CheckBox.h>

struct ZC_GUI__CheckBox : public ZC_GUI__Obj<ZC_GUI_CheckBox>
{
    /*
    Params:
    - name - check box name.
    - _isOn - if true, on start on, otherwise off.
    - _callback - function calls on checkbox change state. Parameter bool is current check box state.
    - colorsCheckBox - buttons collors (may stay default).
    */
    ZC_GUI__CheckBox(const std::wstring& name, bool _isOn, ZC_Function<void(bool)> _callback, const ZC_GUI_ColorsCheckBox& colorsCheckBox = {});

        //  Return chaeck box state.
    bool GetState();
    
    /*
    Changes check box state.
    
    Params:
    - use_callback - if true will use callback setted in constructor.
    */
    void ChangeState(bool use_callback);
};