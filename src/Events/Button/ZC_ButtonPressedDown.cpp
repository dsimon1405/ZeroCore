#include "ZC_ButtonPressedDown.h"

#include "ZC_ButtonHolder.h"
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Events/ZC_ECTargetPointer.h>

#include <cassert>

ZC_ButtonPressedDown::ZC_ButtonPressedDown(ZC_ButtonHolder* _pButtonHolder)
    : pButtonHolder(_pButtonHolder)
{}

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
    if (ZC_Find(pressedButtonDowns, buttonId)) return true;     //  button allready pressed
    
    auto pConnectedButtonDown = ZC_Find(connectedButtonDowns, buttonId);
    //  if button connected create ActiveDownButton with pointer on that function, otherwise with nullptr
    pressedButtonDowns.emplace_back(ZC_ConnectedButton<ZC_Function<void(ZC_ButtonID, float)>*>{ buttonId, pConnectedButtonDown ? &pConnectedButtonDown->func : nullptr });
    isConnected = pConnectedButtonDown;
    return false;
}

bool ZC_ButtonPressedDown::EraseActiveDownButton(ZC_ButtonID buttonId)
{
    for (auto curIter = pressedButtonDowns.begin(); curIter != pressedButtonDowns.end(); ++curIter)
    {
        if (*curIter == buttonId)
        {
            bool isConnected = curIter->func;   //  if active button have not nullptr, in calling function (func), then button down connected
            pressedButtonDowns.erase(curIter);
            return isConnected;
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