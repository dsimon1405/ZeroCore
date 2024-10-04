#pragma once


struct ZC_Random
{
    static int GetRandomInt(int min, int max);

private:
    static bool Init();

    static inline bool start = Init();
};