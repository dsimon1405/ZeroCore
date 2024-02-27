#include "ZC_FPS.h"

#include <thread>

ZC_FPS::ZC_FPS(TimeMeasure timeMeasure)
{
    switch (timeMeasure)
    {
    case TimeMeasure::Nanoseconds: nanosecondsDivisor = 1 ; break;
    case TimeMeasure::Microseconds: nanosecondsDivisor = 1000 ; break;
    case TimeMeasure::Milliseconds: nanosecondsDivisor = 1000000 ; break;
    case TimeMeasure::Seconds: nanosecondsDivisor = 1000000000 ; break;
    }
}

float ZC_FPS::StartNewFrame()
{
    long timeFromPreviousRestart = clock.Time<ZC_Nanoseconds>();
    if (timeFromPreviousRestart < fpsTime) std::this_thread::sleep_for(std::chrono::nanoseconds(fpsTime - timeFromPreviousRestart));
    previousFrameNanoseconds = clock.Restart<ZC_Nanoseconds>();
    return static_cast<float>(previousFrameNanoseconds) / nanosecondsDivisor;
}

void ZC_FPS::SetLimit(long fps)
{
    if (fps < 0) fps = 0;
    fpsTime = fps == 0 ? 0 : static_cast<long>(nanosecond) / (fps + 1);
}

float ZC_FPS::PreviousFrameTime() const noexcept
{
    return static_cast<float>(previousFrameNanoseconds) / nanosecondsDivisor;
}