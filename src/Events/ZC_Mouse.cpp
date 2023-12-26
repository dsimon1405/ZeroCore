#include <ZC/Events/ZC_Mouse.h>

ZC_SConnection<void(float,float,float,float,float)> ZC_Mouse::ConnectMove(ZC_Function<void(float,float,float,float,float)>&& func)
{
    return sMove.Connect(std::move(func));
}

ZC_SConnection<void(float,float,float)> ZC_Mouse::ConnectScroll(ZC_Function<void(float,float,float)>&& func)
{
    return sScroll.Connect(std::move(func));
}

void ZC_Mouse::GetPosition(float& _x, float& _y) noexcept
{
    _x = x;
    _y = y;
}

void ZC_Mouse::Move(float _x, float _y, float xRel, float yRel, float time)
{
    x = _x;
    y = _y;
    sMove(x, y, xRel, yRel, time);
}