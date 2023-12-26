#pragma once

#include <ZC/Tools/Signal/ZC_Signal.h>

class ZC_Event
{
public:
    virtual ~ZC_Event() = default;

    /*
    Binds a function to an event that signals the end of events in this frame.

    Params:
    func - binding function.
    */
    static void ConnectEventsEnd(ZC_Function<void()>&& func);

protected:
    static inline ZC_Signal<void()> sEventsEnd {};

    ZC_Event() = default;
};