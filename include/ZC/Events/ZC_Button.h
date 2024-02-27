#pragma once

#include "ZC_ButtonID.h"
#include <ZC/Tools/Signal/ZC_Signal.h>

#include <forward_list>
#include <list>


//  Buttons events class.
class ZC_Button
{
public:
    static ZC_SConnection ConnectDown(ZC_ButtonID buttonId, ZC_Function<void(float)>&& function);
    static ZC_SConnection ConnectUp(ZC_ButtonID buttonId, ZC_Function<void(float)>&& function);

protected:
    ZC_Button() = default;

    static void AddActiveDownButton(ZC_ButtonID buttonId);
    static void CallUpButton(ZC_ButtonID buttonId, float prevFrameTime);
    //  calls active buttons wich have valid pointer on forward_list of down buttons
    static void CallActiveButtons(float prevFrameTime);

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

    static inline std::list<ConnectedButton> downConnectedButtons;
    static inline std::forward_list<ActiveDownButton> activeDownButtons;
    static inline std::list<ConnectedButton> upConnectedButtons;
};