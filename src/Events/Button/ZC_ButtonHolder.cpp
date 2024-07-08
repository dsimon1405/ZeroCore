#include "ZC_ButtonHolder.h"

void ZC_ButtonHolder::ButtonDown(ZC_ButtonID buttonId, float time)
{
    bool isButtonConnected = false;
    //  calling classes that use down button, till one of classes use button (that meens that button connected in that class)
    if (!buttonPressedDown.AddActiveDownButton(buttonId, isButtonConnected))    //  if button was not pressed continue check
    {
        if (!isButtonConnected) buttonClick.CallButtonDown(buttonId, time);     //  if button was not connected in buttonPressedDown check connection in buttonClick
        esigFirstDownButton(buttonId, time);   //  its first click for that button, so call esigFirstDownButton
    }
}

void ZC_ButtonHolder::ButtonUp(ZC_ButtonID buttonId, float time)
{
    if (!buttonPressedDown.EraseActiveDownButton(buttonId, time))
    {
        if (!buttonClick.CallButtonUp(buttonId, time))
            buttonUp.CallButtonUp(buttonId, time);
    }
    else buttonClick.ButtonWasReleased(buttonId);
}

bool ZC_ButtonHolder::IsButtonDownConnected(ZC_ButtonID buttonId)
{
    return buttonPressedDown.IsButtonDownConnected(buttonId) || buttonClick.IsButtonDownConnected(buttonId);
}