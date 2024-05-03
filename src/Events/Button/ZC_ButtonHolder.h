#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include "ZC_ButtonPressedDown.h"
#include "ZC_ButtonClick.h"
#include "ZC_ButtonUp.h"
#include <ZC/Events/ZC_ESignal.h>

struct ZC_ButtonHolder
{
    ZC_ButtonPressedDown buttonPressedDown { this };
    ZC_ButtonClick buttonClick { this };
    ZC_ButtonUp buttonUp;
    ZC_ESignal<void(ZC_ButtonID, float)> esigFirstDownButton;

    void ButtonDown(ZC_ButtonID buttonId, float time);
    void ButtonUp(ZC_ButtonID buttonId, float time);
    //  returns true if down button allready connected somewhere
    bool IsButtonDownConnected(ZC_ButtonID buttonId);
};