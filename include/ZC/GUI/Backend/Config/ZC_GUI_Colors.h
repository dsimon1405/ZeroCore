#pragma once

#include <ZC/Tools/Math/ZC_Math.h>

namespace ZC_GUI_Colors
{
    //  Widnow
    extern uint window;
    extern uint window_focused;
        //  scroll
    extern uint window_scroll_background;
    extern uint window_scroll_caret;
    extern uint window_scroll_caret_under_cursor;
    extern uint window_scroll_caret_pressed;
        //  frame
    extern uint window_frame;
        //  text
    extern uint window_text;
    
    //  Tree
    extern uint tree_background;
        //  arrow
    extern uint tree_branch_arrow;
    extern uint tree_branch_arrow_under_cursor;
    extern uint tree_branch_arrow_pressed;
        //  button
    extern uint tree_branch_button;
    extern uint tree_branch_button_under_cursor;
    extern uint tree_branch_button_pressed;
        //  text
    extern uint tree_branch_text;
        //  scroll
    extern uint tree_scroll_background;
    extern uint tree_scroll_caret;
    extern uint tree_scroll_caret_under_cursor;
    extern uint tree_scroll_caret_pressed;

    //  Button (for Window background)
    extern uint button;
    extern uint button_under_cursor;
    extern uint button_pressed;
    extern uint button_not_active;
    extern uint button_text;
    extern uint button_text_not_active;

    //  CheckBox
    extern uint checkBox_button;
    extern uint checkBox_button_under_cursor;
    extern uint checkBox_button_pressed;
        //  arrow
    extern uint checkBox_arrow;
        //  text
    extern uint checkBox_text;

    //  ButtonNumber
        //  button
    extern uint number_button;
    extern uint number_button_under_cursor;
    extern uint number_button_pressed;
        //  text
    extern uint number_text;
        //  arrows
    extern uint number_arrow;
    extern uint number_arrow_under_cursor;
    extern uint number_arrow_pressed;

    //  TextInput
    extern uint textInput_background;
    extern uint textInput_text;
    extern uint textInput_caret;
    extern uint texInput_highlight;

    //  DropDownSwitch
    extern uint dropDownSwitch_button;
    extern uint dropDownSwitch_button_under_cursor;
    extern uint dropDownSwitch_button_pressed;
    extern uint dropDownSwitch_text;
    extern uint dropDownSwitch_arrow;
};

