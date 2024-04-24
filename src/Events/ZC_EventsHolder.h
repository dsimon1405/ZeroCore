#pragma once

#include "ZC_Button.h"
#include "ZC_Mouse.h"

struct ZC_Events;

class ZC_EventsHolder
{
    friend struct ZC_Events;
public:
    static ZC_uptr<ZC_EventsHolder> MakeEventsHolder();
    
    virtual ~ZC_EventsHolder();

    virtual bool PollEvents(float previousFrameTime) = 0;

    void GetCursorPosition(float& posX, float& posY);

protected:
    ZC_Button button;
    ZC_Mouse mouse;
    ZC_Signal<void(float)> sigHandleEventsEnd { false };
    ZC_Signal<void(float,float)> sigWindowResize { false };

    ZC_EventsHolder();

private:
    static inline ZC_EventsHolder* pEventsHolder;
};