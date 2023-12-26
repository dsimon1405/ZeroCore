#pragma once

#include <chrono>
#include <concepts>
#include <utility>

typedef typename std::chrono::nanoseconds ZC_Nanoseconds;
typedef typename std::chrono::microseconds ZC_Microseconds;
typedef typename std::chrono::milliseconds ZC_Milliseconds;
typedef typename std::chrono::seconds ZC_Seconds;
typedef typename std::chrono::minutes ZC_Minutes;
typedef typename std::chrono::hours ZC_Hours;
typedef typename std::chrono::days ZC_Days;
typedef typename std::chrono::weeks ZC_Weeks;
typedef typename std::chrono::years ZC_Years;
typedef typename std::chrono::months ZC_Months;

template<typename T>
concept ZC_cTimeMeasure = std::same_as<T, ZC_Nanoseconds>
                       || std::same_as<T, ZC_Microseconds>
                       || std::same_as<T, ZC_Milliseconds>
                       || std::same_as<T, ZC_Seconds>
                       || std::same_as<T, ZC_Minutes>
                       || std::same_as<T, ZC_Hours>
                       || std::same_as<T, ZC_Days>
                       || std::same_as<T, ZC_Weeks>
                       || std::same_as<T, ZC_Years>
                       || std::same_as<T, ZC_Months>;

//  Timing class.
class ZC_Clock
{
public:
    //  Create an object ZC_Clock and the timing begins.
    ZC_Clock() noexcept;

    ZC_Clock(const ZC_Clock&) = delete;
    ZC_Clock& operator = (const ZC_Clock&) = delete;

    ZC_Clock(ZC_Clock&& clock) noexcept;
    ZC_Clock& operator = (ZC_Clock&& clock) noexcept;

    ~ZC_Clock() = default;

    //  Starts/restarts counting down time.
    void Start() noexcept;

    //  Calculate the duration since the previous Start or Stop call. Starts counting down time.
    template<ZC_cTimeMeasure TTimeMeasue>
    long Restart();

    //  Calculate the duration since the previous Start or Stop call.
    template<ZC_cTimeMeasure TTimeMeasue>
    long Time();

private:
    typedef typename std::chrono::high_resolution_clock Clock;
    typedef typename std::chrono::time_point<Clock> TimePoint;

    TimePoint start;
};

template<ZC_cTimeMeasure TTimeMeasue>
long ZC_Clock::Restart()
{
    TimePoint now = Clock::now();
    long result = static_cast<long>(std::chrono::duration_cast<TTimeMeasue>(now - start).count());
    start = std::move(now);
    return result;
}

template<ZC_cTimeMeasure TTimeMeasue>
long ZC_Clock::Time()
{
    return static_cast<long>(std::chrono::duration_cast<TTimeMeasue>(Clock::now() - start).count());
}