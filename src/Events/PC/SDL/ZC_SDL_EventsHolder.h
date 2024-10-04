#pragma once

#include <Events/ZC_EventsHolder.h>

struct ZC_SDL_EventsHolder : public ZC_EventsHolder
{
    void PollEvents(float previousFrameTime) override;
};