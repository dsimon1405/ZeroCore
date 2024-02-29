#include "ZC_Button.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>

ZC_SConnection ZC_Button::ConnectDown(ZC_ButtonID buttonId, ZC_Function<void(float)>&& function)
{
    auto downConnectedButtonsIter = std::find(downConnectedButtons.begin(), downConnectedButtons.end(), buttonId);
    if (downConnectedButtonsIter == downConnectedButtons.end())
    {
        auto& rConnectedButton = downConnectedButtons.emplace_back(ConnectedButton{ buttonId });    //  adds button to connected
        auto sConnection = rConnectedButton.sigFunctions.Connect(std::move(function));  //  connects to button signal and get return value ZC_SConnection
        //  if button already active, adds pointer on button signal to actives
        auto activeDownButtonsIter = std::find(activeDownButtons.begin(), activeDownButtons.end(), buttonId);
        if (activeDownButtonsIter != activeDownButtons.end()) activeDownButtonsIter->psigFunctions = &(rConnectedButton.sigFunctions);
        return sConnection;
    }
    else return downConnectedButtonsIter->sigFunctions.Connect(std::move(function));
}

ZC_SConnection ZC_Button::ConnectUp(ZC_ButtonID buttonId, ZC_Function<void(float)>&& function)
{
    auto upConnectedButtonsIter = std::find(upConnectedButtons.begin(), upConnectedButtons.end(), buttonId);
    return upConnectedButtonsIter != upConnectedButtons.end() ? upConnectedButtonsIter->sigFunctions.Connect(std::move(function))
        :  upConnectedButtons.emplace_back(ConnectedButton{ buttonId }).sigFunctions.Connect(std::move(function));
}

void ZC_Button::AddActiveDownButton(ZC_ButtonID buttonId)
{
    auto activeDownButtonsIter = std::find(activeDownButtons.begin(), activeDownButtons.end(), buttonId);
    if (activeDownButtonsIter == activeDownButtons.end())   //  add active button if not already added
    {   //  if button connected as (down button) add pointer on downConnectedButtons sigFunctions, otherwise nullptr
        auto downConnectedButtonsIter = std::find(downConnectedButtons.begin(), downConnectedButtons.end(), buttonId);
        if (downConnectedButtonsIter == downConnectedButtons.end()) activeDownButtons.emplace_front(ActiveDownButton{ buttonId, nullptr });
        else if (downConnectedButtonsIter->sigFunctions.IsEmpty())
        {   //  if button already disconnect from down, erases from down
            downConnectedButtons.erase(downConnectedButtonsIter);
            activeDownButtons.emplace_front(ActiveDownButton{ buttonId, nullptr });
        }
        else activeDownButtons.emplace_front(ActiveDownButton{ buttonId, &(downConnectedButtonsIter->sigFunctions) });
    }
}

void ZC_Button::CallUpButton(ZC_ButtonID buttonId, float prevFrameTime)
{
    auto upConnectedButtonsIter = std::find(upConnectedButtons.begin(), upConnectedButtons.end(), buttonId);
    if (upConnectedButtonsIter != upConnectedButtons.end())
    {
        upConnectedButtonsIter->sigFunctions(prevFrameTime); //  calls a button signal(signal will connect/disconnect functions and call that connected)
        if (upConnectedButtonsIter->sigFunctions.IsEmpty()) upConnectedButtons.erase(upConnectedButtonsIter);   //  if signal became empty, erases from up buttons
    }
    ZC_ForwardListErase(activeDownButtons, buttonId);   //  erases that button from active down buttons (if it there)
}

void ZC_Button::CallActiveButtons(float prevFrameTime)
{
    for (auto& activeDownButton : activeDownButtons)
    {
        if (!(activeDownButton.psigFunctions)) continue;    //  if nullptr on a button signal, continue
        (*(activeDownButton.psigFunctions))(prevFrameTime); //  calls an active down button signal
        if (activeDownButton.psigFunctions->IsEmpty())
        {   //  if after signals call signal empty(no connected function on that button), erase that button from downConnectedButtons
            downConnectedButtons.erase(std::find(downConnectedButtons.begin(), downConnectedButtons.end(), activeDownButton.buttonId));
            activeDownButton.psigFunctions = nullptr;   //  make nullptr on a button signal
        }
    }
}


//  ConnectedButton

bool ZC_Button::ConnectedButton::operator == (ZC_ButtonID _buttonId)
{
    return buttonId == _buttonId;
}


//  ActiveDownButton

bool ZC_Button::ActiveDownButton::operator == (ZC_ButtonID _buttonId)
{
    return buttonId == _buttonId;
}