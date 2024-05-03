#pragma once

#include "ZC_IGWindow.h"
#include <ZC/Tools/ZC_ButtonManipulator.h>

/*
Class combine functionality of ZC_IGWindow and ZC_ButtonManipulator classes. Heir must override ZC_ButtonManipulator::VCallButtonDownBM(ZC_ButtonID buttonId, float time).
Fucntionality of heir is to (draw/not draw) and in the same time (connect/disconnect) event to buttons realized in VCallButtonDownBM(...).
*/
struct ZC_IGWBM : public ZC_IGWindow, protected ZC_ButtonManipulator
{
    /*
    Create ImGui window for heir with buttons events functionality.

    Params:
    - unicName - unic name for window.
    - needDraw - window must be draw after creation.
    - _width - window width.
    - _height - window height.
    - _indentX - value of horizontal indent from border of global window. If used IndentFlag:
        X_Left_Pixel, X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        X_Left_Percent, X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        X_Center -> value no metter.
    - _indentX - value of vertival indent from border of global window. If used IndentFlag: 
        Y_Top_Pixel, Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        Y_Top_Percent, Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        Y_Center -> value no metter.
    - _indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    - _mayClose - window must have cross to close the window.
    - _igfw - ImGuiWindowFlags.
    */
    ZC_IGWBM(ZC_ButtonID _activateButtonId, bool _isStaircase, bool isFloor, std::string &&unicName, bool needDraw,
            float _width, float _height, float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags, bool _mayClose, int _igwf)
        : ZC_IGWindow(std::move(unicName), needDraw, _width, _height, _indentX, _indentY, _indentFlags, _mayClose, _igwf),
        ZC_ButtonManipulator(_activateButtonId, _isStaircase, isFloor)
    {
        if (needDraw) this->ActivateBM();
    }

private:
    void VActivateBM() override
    {
        this->NeedDrawIGW(true);   //  if pFloorBM became active in ZC_ButtonManipulator::DeactivateBM(), not throught ActivateIGWBM() that can change his draw state
    }

    void VDeactivateBM() override
    {
        this->NeedDrawIGW(this->IsActiveBM());
    }

    void VDrawStateChangedIGW(bool isDrawing) override
    {
        isDrawing ? this->ActivateBM() : this->DeactivateBM();  //  if changing draw state in ZC_IGWWindow (the window was covered with a cross)
    }
};