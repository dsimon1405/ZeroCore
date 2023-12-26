#pragma once

struct ZC_SDL_FPS
{
    // 1 sec = 1 000 000 000 nanosec.
    static const inline float nanosecond = 1000000000.f;
    unsigned long prevFrameTime = 0;
    unsigned long fpsTime = nanosecond / 61;

    void Actualize();
    void SetLimit(unsigned long fps);
    float SecondsTimestamp(unsigned long timestamp) const noexcept;
    float PreviousFrameSeconds() const noexcept;
};