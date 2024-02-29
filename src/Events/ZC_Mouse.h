#pragma once

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
                                                        fifth - previous frame time.

    Return:
    Connection to event.
    */
    ZC_SConnection ConnectMove(ZC_Function<void(float,float,float,float,float)>&& func);

    /*
    Binds the function to the scroll event.

    Params:
    func - function for binding (binding function params:
                                                        first - scroll axis x.
                                                        second - scroll axis y.
                                                        third - previous frame time.

    Return:
    Connection to event.
    */
    ZC_SConnection ConnectScroll(ZC_Function<void(float,float,float)>&& func);
    void GetMousePosition(float& _x, float& _y) noexcept;

    void MouseMove(float _x, float _y, float _xRel, float _yRel, float time);
    void MouseScroll(float,float,float);

private:
    ZC_Signal<void(float,float,float,float,float)> sigMove { false };
    ZC_Signal<void(float,float,float)> sigScroll { false };
    float x = 0,
        y = 0;
};