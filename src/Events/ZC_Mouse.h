#pragma once

#include <ZC/Events/ZC_ESignal.h>

//  Mouse move/scroll events class (buttons in ZC_ButtonOperator).
class ZC_Mouse
{
public:
    void Init();
    ZC_EC ConnectMove(ZC_Function<void(float,float,float,float,float)>&& func);
    ZC_EC ConnectMoveOnceInFrame(ZC_Function<void(float,float,float,float,float)>&& func);
    ZC_EC ConnectScroll(ZC_Function<void(float,float,float)>&& func);
    ZC_EC ConnectScrollOnceInFrame(ZC_Function<void(float,float,float)>&& func);
    void GetCursorPosition(float& _x, float& _y) noexcept;
    //  axise Y data must be with Y zero point at buttom left corner of the window
    void MouseMove(float _cursorPosX, float _cursorPosY, float _cursorRelX, float _cursorRelY, float time);
    void MouseScroll(float horizontal, float vertical, float time);

private:
    ZC_ESignal<void(float,float,float,float,float)> sigMove;
    ZC_ESignal<void(float,float,float,float,float)> sigMoveOnceInFrame;
    ZC_ESignal<void(float,float,float)> sigScroll;
    ZC_ESignal<void(float,float,float)> sigScrollOnceInFrame;
    float cursorPosX = 0,
        cursorPosY = 0,
        cursorRelX = 0,
        cursorRelY = 0,
        scrolledHorizontal = 0,
        scrolledVertical = 0;

    void MoveOnceInFrame(float time);
    void ScrollOnceInFrame(float time);
};