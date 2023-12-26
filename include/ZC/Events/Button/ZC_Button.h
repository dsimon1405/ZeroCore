#pragma once

#include <ZC/Tools/Signal/ZC_Signals.h>
#include "ZC_ButtonID.h"

//  Button struct for actions.
struct ZC_Button
{
    enum State
    {
        Down,
        Up
    };

    ZC_ButtonID id;
    State state;

    /*
    Create button.

    Params:
    _id - button identifier.
    _state - button state.
    */
    ZC_Button(ZC_ButtonID _id, State _state) noexcept;

    bool operator < (const ZC_Button& btn) const noexcept;
};