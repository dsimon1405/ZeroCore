#pragma once

#include <ZC/GUI/ZC_GUI_WinImmutable.h>
#include <ZC/GUI/ZC_GUI_Text.h>

#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/GUI/ZC_GUI.h> 

struct ZC_GUI_TextInputWindow : protected ZC_GUI_WinImmutable
{
    int win_buttonOffset = 0;
    

    ZC_GUI_TextInputWindow()
        : ZC_GUI_WinImmutable(ZC_WOIData{ .width = 0.f, .height = 0.f, .indentX = 0.f, .indentY = 0.f, .indentFlags = ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel }, 0)
    {
        this->pObjData->color = ZC_PackColorUCharToUInt(30, 30, 30);
        
        int fontHeight = ZC_GUI::pGUI->textManager.GetFontHeight();
        win_buttonOffset = fontHeight / 7;  //  takes 1/7 font height for top and bottom space
        this->pObjData->height = fontHeight + (win_buttonOffset * 2);    // (* 2) top and bottom
    }


};