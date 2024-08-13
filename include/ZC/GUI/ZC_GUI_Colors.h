#pragma once

#include <ZC/Tools/Math/ZC_Math.h>

namespace ZC_GUI_Colors
{
    //  Text
    const uint text = ZC_PackColorUCharToUInt(180, 180, 180);
    
    //  Tree
    const uint tree_background = ZC_PackColorUCharToUInt(40, 40, 40);
        //  arrow
    const uint tree_branch_arrow = ZC_PackColorUCharToUInt(100, 100, 100);
    const uint tree_branch_arrow_under_cursor = ZC_PackColorUCharToUInt(160, 160, 160);
    const uint tree_branch_arrow_pressed = ZC_PackColorUCharToUInt(200, 200, 200);
        //  button
    const uint tree_branch_button = tree_background;
    const uint tree_branch_button_under_cursor = ZC_PackColorUCharToUInt(70, 70, 70);
    const uint tree_branch_button_pressed = ZC_PackColorUCharToUInt(100, 100, 100);
        //  text
    const uint tree_branch_text = text;
        //  scroll
    const uint tree_scroll_background = ZC_PackColorUCharToUInt(25, 25, 25);
    const uint tree_scroll_caret = ZC_PackColorUCharToUInt(90, 90, 90);
    const uint tree_scroll_caret_under_cursor = ZC_PackColorUCharToUInt(110, 110, 110);
    const uint tree_scroll_caret_pressed = ZC_PackColorUCharToUInt(130, 130, 130);

    //  Button (for Window background)
    const uint button = ZC_PackColorUCharToUInt(80, 80, 80);
    const uint button_under_cursor = ZC_PackColorUCharToUInt(110, 110, 110);
    const uint button_pressed = ZC_PackColorUCharToUInt(180, 180, 180);

    //  Widnow
    const uint window = ZC_PackColorUCharToUInt(50,50,50);
    const uint window_focused = ZC_PackColorUCharToUInt(60,60,60);
        //  scroll
    const uint window_scroll_background = ZC_PackColorUCharToUInt(70, 70, 70);
    const uint window_scroll_caret = ZC_PackColorUCharToUInt(100, 100, 100);
    const uint window_scroll_caret_under_cursor = ZC_PackColorUCharToUInt(120, 120, 120);
    const uint window_scroll_caret_pressed = ZC_PackColorUCharToUInt(140, 140, 140);
        //  frame
    const uint window_frame = ZC_PackColorUCharToUInt(30, 30, 30);

    // const unsigned int  = ZC_PackColorUCharToUInt(, , );
};