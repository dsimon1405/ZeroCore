#pragma once

#include "ZC_ConnectedButton.h"
#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Events/ZC_EC.h>

#include <forward_list>

struct ZC_ButtonUp
{
    std::forward_list<ZC_ConnectedButton<ZC_Function<void(ZC_ButtonID, float)>>> connectedButtonUps;

    ZC_EC Connect(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& function);
    void Disconnect(const void* pFuncData);
    void CallButtonUp(ZC_ButtonID buttonId, float time);
};