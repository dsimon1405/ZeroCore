#include "ZC_Updater.h"

ZC_EC ZC_Updater::Connect(ZC_Function<void(float)>&& func, size_t level)
{
    auto iter = updates.find(level);
    return iter != updates.end() ? iter->second.esig.Connect(std::move(func)) :
                                    updates.emplace(level, ESigActive{ .is_active = true, .esig = ZC_ESignal<void(float)>() }).first->second.esig.Connect(std::move(func));
}

void ZC_Updater::Call(float time)
{
    if (!isActive) return;
    for (auto& pair : updates)
        if (pair.second.is_active) pair.second.esig(time);
}

void ZC_Updater::ChangeUpdaterState(bool needUpdate)
{
    isActive = needUpdate;
}

void ZC_Updater::ChangeLevelState(size_t lvl, bool is_acitve)
{
    auto iter = updates.find(lvl);
    if (iter == updates.end()) return;
    iter->second.is_active = is_acitve;
}