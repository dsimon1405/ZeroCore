#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/Tools/Signal/ZC_Signal.h>

#include <forward_list>
#include <list>

//  Buttons events class.
class ZC_Button
{
public:
    ZC_SConnection ConnectDown(ZC_ButtonID buttonId, ZC_Function<void(float)>&& function, bool callIfDown);
    ZC_SConnection ConnectUp(ZC_ButtonID buttonId, ZC_Function<void(float)>&& function);
    ZC_SConnection ConnectFirstDown(ZC_Function<void(ZC_ButtonID, float)>&& function);

    void AddActiveDownButton(ZC_ButtonID buttonId, float time);
    void CallUpButton(ZC_ButtonID buttonId, float prevFrameTime);
    //  calls active buttons wich have valid pointer on forward_list of down buttons
    void CallActiveButtons(float prevFrameTime);

private:
    struct ConnectedButton
    {
        ZC_ButtonID buttonId;
        ZC_Signal<void(float)> sigFunctions { false };

        bool operator == (ZC_ButtonID _buttonId);
    };

    struct ActiveDownButton
    {
        ZC_ButtonID buttonId;
        ZC_Signal<void(float)>* psigFunctions;

        bool operator == (ZC_ButtonID _buttonId);
    };

    std::list<ConnectedButton> downConnectedButtons;
    std::forward_list<ActiveDownButton> activeDownButtons;
    std::list<ConnectedButton> upConnectedButtons;
    ZC_Signal<void(ZC_ButtonID, float)> sigFirstDownButton { false };
};