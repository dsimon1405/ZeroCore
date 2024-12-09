#pragma once

#include <ZC/ZC_Types.h>

struct ZC_Random
{
        //  return random int
    static i_zc GetRandomInt(i_zc min, i_zc max);
        //  multiplies min and max by 100.f, calculates a random value, divides the result by 100.f and returns.
    static f_zc GetRandomFloat_x_100(f_zc min, f_zc max);

private:
    static bool Init();

    static inline bool start = Init();
};