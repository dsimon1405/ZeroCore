#include "ZC_ButtonPressedDown.h"

#include "ZC_ButtonHolder.h"
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Events/ZC_ECTargetPointer.h>
#include <ZC/GUI/ZC_GUI.h>

#include <cassert>

ZC_ButtonPressedDown::ZC_ButtonPressedDown(ZC_ButtonHolder* _pButtonHolder)
    : pButtonHolder(_pButtonHolder)
{
    pBPD = this;
}

ZC_EC ZC_ButtonPressedDown::Connect(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& function)
{
    assert(!pButtonHolder->IsButtonDownConnected(buttonId));    //  down button allready connected some where
    return { new ZC_ECTargetPointer<ZC_ButtonPressedDown*>(this, connectedButtonDowns.emplace_front(buttonId, std::move(function)).func.GetPointerOnData()) };
}

void ZC_ButtonPressedDown::Disconnect(const void* pFunc)
{
    ZC_ForwardListErase(connectedButtonDowns, pFunc);
        //  if disconnected button active, make pointer on active function = nullptr
    auto pActiveButtonDown = ZC_Find(pressedButtonDowns, pFunc);
    if (pActiveButtonDown) pActiveButtonDown->func = nullptr;
}

bool ZC_ButtonPressedDown::AddActiveDownButton(ZC_ButtonID buttonId, bool& isConnected)
{
    auto pPressedButton = ZC_Find(pressedButtonDowns, buttonId);
    if (pPressedButton)     //  button allready pressed
    {
        isConnected = pPressedButton->pGuiObj || pPressedButton->func;
        return true;
    }
    if ((buttonId == ZC_ButtonID::M_LEFT && ZC_Find(pressedButtonDowns, ZC_ButtonID::M_RIGHT))
        || (buttonId == ZC_ButtonID::M_RIGHT && ZC_Find(pressedButtonDowns, ZC_ButtonID::M_LEFT)))   //  mbl pressed, avoid start mbr press, till mbl release
    {
        pressedButtonDowns.emplace_back(ZC_ConnectedButton<ZC_Function<void(ZC_ButtonID, float)>*>{ buttonId, nullptr, nullptr});
        return false;
    }
    
    ZC_GUI_Obj* pGuiObj = ZC_GUI::pGUI ? ZC_GUI::pGUI->eventManager.GetButtonDownObject(buttonId) : nullptr;    //  find button in gui objects
    if (pGuiObj) isConnected = pressedButtonDowns.emplace_back(ZC_ConnectedButton<ZC_Function<void(ZC_ButtonID, float)>*>{ buttonId, nullptr, pGuiObj}).pGuiObj;
    else
    {
        auto pConnectedButtonDown = ZC_Find(connectedButtonDowns, buttonId);    //  find button function in connected events
            //  if button connected create ActiveDownButton with pointer on that function, otherwise with nullptr
        isConnected = pressedButtonDowns.emplace_back(ZC_ConnectedButton<ZC_Function<void(ZC_ButtonID, float)>*>
            { buttonId, pConnectedButtonDown ? &(pConnectedButtonDown->func) : nullptr}).func;
    
    }
    return false;
}

void ZC_ButtonPressedDown::TextButtonDown(unsigned char ch)
{
    if (ZC_GUI::pGUI) ZC_GUI::pGUI->textInputWindow.TextButtonDown(ch);
}

bool ZC_ButtonPressedDown::EraseActiveDownButton(ZC_ButtonID buttonId, float time)
{
    for (auto curIter = pressedButtonDowns.begin(); curIter != pressedButtonDowns.end(); ++curIter)
    {
        if (*curIter == buttonId)
        {
            bool isEventUsed = curIter->ButtonUp_GUIObj(buttonId, time);
            pressedButtonDowns.erase(curIter);
            return isEventUsed;
        }
    }
    return false;
}

void ZC_ButtonPressedDown::CallPressedButtons(float time)
{
    for (auto& activeButtonDown : pressedButtonDowns) activeButtonDown.Call(time); //  call active button's function
}

bool ZC_ButtonPressedDown::IsButtonDownConnected(ZC_ButtonID buttonId)
{
    return ZC_Find(connectedButtonDowns, buttonId) != nullptr;
}

void ZC_ButtonPressedDown::DisablePressedButton(void* pGUI_Obj)
{
    ZC_Find(pBPD->pressedButtonDowns, static_cast<ZC_GUI_Obj*>(pGUI_Obj))->pGuiObj = nullptr;
}

bool ZC_ButtonPressedDown::IsButtonPressed(ZC_ButtonID buttonId)
{
    return ZC_Find(pressedButtonDowns, buttonId);
}