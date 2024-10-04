#pragma once

#include <ZC/Events/ZC_ESignal.h>

#include <map>

class ZC_Updater
{
public:
    ZC_EC Connect(ZC_Function<void(float)>&& func, size_t level);
    void Call(float time);
    void ChangeUpdaterState(bool needUpdate);
    void ChangeLevelState(size_t lvl, bool is_acitve);

private:
    struct ESigActive
    {
        bool is_active;
        ZC_ESignal<void(float)> esig;
    };

    bool isActive = true;
    std::map<size_t, ESigActive> updates;
};