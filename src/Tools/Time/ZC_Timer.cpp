#include <ZC/Tools/Time/ZC_Timer.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <cassert>
#include <iostream>
#include <cmath>

ZC_Timer::ZC_Timer(ZC_TimerReport _report, double _counter, ZC_TROMask _output_mask, const std::string& _name)
    : report(_report),
    counter(_counter < 0. ? 0.
        : _report == ZC_TR__repeats ? std::floor(_counter)
        : _report == ZC_TR__seconds ? _counter * nanosecond
        : _counter),    //  for ZC_TR_call_Report counter no metter
    output_mask(_output_mask),
    name(_name)
{
    assert((_output_mask & (~(ZC_TRO__average | ZC_TRO__values | ZC_TRO__values_count | ZC_TRO__seconds_total))) == 0);
}

void ZC_Timer::StartPoint()
{
    if (!active) return;
    if (!(points.empty())) assert(points.back().end_nanosecs != -1ll);    //  point allready started
    points.emplace_back(Point{ .start_nanosecs = clock.Time<ZC_Nanoseconds>() });
}

void ZC_Timer::EndPoint()
{
    if (!active) return;
    assert(!(points.empty()) && points.back().end_nanosecs == -1ll);  //  no start point
    Point& last_point = points.back();
    points.back().end_nanosecs = clock.Time<ZC_Nanoseconds>();

    switch (report)
    {
    case ZC_TR__seconds: if (last_point.end_nanosecs - points.front().start_nanosecs >= counter) Report(); break;
    case ZC_TR__repeats: if (counter == 0. || points.size() == counter) Report(); break;
    default: break;
    }
}

void ZC_Timer::Report()
{
    std::cout<<name<<":";
    double total_time = 0.;
    double points_count = 0.;
    std::string caret = " ";   //  white space or new line
    if (output_mask & ZC_TRO__values)
    {
        unsigned long id = 1ul;
        for (Point& point : points)
        {
            if (point.end_nanosecs < point.start_nanosecs) continue;    //  end is default -1 or time long long is overflow
            double point_time = double(point.end_nanosecs - point.start_nanosecs) / nanosecond;
            std::cout<<"\n"<<id++<<". "<<point_time;
            total_time += point_time;
            ++points_count;
        }
        caret = "\n";
    }
    if (output_mask & ZC_TRO__average)
    {
        if (total_time == 0.)
        {
            for (Point& point : points)
            {
                if (point.end_nanosecs < point.start_nanosecs) continue;    //  end is default -1 or time long long is overflow
                total_time += double(point.end_nanosecs - point.start_nanosecs) / nanosecond;
                ++points_count;
            }
        }
        std::cout<<caret<<"avg: "<<(points_count == 0. ? 0. : (total_time / points_count));
        caret = "       ";
    }
    if (output_mask & ZC_TRO__values_count)
    {
        std::cout<<caret<<"vals: "<<points_count;
        caret = "       ";
    }
    if (output_mask & ZC_TRO__seconds_total) std::cout<<caret<<"total time: "<<(double(clock.Time<ZC_Nanoseconds>() - points.front().start_nanosecs) / nanosecond);
    std::cout<<std::endl;
    points.clear();
}

void ZC_Timer::ChangeActivity(bool _active)
{
    if (active == _active) return;

    active = _active;
    if (!active) points.clear();
}

bool ZC_Timer::GetActivity() const noexcept
{
    return active;
}

size_t ZC_Timer::GetValuesCount() const noexcept
{
    return points.size();
}