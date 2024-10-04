#include <ZC/Tools/ZC_Random.h>

#include <stdlib.h>
#include <ctime>

int ZC_Random::GetRandomInt(int min, int max)
{
    return min + (rand() % (max - min));
}

bool ZC_Random::Init()
{
    srand(time(0));
    return true;
}