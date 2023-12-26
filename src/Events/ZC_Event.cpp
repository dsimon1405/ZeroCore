#include <ZC/Events/ZC_Event.h>

void ZC_Event::ConnectEventsEnd(ZC_Function<void()>&& func)
{
    sEventsEnd.Connect(std::move(func));
}