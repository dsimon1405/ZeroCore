#include "ZC_Mouse.h"

#include <ZC/Events/ZC_Events.h>
#include <ZC/GUI/ZC_GUI.h>

void ZC_Mouse::Init()
{
    ZC_Events::ConnectHandleEventsEnd({ &ZC_Mouse::MoveOnceInFrame, this });
    ZC_Events::ConnectHandleEventsEnd({ &ZC_Mouse::ScrollOnceInFrame, this });
}

ZC_EC ZC_Mouse::ConnectMove(ZC_Function<void(float,float,float,float,float)>&& func)
{
    return sigMove.Connect(std::move(func));
}

ZC_EC ZC_Mouse::ConnectMoveOnceInFrame(ZC_Function<void(float,float,float,float,float)>&& func)
{
    return sigMoveOnceInFrame.Connect(std::move(func));
}

ZC_EC ZC_Mouse::ConnectScroll(ZC_Function<void(float,float,float)>&& func)
{
    return sigScroll.Connect(std::move(func));
}

ZC_EC ZC_Mouse::ConnectScrollOnceInFrame(ZC_Function<void(float,float,float)>&& func)
{
    return sigScrollOnceInFrame.Connect(std::move(func));
}

void ZC_Mouse::GetCursorPosition(float& posX, float& posY) noexcept
{
    posX = cursorPosX;
    posY = cursorPosY;
}

void ZC_Mouse::MouseMove(float _cursorPosX, float _cursorPosY, float _cursorRelX, float _cursorRelY, float time)
{
    cursorPosX = _cursorPosX;
    cursorPosY = _cursorPosY;
    cursorRelX += _cursorRelX;
    cursorRelY += _cursorRelY;
    if (ZC_GUI::pGUI)
    {
        mayCallSigMove = ZC_GUI::pGUI->eventManager.CursoreMove(cursorPosX, cursorPosY, cursorRelX, cursorRelY, time);
        if (mayCallSigMove) sigMove(cursorPosX, cursorPosY, _cursorRelX, _cursorRelY, time);
    }
    else sigMove(cursorPosX, cursorPosY, _cursorRelX, _cursorRelY, time);
}

void ZC_Mouse::MouseScroll(float horizontal, float vertical, float time)
{
    scrolledHorizontal += horizontal;
    scrolledVertical += vertical;
    sigScroll.CallLastConnected(horizontal, vertical, time);
}

void ZC_Mouse::MoveOnceInFrame(float time)
{
    if (cursorRelX != 0.f || cursorRelY != 0.f)
    {
        // sigMoveOnceInFrame.CallLastConnected(cursorPosX, cursorPosY, cursorRelX, cursorRelY, time);
        if (mayCallSigMove) sigMoveOnceInFrame(cursorPosX, cursorPosY, cursorRelX, cursorRelY, time);
        cursorRelX = 0.f;
        cursorRelY = 0.f;
    }
}

void ZC_Mouse::ScrollOnceInFrame(float time)
{
    if (scrolledHorizontal != 0.f || scrolledVertical != 0.f)
    {
        sigScrollOnceInFrame.CallLastConnected(scrolledHorizontal, scrolledVertical, time);
        scrolledHorizontal = 0.f;
        scrolledVertical = 0.f;
    }
}
