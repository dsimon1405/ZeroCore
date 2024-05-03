#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/Events/ZC_EventConnection.h>
#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_ConnectedButton.h"

#include <forward_list>
#include <list>

class ZC_ButtonHolder;

class ZC_ButtonPressedDown
{
public:
    ZC_ButtonPressedDown(ZC_ButtonHolder* _pButtonHolder);

    ZC_EC Connect(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& function);
    void Disconnect(const void* pFunc);
    //  returns true if button was allready pressed, otherwise false and cheks connetion state and set in isConnected
    bool AddActiveDownButton(ZC_ButtonID buttonId, bool& isConnected);
    //  returns true if button connected, calls on button up
    bool EraseActiveDownButton(ZC_ButtonID buttonId);
    //  calls active buttons wich have valid pointer on forward_list of down buttons
    void CallPressedButtons(float time);
    bool IsButtonDownConnected(ZC_ButtonID buttonId);

private:
    ZC_ButtonHolder* pButtonHolder;
    std::forward_list<ZC_ConnectedButton<ZC_Function<void(ZC_ButtonID, float)>>> connectedButtonDowns;
    std::list<ZC_ConnectedButton<ZC_Function<void(ZC_ButtonID, float)>*>> pressedButtonDowns;
};