#include "ZC_ButtonUp.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/Events/ZC_ECTargetPointer.h>

#include <cassert>

ZC_EC ZC_ButtonUp::Connect(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& function)
{
    assert(!ZC_Find(connectedButtonUps, buttonId));   //  same button
    connectedButtonUps.emplace_front(ZC_ConnectedButton{ buttonId, std::move(function) });
    return { new ZC_ECTargetPointer<ZC_ButtonUp*, ZC_ButtonID>(this, buttonId) };
}

void ZC_ButtonUp::Disconnect(ZC_ButtonID buttonId)
{
    ZC_ForwardListErase(connectedButtonUps, buttonId);
}

void ZC_ButtonUp::CallButtonUp(ZC_ButtonID buttonId, float time)
{
    auto pConnectedButtonUp = ZC_Find(connectedButtonUps, buttonId);
    if (pConnectedButtonUp) pConnectedButtonUp->func(buttonId, time);
}