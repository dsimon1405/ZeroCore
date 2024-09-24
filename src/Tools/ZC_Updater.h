#pragma once

#include <ZC/Events/ZC_ESignal.h>

#include <map>

struct ZC_Updater
{
    // struct Pair
    // {
    //     size_t level;
    //     ZC_ESignal<void(float)> eSig;

    //     bool operator == (size_t _lvl) const noexcept;
    // };

    std::map<size_t, ZC_ESignal<void(float)>> updates;

    ZC_EC Connect(ZC_Function<void(float)>&& func, size_t level);
    void Call(float time);
};



// bool ZC_Updater::Pair::operator == (size_t _lvl) const noexcept
// {
//     return level == _lvl;
// }