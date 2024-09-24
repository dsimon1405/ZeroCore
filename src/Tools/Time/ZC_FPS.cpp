#include "ZC_FPS.h"

#include <ZC/Objects/Text/ZC_Fonts.h>

#include <thread>
#include <format>

ZC_FPS::ZC_FPS(ZC_FPS_TimeMeasure timeMeasure)
{
    ChangeTimeMeasure(timeMeasure);
}

float ZC_FPS::StartNewFrame()
{
    long timeFromPreviousRestart = 0;
    if (fpsTime != 0)
    {
        timeFromPreviousRestart = clock.Time<ZC_Nanoseconds>();
        if (timeFromPreviousRestart < fpsTime) std::this_thread::sleep_for(std::chrono::nanoseconds(fpsTime - timeFromPreviousRestart));
    }
    previousFrameNanoseconds = clock.Restart<ZC_Nanoseconds>();

    if (upTextFPS && upTextFPS->IsDrawing()) UpdateText(timeFromPreviousRestart);

    ++frame_counter;

    return static_cast<float>(previousFrameNanoseconds) / nanosecondsDivisor;
}

void ZC_FPS::SetLimit(long fps)
{
    if (fps < 0) fps = 0;
    fpsTime = fps == 0 ? 0 : static_cast<long>(nanosecond) / (fps + 1);
}

void ZC_FPS::ChangeTimeMeasure(ZC_FPS_TimeMeasure timeMeasure)
{
    switch (timeMeasure)
    {
    case ZC_FPS_TimeMeasure::ZC_FPS_TM__Nanoseconds: nanosecondsDivisor = 1.f; break;
    case ZC_FPS_TimeMeasure::ZC_FPS_TM__Microseconds: nanosecondsDivisor = 1000.f; break;
    case ZC_FPS_TimeMeasure::ZC_FPS_TM__Milliseconds: nanosecondsDivisor = 1000000.f; break;
    case ZC_FPS_TimeMeasure::ZC_FPS_TM__Seconds: nanosecondsDivisor = 1000000000.f; break;
    }
}

float ZC_FPS::PreviousFrameTime() const noexcept
{
    return static_cast<float>(previousFrameNanoseconds) / nanosecondsDivisor;
}

void ZC_FPS::NeedDraw(bool needDraw)
{
    if (needDraw && !upTextFPS) upTextFPS = CreateText();
    if (upTextFPS) upTextFPS->NeedDraw(needDraw);
}

bool ZC_FPS::IsDrawing()
{
    return upTextFPS && upTextFPS->IsDrawing();
}

unsigned long long ZC_FPS::GetCurrentFrameNumber() const
{
    return frame_counter;
}

ZC_uptr<ZC_TextWindow> ZC_FPS::CreateText()
{
    const ulong textHeight = 30;
    ZC_FontData fontData{ ZC_F_Arial, textHeight };
    ZC_Fonts::Load(&fontData, 1);

    return { new ZC_TextWindow({ ZC_F_Arial, textHeight }, "FPS: 0 ()", ZC_TA_Left, 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel, true) };
}

void ZC_FPS::UpdateText(long timeFromPreviousRestart)
{
    static ZC_Clock clock;
    static const long limit = nanosecond / 2;   //  update upTextDPF every 1/2 of a second
    
    if (clock.Time<ZC_Nanoseconds>() < limit) return;
    clock.Start();

    float limitedFPS = static_cast<float>(nanosecond) / static_cast<float>(previousFrameNanoseconds);   //  or factical fps
    fpsTime == 0 ? upTextFPS->SetText(std::format("fps: {:.1f}", limitedFPS))
        : upTextFPS->SetText(std::format("fps: {:.1f}({:.1f})", limitedFPS, static_cast<float>(nanosecond) / static_cast<float>(timeFromPreviousRestart)));

    limitedFPS < 45.f ? upTextFPS->SetColorUChar(192, 0, 0)
        : limitedFPS < 55.f ? upTextFPS->SetColorUChar(192, 128, 0)
        : upTextFPS->SetColorUChar(0, 192, 0);
}