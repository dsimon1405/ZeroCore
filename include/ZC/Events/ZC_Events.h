#pragma once

#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_ButtonID.h"

struct ZC_Events
{
    ZC_Events() = delete;

    /*
    Binds the function to the button down event (will call while the button is pressed).

    Params:
    func - binding function (binding function parameters: previous frame time).

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectButtonDown(ZC_ButtonID buttonId, ZC_Function<void(float)>&& func);

    /*
    Binds the function to the button up event.

    Params:
    func - binding function (binding function parameters: previous frame time).

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectButtonUp(ZC_ButtonID buttonId, ZC_Function<void(float)>&& func);

    /*
    Binds the function to the move event.

    Params:
    func - binding function (binding function parameters:
                                                        first - X coordinates in the window,
                                                        second - Y coordinates in the window,
                                                        third - changes along the X axis,
                                                        fourth - changes along the Y axis.
                                                        fifth - previous frame time.)

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectMouseMove(ZC_Function<void(float,float,float,float,float)>&& func);

    /*
    Binds the function to the move event that calls one in frame.

    Params:
    func - binding function (binding function parameters:
                                                        first - last X coordinates in the current window frame,
                                                        second - last Y coordinates in the window,
                                                        third - sum of changes along the X axis in the current frame,
                                                        fourth - sum of changes along the Y axis in the current frame,
                                                        fifth - previous frame time.)

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectMouseMoveOnceInFrame(ZC_Function<void(float,float,float,float,float)>&& func);

    /*
    Binds the function to the scroll event.

    Params:
    func - function for binding (binding function params:
                                                        first - horizontal rotation (-1 or 1),
                                                        second - vertical rotation (-1 or 1),
                                                        third - previous frame time.)

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectMouseScroll(ZC_Function<void(float,float,float)>&& func);

    /*
    Binds the function to the scroll event.

    Params:
    func - function for binding (binding function params:
                                                        first - sum of horizontal rotations in the current frame,
                                                        second - sum of vertical rotations in the current frame,
                                                        third - previous frame time.)

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectMouseScrollOnceInFrame(ZC_Function<void(float,float,float)>&& func);

    /*
    Binds the function to the window resize.

    Params:
    func - binding function (binding function parameters:
                                                        first - new width
                                                        second - new height.)

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectWindowResize(ZC_Function<void(float,float)>&& func);

    /*
    Binds a function to the end of an event handle.

    Params:
    func - binding function (binding function parameters: previous frame time).

    Return:
    Connection to event.
    */
    static ZC_SConnection ConnectHandleEventsEnd(ZC_Function<void(float)>&& func);
};