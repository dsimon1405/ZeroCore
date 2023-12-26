#pragma once

#include <ZC/Tools/Time/ZC_Clock.h>

struct ZC_FPS
{
    static const inline long nanosecond = 1000000000;
    long fpsTime = nanosecond / 61;
    ZC_Clock clock {};
    double prevFrameSeconds = 0.f;

    void Actualize();
    bool SetLimit(long fps);
};