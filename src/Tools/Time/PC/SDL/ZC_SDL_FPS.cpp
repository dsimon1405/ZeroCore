#include "ZC_SDL_FPS.h"

#include <thread>
#include <SDL3/SDL_timer.h>

void ZC_SDL_FPS::Actualize()
{
    const unsigned long curFrameTime = SDL_GetTicksNS() - prevFrameTime;
    if (curFrameTime < fpsTime) std::this_thread::sleep_for(std::chrono::nanoseconds(fpsTime - curFrameTime));
    prevFrameTime = SDL_GetTicksNS();
}

void ZC_SDL_FPS::SetLimit(unsigned long fps)
{
    fpsTime = fps == 0 ? 0 : static_cast<unsigned long>(nanosecond) / (fps + 1);
}

float ZC_SDL_FPS::SecondsTimestamp(unsigned long timestamp) const noexcept
{
    return static_cast<float>(timestamp - prevFrameTime) / nanosecond;
}

float ZC_SDL_FPS::PreviousFrameSeconds() const noexcept
{
    return static_cast<float>(prevFrameTime) / nanosecond;
}