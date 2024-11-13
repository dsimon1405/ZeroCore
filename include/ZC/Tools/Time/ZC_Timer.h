#pragma once

#include "ZC_Clock.h"

#include <list>

enum ZC_TimerReport
{
    ZC_TR__call_Report,
    ZC_TR__seconds,
    ZC_TR__repeats,
};

enum ZC_TimerReportOutput
{
    ZC_TRO__values          = 1 << 1,   //  all time measures in seonds captured with StartPoint() EndPoint()
    ZC_TRO__average         = 1 << 2,   //  average of values
    ZC_TRO__values_count    = 1 << 3,   //  amount of unreported calls EndPoint()
    ZC_TRO__seconds_total   = 1 << 4,   //  time from laters unreported call StartPoint()
};
typedef int ZC_TROMask;

/*
Class to measure a time of the operation. How to use: create ZC_Timer (may be static variable in cycle of the method). Make call StartPoint() - start tracking time,
then EndPoint() - to finish tracking and check console out condition. If condition reached will call Report(), wich make out to console and erase that data.
*/
class ZC_Timer
{
public:
    /*
    - _report - when make console report.
    - _counter - for ZC_TR__call_Report is no metter, for ZC_TR__seconds is seconds, for ZC_TR__repeats count of call EndPoint().
    - _output_mask - what to output in console.
    - _name - name will be at the beginning of the console output.
    */
    ZC_Timer(ZC_TimerReport _report, double _counter, ZC_TROMask _output_mask, const std::string& _name);

        //  start tracking time.
    void StartPoint();
        //  end tracking time and call Report if report condition reached.
    void EndPoint();
        //  make output to console of the data, and erase it.
    void Report();
        //  if true - use StartPoint() EndPoint(), otherwise not. If not clear saved data.
    void ChangeActivity(bool _active);
    bool GetActivity() const noexcept;
        //  count of values in the timer now.
    size_t GetValuesCount() const noexcept;

private:
    static inline ZC_Clock clock;
    static inline const double nanosecond = 1000000000.f;   // 1 sec = 1 000 000 000 nanosec.

    struct Point
    {
        long long start_nanosecs = -1ll;
        long long end_nanosecs = -1ll;
    };

    bool active = true;
    std::list<Point> points;
    ZC_TimerReport report;
    double counter;  //  for ZC_TR__seconds seconds, for ZC_TR__repeats repeats count
    ZC_TROMask output_mask;
    std::string name;
};