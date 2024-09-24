#include "ZC_Updater.h"

ZC_EC ZC_Updater::Connect(ZC_Function<void(float)>&& func, size_t level)
{
    auto iter = updates.find(level);
    return iter != updates.end() ? iter->second.Connect(std::move(func)) :
                                    updates.emplace(level, ZC_ESignal<void(float)>()).first->second.Connect(std::move(func));
}

void ZC_Updater::Call(float time)
{
    for (auto& pair : updates) pair.second(time);
}