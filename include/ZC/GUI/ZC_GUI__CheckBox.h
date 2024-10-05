#pragma once

#include "ZC_GUI__Obj.h"
#include <ZC/GUI/Backend/ZC_GUI_CheckBox.h>

class ZC_GUI__CheckBox : public ZC_GUI__Obj<ZC_GUI_CheckBox>
{
public:
    /*
    Params:
    - name_pos - position of the name relative to the button. May be only OutOfButtonRight or OutOfButtonLeft, trying to set other will set OutOfButtonRight.
    - name - checkbox name.
    - _isOn - start state.
    - _callback_state_changed - function called when state changed.
    - _callback_focuse_changed - the function is called when the focus change. If true - in focus.
    - colorsCheckBox - colors.
    - immutable_name - must be false to change name after creation.
    - reserve_name_width - width in pixels of the longest name (ignores if immutable_name true)
    - name_alignment - name alignment in the texture if the name width in pixels is less then reserve_name_vidth.
    */
    ZC_GUI__CheckBox(ZC_GUI_TFB_Indent_Location name_pos, const std::wstring& name, bool _isOn, ZC_Function<void(bool)>&& _callback_state_changed,
        ZC_Function<void(bool)>&& _callback_focuse_changed, const ZC_GUI_ColorsCheckBox& colorsCheckBox = {}, bool immutable_name = true, int reserve_name_width = 0,
        ZC_GUI_TextAlignment name_alignment = ZC_GUI_TextAlignment::Left);

        //  Return chaeck box state.
    bool GetState();
    
    /*
    Changes check box state.
    
    Params:
    - use_callback - if true will use callback setted in constructor.
    */
    void ChangeState(bool use_callback);

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

private:
    ZC_Function<void(bool)> callback_focus_changed;
};