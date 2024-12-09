#include <ZC/Tools/ZC_Random.h>

#include <stdlib.h>
#include <ctime>

i_zc ZC_Random::GetRandomInt(i_zc min, i_zc max)
{
    return min + (rand() % (max - min + 1));
}

f_zc ZC_Random::GetRandomFloat_x_100(f_zc min, f_zc max)
{
    static const f_zc f_100 = 100.f; 
    return f_zc(ZC_Random::GetRandomInt(min * f_100, max * f_100)) / f_100;
}

bool ZC_Random::Init()
{
    srand(time(0));
    return true;
}