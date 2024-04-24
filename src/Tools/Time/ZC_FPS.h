#pragma once

#include <ZC/Tools/Time/ZC_Clock.h>
#include <ZC/Objects/Text/ZC_TextWindow.h>

struct ZC_FPS
{
    enum TimeMeasure
    {
        Nanoseconds,
        Microseconds,
        Milliseconds,
        Seconds,
    };

    //  timeMeasure - defined in which time measure going to be returned all time results.
    ZC_FPS(TimeMeasure timeMeasure);

    /*
    Start count time of new frame.

    Return:
    Time of previous frame in the selected measure.
    */
    float StartNewFrame();

    /*
    Set limit of frames in second.

    fps - limit. If 0 - unlimit. If < 0 than will set 0.
    */
    void SetLimit(long fps);

    /*
    Return:
    Previous framse time in selected measure.
    */
    float PreviousFrameTime() const noexcept;

    void NeedDraw(bool _needDraw);
    bool IsDrawing();

private:
    ZC_Clock clock;
    // 1 sec = 1 000 000 000 nanosec.
    const float nanosecond = 1000000000.f;
    long previousFrameNanoseconds = 0;
    long fpsTime = static_cast<long>(nanosecond / 61.f);
    float nanosecondsDivisor;

    bool needDraw = false;
    ZC_uptr<ZC_TextWindow> upTextFPS;

    ZC_uptr<ZC_TextWindow> CreateText();
    void UpdateText(long timeFromPreviousrestart);
};
