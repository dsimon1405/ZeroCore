#include <ZC/Tools/Time/ZC_Clock.h>

ZC_Clock::ZC_Clock() noexcept
    : start(Clock::now())
{}

ZC_Clock::ZC_Clock(ZC_Clock&& clock) noexcept
    : start(std::move(clock.start))
{}

ZC_Clock& ZC_Clock::operator = (ZC_Clock&& clock) noexcept
{
    start = std::move(clock.start);
    return *this;
}

void ZC_Clock::Start() noexcept
{
    start = Clock::now();
}