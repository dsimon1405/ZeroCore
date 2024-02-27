#pragma once

#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Tools/Signal/ZC_Signal.h>

//  Mouse move/scroll events class (buttons in ZC_ButtonOperator).
class ZC_Mouse
{
public:
    /*
    Binds the function to the move event.

    Params:
    func - binding function (binding function parameters:
                                                        first - X coordinates in the window,
                                                        second - Y coordinates in the window,
                                                        third - changes along the X axis,
                                                        fourth - changes along the Y axis.
                                                        fifth - time stamp of the action in seconds (time from the previous frame until the event occurred).)

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectMove(ZC_Function<void(float,float,float,float,float)>&& func);

    /*
    Binds the function to the scroll event.

    Params:
    func - function for binding (binding function params:
                                                        first - scroll axis x.
                                                        second - scroll axis y.
                                                        third - time stamp of the action in seconds (time from the previous frame until the event occurred).)

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectScroll(ZC_Function<void(float,float,float)>&& func);
    static void GetPosition(float& _x, float& _y) noexcept;

protected:
    static inline float x = 0,
        y = 0;
    static inline ZC_Signal<void(float,float,float,float,float)> sMove { false };
    static inline ZC_Signal<void(float,float,float)> sScroll { false };

    ZC_Mouse() = default;

    void Move(float _x, float _y, float _xRel, float _yRel, float time);
};