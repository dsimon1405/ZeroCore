#pragma once

#include <ZC/Events/ZC_Event.h>
#include "ZC_Button.h"

//  Buttons events class.
class ZC_ButtonOperator : public virtual ZC_Event
{
public:
    ~ZC_ButtonOperator() override = default;

    /*
    Binds the function to the button event.

    Params:
    but - button for binding.
    func - function for binding (binding function parameters: firts - time stamp of the action in seconds (time from the previous frame until the event occurred).).

    Return:
    Connection to event.
    */
    static ZC_SConnection<void(float)> Connect(ZC_Button but, ZC_Function<void(float)> func);

protected:
    static inline ZC_Signals<ZC_Button, void(float)> ssButton {};

    ZC_ButtonOperator() = default;
};