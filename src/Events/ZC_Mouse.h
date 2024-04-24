#pragma once

#include <ZC/Tools/Signal/ZC_Signal.h>

//  Mouse move/scroll events class (buttons in ZC_ButtonOperator).
class ZC_Mouse
{
public:
    void Init();
    ZC_SConnection ConnectMove(ZC_Function<void(float,float,float,float,float)>&& func);
    ZC_SConnection ConnectMoveOnceInFrame(ZC_Function<void(float,float,float,float,float)>&& func);
    ZC_SConnection ConnectScroll(ZC_Function<void(float,float,float)>&& func);
    ZC_SConnection ConnectScrollOnceInFrame(ZC_Function<void(float,float,float)>&& func);
    void GetCursorPosition(float& _x, float& _y) noexcept;
    //  axise Y data must be with Y zero point at buttom left corner of the window
    void MouseMove(float _cursorPosX, float _cursorPosY, float _cursorRelX, float _cursorRelY, float time);
    void MouseScroll(float horizontal, float vertical, float time);

private:
    ZC_Signal<void(float,float,float,float,float)> sigMove { false };
    ZC_Signal<void(float,float,float,float,float)> sigMoveOnceInFrame { false };
    ZC_Signal<void(float,float,float)> sigScroll { false };
    ZC_Signal<void(float,float,float)> sigScrollOnceInFrame { false };
    float cursorPosX = 0,
        cursorPosY = 0,
        cursorRelX = 0,
        cursorRelY = 0,
        scrolledHorizontal = 0,
        scrolledVertical = 0;

    void MoveOnceInFrame(float time);
    void ScrollOnceInFrame(float time);
};