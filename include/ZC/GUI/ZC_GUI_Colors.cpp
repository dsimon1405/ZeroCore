#include "ZC_GUI_Colors.h"

#include <ZC/Tools/Math/ZC_Math.h>

namespace ZC_GUI_Colors
{
    //  Widnow
    uint window = ZC_PackColorUCharToUInt(50,50,50);
    uint window_focused = ZC_PackColorUCharToUInt(60,60,60);
        //  scroll
    uint window_scroll_background = ZC_PackColorUCharToUInt(70, 70, 70);
    uint window_scroll_caret = ZC_PackColorUCharToUInt(100, 100, 100);
    uint window_scroll_caret_under_cursor = ZC_PackColorUCharToUInt(120, 120, 120);
    uint window_scroll_caret_pressed = ZC_PackColorUCharToUInt(140, 140, 140);
        //  frame
    uint window_frame = ZC_PackColorUCharToUInt(30, 30, 30);
        //  text
    uint window_text = ZC_PackColorUCharToUInt(190, 190, 190);
    
    //  Tree
    uint tree_background = ZC_PackColorUCharToUInt(40, 40, 40);
        //  arrow
    uint tree_branch_arrow = ZC_PackColorUCharToUInt(130, 130, 130);
    uint tree_branch_arrow_under_cursor = ZC_PackColorUCharToUInt(170, 170, 170);
    uint tree_branch_arrow_pressed = ZC_PackColorUCharToUInt(200, 200, 200);
        //  button
    uint tree_branch_button = tree_background;
    uint tree_branch_button_under_cursor = ZC_PackColorUCharToUInt(70, 70, 70);
    uint tree_branch_button_pressed = ZC_PackColorUCharToUInt(100, 100, 100);
        //  text
    uint tree_branch_text = window_text;
        //  scroll
    uint tree_scroll_background = ZC_PackColorUCharToUInt(25, 25, 25);
    uint tree_scroll_caret = ZC_PackColorUCharToUInt(90, 90, 90);
    uint tree_scroll_caret_under_cursor = ZC_PackColorUCharToUInt(110, 110, 110);
    uint tree_scroll_caret_pressed = ZC_PackColorUCharToUInt(130, 130, 130);

    //  Button (for Window background)
    uint button = ZC_PackColorUCharToUInt(100, 100, 100);
    uint button_under_cursor = ZC_PackColorUCharToUInt(115, 115, 115);
    uint button_pressed = ZC_PackColorUCharToUInt(130, 130, 130);

    //  CheckBox
    uint checkBox_button = button;
    uint checkBox_button_under_cursor = button_under_cursor;
    uint checkBox_button_pressed = button_pressed;
        //  arrow
    uint checkBox_arrow = ZC_PackColorUCharToUInt(200, 200, 200);
        //  text
    uint checkBox_text = window_text;

    //  Number
        //  button
    uint number_button = button;
    uint number_button_under_cursor = ZC_PackColorUCharToUInt(110, 110, 110);
    uint number_button_pressed = ZC_PackColorUCharToUInt(120, 120, 120);;
        //  text
    uint number_text = ZC_PackColorUCharToUInt(200, 200, 200);
        //  arrows
    uint number_arrow = ZC_PackColorUCharToUInt(140, 140, 140);
    uint number_arrow_under_cursor = ZC_PackColorUCharToUInt(180, 180, 180);
    uint number_arrow_pressed = ZC_PackColorUCharToUInt(210, 210, 210);

    //  TextInput
    uint textInput_background = ZC_PackColorUCharToUInt(20, 20, 20);
    uint textInput_text = ZC_PackColorUCharToUInt(220, 220, 220);
    uint textInput_caret = textInput_text;
    uint texInput_highlight = ZC_PackColorUCharToUInt(122, 60, 50);    //  black


    // const unsigned int  = ZC_PackColorUCharToUInt(, , );
};

