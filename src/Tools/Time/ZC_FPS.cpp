#include "ZC_FPS.h"

#include <thread>

void ZC_FPS::Actualize()
{
    const long curFrameTime = clock.Time<ZC_Nanoseconds>();
    if (curFrameTime < fpsTime) std::this_thread::sleep_for(ZC_Nanoseconds(fpsTime - curFrameTime));
    prevFrameSeconds = static_cast<double>(clock.Restart<ZC_Nanoseconds>()) / static_cast<double>(nanosecond);
}

bool ZC_FPS::SetLimit(long fps)
{
    if (fps < 0) return false;
    // 1 sec = 1 000 000 000 nanosec
    fpsTime = fps == 0 ? 0 : nanosecond / (fps + 1);
    return true;
}