#pragma once

#include "Button/ZC_ButtonHolder.h"
#include "ZC_Mouse.h"

struct ZC_Events;

class ZC_EventsHolder
{
    friend struct ZC_Events;
public:
    static ZC_uptr<ZC_EventsHolder> MakeEventsHolder();
    
    virtual ~ZC_EventsHolder();

    virtual void PollEvents(float previousFrameTime) = 0;

    void GetCursorPosition(float& posX, float& posY);

protected:
    ZC_ButtonHolder buttonHolder;
    ZC_Mouse mouse;
    ZC_ESignal<void(float)> sigHandleEventsEnd;
    ZC_ESignal<void(float,float)> sigWindowResize;
    ZC_ESignal<void(float)> sigHandleEventsStart;
    ZC_Function<void()> funcWindowCloseButton;

    ZC_EventsHolder();

private:
    static inline ZC_EventsHolder* pEventsHolder;
};