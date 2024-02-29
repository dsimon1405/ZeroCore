#include "ZC_Mouse.h"

ZC_SConnection ZC_Mouse::ConnectMove(ZC_Function<void(float,float,float,float,float)>&& func)
{
    return sigMove.Connect(std::move(func));
}

ZC_SConnection ZC_Mouse::ConnectScroll(ZC_Function<void(float,float,float)>&& func)
{
    return sigScroll.Connect(std::move(func));
}

void ZC_Mouse::GetMousePosition(float& _x, float& _y) noexcept
{
    _x = x;
    _y = y;
}

void ZC_Mouse::MouseMove(float _x, float _y, float xRel, float yRel, float time)
{
    x = _x;
    y = _y;
    sigMove.CallLastConnected(x, y, xRel, yRel, time);
}

void ZC_Mouse::MouseScroll(float x, float y, float time)
{
    sigScroll.CallLastConnected(x, y, time);
}