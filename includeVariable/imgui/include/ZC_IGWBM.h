#pragma once

#include "ZC_IGWindow.h"
#include <ZC/Tools/ZC_ButtonManipulator.h>

/*
Class combine functionality of ZC_IGWindow and ZC_ButtonManipulator classes. Heir must override ZC_ButtonManipulator::VCallButtonDownBM(ZC_ButtonID buttonId, float time).
Fucntionality of heir is to (draw/not draw) and in the same time (connect/disconnect) event to buttons realized in VCallButtonDownBM(...). One heir must be floor level
(isFloor = true)!
*/
struct ZC_IGWBM : public ZC_IGWindow, protected ZC_ButtonManipulator
{
    /*
    Create ImGui window for heir with buttons events functionality.

    Params:
    - _activateButtonId - button for activation.
    - _isLadder - indicates whether the heir is a ladder type or not (can't be ladder and floor at the same time).
    - _isFloor - indicates whether the heir is a floor type or not, one heir must have this parameter true (can't be ladder and floor at the same time).
    - _use_activateButtonId_ForDeactivation - use _activateButtonId for deactivation or not (no metter, if isFloor = false).
    - unicName - unic name for window.
    - needDraw - window must be draw after creation.
    - _width - window width (no metter, if setted _igwf flag ImGuiWindowFlags_AlwaysAutoResize).
    - _height - window height (no metter, if setted _igwf flag ImGuiWindowFlags_AlwaysAutoResize).
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
    ZC_IGWBM(ZC_ButtonID _activateButtonId, bool _isStaircase, bool isFloor, bool _use_activateButtonId_ForDeactivation, std::string&& unicName,
        bool needDraw, float _width, float _height, float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags, bool _mayClose, int _igwf);

    bool IsEventsTargetIGWBM() const noexcept;
    bool IsActiveIGWBM() const noexcept;

private:
    virtual void VActivateIGWBM() {}      //  if need some activation on activate
    virtual void VDeactivateIGWBM() {}    //  if need some deactivation on deactivate
    
    void VActivateBM() override;
    void VDeactivateBM() override;
    void VFocusStateChangedIGW(bool _isFocused) override;
};