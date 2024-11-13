#include "ZC_Updater.h"

ZC_EC ZC_Updater::Connect(ZC_Function<void(float)>&& func, size_t level)
{
    auto iter = updates.find(level);
    return iter != updates.end() ? iter->second.esig.Connect(std::move(func)) :
                                    updates.emplace(level, ESigActive{ .is_active = true, .esig = ZC_ESignal<void(float)>() }).first->second.esig.Connect(std::move(func));
}

// #define ZC_Updater_Call_timer
#ifdef ZC_Updater_Call_timer
#include <ZC/Tools/Time/ZC_Timer.h>
#include <iostream>
#endif
void ZC_Updater::Call(float time)
{
    if (!isActive) return;
#ifdef ZC_Updater_Call_timer
    static ZC_Timer timers[]
    {
        ZC_Timer(ZC_TR__seconds, 2., ZC_TRO__average, "0"),
        ZC_Timer(ZC_TR__seconds, 2., ZC_TRO__average, "1"),
        ZC_Timer(ZC_TR__seconds, 2., ZC_TRO__average, "2"),
        ZC_Timer(ZC_TR__seconds, 2., ZC_TRO__average, "3"),
    };
    for (auto& pair : updates)
    {
        timers[pair.first].StartPoint();
        if (pair.second.is_active) pair.second.esig(time);
        timers[pair.first].EndPoint();
    }
    if (timers[0].GetValuesCount() == 0ul) std::cout<<std::endl;
 #else
    for (auto& pair : updates)
        if (pair.second.is_active) pair.second.esig(time);
#endif
}

void ZC_Updater::ChangeState(bool needUpdate)
{
    isActive = needUpdate;
}

void ZC_Updater::ChangeLevelState(size_t lvl, bool is_acitve)
{
    auto iter = updates.find(lvl);
    if (iter == updates.end()) return;
    iter->second.is_active = is_acitve;
}