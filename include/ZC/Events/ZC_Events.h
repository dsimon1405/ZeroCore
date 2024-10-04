#pragma once

#include <ZC/Events/ZC_EC.h>
#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_ButtonID.h"
#include <ZC/Tools/Math/ZC_Vec3.h>

#include <vector>

struct ZC_Events
{
    ZC_Events() = delete;

    /*
    Binds a function to the click event of the window's close button. Only one function may be bind to event. To Disconnect call ConnectWindowCloseButton(nullptr).
    If function not seted window will be closed.

    Params:
    - function - function which will be called on click event of the windows close button.
    */
    static void ConnectWindowCloseButton(ZC_Function<void()>&& function);

    /*
    Binds the function to the button down event (will call function while the button is pressed). One down button can only connected once with this ConnectButtonPressDown(...),
    or once with ConnectButtonClick(...).

    Params:
    - buttonId - button id.
    - function - binding function (binding function parameters: previous frame time).

    Return:
    Connection to event.
    */
    static ZC_EC ConnectButtonPressDown(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& function);
    
    /*
    Binds a function to a button click event. This event calls funcDown when the button is pressed (only called once, even if the button is pressed continuously),
    and then calls funcUp when the button is released. One of connectiong fucntions can be missed, with {} constructor). One down button, can only connected once
    with this ConnectButtonPressDown(...), or once with ConnectButtonClick(...), or be the first button in a combination in the ConnectButtonCombination(...) function.

    Params:
    - buttonId - button id.
    - funcDown - function for calls on button down event (binding function parameters: previous frame time).
    - funcUp - function for calls on button up event (binding function parameters: previous frame time). That function could be missed with {} constructor.

    Return:
    Connection to event.
    */
    static ZC_EC ConnectButtonClick(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& funcDown, ZC_Function<void(ZC_ButtonID, float)>&& funcUp);

    /*
    Binds the function to the button up event.

    Params:
    - buttonId - button id.
    - func - binding function (binding function parameters: previous frame time).

    Return:
    Connection to event.
    */
    static ZC_EC ConnectButtonUp(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& func);
    
    /*
    Binds the function to the first button down event. Function will calls each time as some button will be click down.

    Params:
    - func - binding function (binding function parameters:
                                                            - button id,
                                                            - previous frame time).

    Return:
    Connection to event.
    */
    static ZC_EC ConnectFirstDownButton(ZC_Function<void(ZC_ButtonID, float)>&& funcDown);

    /*
    Binds the function to the move event.

    Params:
    - func - binding function (binding function parameters:
                                                        first - X coordinates in the window,
                                                        second - Y coordinates in the window,
                                                        third - changes along the X axis,
                                                        fourth - changes along the Y axis.
                                                        fifth - previous frame time.)

    Window coords shema:
    -   x = 0, y = 1 ----- x = 1, y = 1
    -      |              |
    -      |              |
    -      |              |
    -      |              |
    -   x = 0, y = 0 ----- x = 1, y = 0

    Return:
    Connection to event.
    */
    static ZC_EC ConnectMouseMove(ZC_Function<void(float,float,float,float,float)>&& func);

    /*
    Binds the function to the move event that calls one in frame.

    Params:
    - func - binding function (binding function parameters:
                                                        first - last X coordinates in the current window frame,
                                                        second - last Y coordinates in the window,
                                                        third - sum of changes along the X axis in the current frame,
                                                        fourth - sum of changes along the Y axis in the current frame,
                                                        fifth - previous frame time.)

    Window coords shema:
    -   x = 0, y = 1 ----- x = 1, y = 1
    -      |              |
    -      |              |
    -      |              |
    -      |              |
    -   x = 0, y = 0 ----- x = 1, y = 0

    Return:
    Connection to event.
    */
    static ZC_EC ConnectMouseMoveOnceInFrame(ZC_Function<void(float,float,float,float,float)>&& func);

    /*
    Binds the function to the scroll event.

    Params:
    - func - function for binding (binding function params:
                                                        first - horizontal rotation (-1 or 1),
                                                        second - vertical rotation (-1 or 1),
                                                        third - previous frame time.)

    Return:
    Connection to event.
    */
    static ZC_EC ConnectMouseScroll(ZC_Function<void(float,float,float)>&& func);

    /*
    Binds the function to the scroll event.

    Params:
    - func - function for binding (binding function params:
                                                        first - sum of horizontal rotations in the current frame,
                                                        second - sum of vertical rotations in the current frame,
                                                        third - previous frame time.)

    Return:
    Connection to event.
    */
    static ZC_EC ConnectMouseScrollOnceInFrame(ZC_Function<void(float,float,float)>&& func);

    /*
    Binds the function to the window resize.

    Params:
    - func - binding function (binding function parameters:
                                                        first - new width
                                                        second - new height.)

    Return:
    Connection to event.
    */
    static ZC_EC ConnectWindowResize(ZC_Function<void(float,float)>&& func);

    /*
    Binds a function to the end of an event handle.

    Params:
    - func - binding function (binding function parameters: previous frame time).

    Return:
    Connection to event.
    */
    static ZC_EC ConnectHandleEventsEnd(ZC_Function<void(float)>&& func);

    /*
    Binds the function to the event of changing the position of the currently active camera. If there is no active camera, ZC_SConnection will be returned disconnected.
    If another camera becomes active, the binding to events will not change and camera change events from a camera that is no longer active will be received.

    Params:
    - func - binding function (binding function parameters: new camera position).

    Return:
    Connection to event.
    */
    static ZC_EC ConnectActiveCameraChangePosition(ZC_Function<void(const ZC_Vec3<float>&)>&& func);

    /*
    Binds a function to the end of drawing.

    Params:
    - func - binding function (binding function parameters: previous frame time).

    Return:
    Connection to event.
    */
    static ZC_EC ConnectHandeEventsStart(ZC_Function<void(float)>&& func);

    //  Returns true if button pressed, otherwise false.
    static bool IsButtonPressed(ZC_ButtonID buttonId);
};