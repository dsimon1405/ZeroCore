#pragma once

#include <ZC/Tools/Time/ZC_Clock.h>
#include <ZC/Objects/Text/ZC_TextWindow.h>
#include <ZC/Tools/Time/ZC_FPS_TimeMeasure.h>

struct ZC_FPS
{
    //  _time_measure - defined in which time measure going to be returned all time results.
    ZC_FPS(ZC_FPS_TimeMeasure _time_measure);

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

    //  Change time measure.
    void ChangeTimeMeasure(ZC_FPS_TimeMeasure _time_measure);

    // Return previous frame time in selected measure.
    float GetPreviousFrameTime(ZC_FPS_TimeMeasure _time_measure) const noexcept;

    //  Return current time measure.
    ZC_FPS_TimeMeasure GetTimeMeasure() const noexcept;

    void NeedDraw(bool _needDraw);
    bool IsDrawing();
    //  retrun current frame number
    unsigned long long GetCurrentFrameNumber() const;

private:
    ZC_Clock clock;
    // 1 sec = 1 000 000 000 nanosec.
    const float nanosecond = 1000000000.f;
    long previousFrameNanoseconds = 0;
    long fpsTime = static_cast<long>(nanosecond / 61.f);
    float nanosecondsDivisor;

    unsigned long long frame_counter = 0;
    ZC_FPS_TimeMeasure time_measure = ZC_FPS_TM__Nanoseconds;

    bool needDraw = false;
    ZC_uptr<ZC_TextWindow> upTextFPS;

    ZC_uptr<ZC_TextWindow> CreateText();
    void UpdateText(long timeFromPreviousrestart);
};
