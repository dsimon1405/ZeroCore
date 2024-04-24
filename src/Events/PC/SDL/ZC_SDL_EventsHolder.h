#pragma once

#include <Events/ZC_EventsHolder.h>

struct ZC_SDL_EventsHolder : public ZC_EventsHolder
{
    bool PollEvents(float previousFrameTime) override;
};