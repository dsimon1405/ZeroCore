#include "ZC_ButtonClick.h"

#include "ZC_ButtonHolder.h"
#include <ZC/Events/ZC_ECTargetPointer.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <cassert>

ZC_ButtonClick::ZC_ButtonClick(ZC_ButtonHolder* _pButtonHolder)
    : pButtonHolder(_pButtonHolder)
{}

ZC_EC ZC_ButtonClick::Connect(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& funcDown, ZC_Function<void(ZC_ButtonID, float)>&& funcUp)
{
    assert(!pButtonHolder->IsButtonDownConnected(buttonId));    //  down button allready connected some where
    assert(funcDown || funcUp);    //  must be minimum one function
    return { new ZC_ECTargetPointer<ZC_ButtonClick*, const void*>(this,
        buttonHolders.emplace_front(ButtonHolder{ buttonId, std::move(funcDown), std::move(funcUp) }).funcDown.GetPointerOnData()) };
}

void ZC_ButtonClick::Disconnect(const void* pFuncDownData)
{
    ZC_ForwardListErase(buttonHolders, pFuncDownData);
}

bool ZC_ButtonClick::CallButtonDown(ZC_ButtonID buttonId, float time)
{
    auto pDownButtonHolder = ZC_Find(buttonHolders, buttonId);
    return pDownButtonHolder ? pDownButtonHolder->CallButtonDown(buttonId, time) : false;
}

bool ZC_ButtonClick::CallButtonUp(ZC_ButtonID buttonId, float time)
{
    auto pDownButtonHolder = ZC_Find(buttonHolders, buttonId);
    return pDownButtonHolder ? pDownButtonHolder->CallButtonUp(buttonId, time) : false;
}

bool ZC_ButtonClick::IsButtonDownConnected(ZC_ButtonID buttonId)
{
    return ZC_Find(buttonHolders, buttonId) != nullptr;
}


//  ButtonHolder

bool ZC_ButtonClick::ButtonHolder::operator == (ZC_ButtonID _buttonId) const noexcept
{
    return buttonId == _buttonId;
}

bool ZC_ButtonClick::ButtonHolder::operator == (const void* pFuncDownData) const noexcept
{
    return funcDown.GetPointerOnData() == pFuncDownData;
}

bool ZC_ButtonClick::ButtonHolder::CallButtonDown(ZC_ButtonID buttonId, float time)
{
    if (callDown)
    {
        if (funcDown) funcDown(buttonId, time);   //  if callDown, calls down function
        callDown = false;
        return true;
    }
    return false;
}

bool ZC_ButtonClick::ButtonHolder::CallButtonUp(ZC_ButtonID buttonId, float time)
{
    if (!callDown)
    {
        if (funcUp) funcUp(buttonId, time);  //  if not callDown and if exists up function, calls
        callDown = true;
        return true;
    }
    return false;
}