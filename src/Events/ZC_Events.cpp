#include <ZC/Events/ZC_Events.h>

#include "ZC_EventsHolder.h"
#include <ZC/Objects/Camera/ZC_Camera.h>

ZC_SConnection ZC_Events::ConnectButtonDown(ZC_ButtonID buttonId, ZC_Function<void(float)>&& func, bool callIfDown)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->button.ConnectDown(buttonId, std::move(func), callIfDown) : ZC_SConnection();
}

ZC_SConnection ZC_Events::ConnectButtonUp(ZC_ButtonID buttonId, ZC_Function<void(float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->button.ConnectUp(buttonId, std::move(func)) : ZC_SConnection();
}

ZC_SConnection ZC_Events::ConnectMouseMove(ZC_Function<void(float,float,float,float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->mouse.ConnectMove(std::move(func)) : ZC_SConnection();
}

ZC_SConnection ZC_Events::ConnectMouseMoveOnceInFrame(ZC_Function<void(float,float,float,float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->mouse.ConnectMoveOnceInFrame(std::move(func)) : ZC_SConnection();
}

ZC_SConnection ZC_Events::ConnectMouseScroll(ZC_Function<void(float,float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->mouse.ConnectScroll(std::move(func)) : ZC_SConnection();
}

ZC_SConnection ZC_Events::ConnectMouseScrollOnceInFrame(ZC_Function<void(float,float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->mouse.ConnectScrollOnceInFrame(std::move(func)) : ZC_SConnection();
}

ZC_SConnection ZC_Events::ConnectWindowResize(ZC_Function<void(float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->sigWindowResize.Connect(std::move(func)) : ZC_SConnection();
}

ZC_SConnection ZC_Events::ConnectHandleEventsEnd(ZC_Function<void(float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->sigHandleEventsEnd.Connect(std::move(func)) : ZC_SConnection();
}

ZC_SConnection ZC_Events::ConnectActiveCameraChangePosition(ZC_Function<void(const ZC_Vec3<float>&)>&& func)
{
    return ZC_Camera::GetActiveCamera() ? ZC_Camera::GetActiveCamera()->ConnectChangeCameraPosition(std::move(func)) : ZC_SConnection();
}

ZC_SConnection ZC_Events::ConnectFirstDownButton(ZC_Function<void(ZC_ButtonID, float)>&& func)
{
    return ZC_Camera::GetActiveCamera() ? ZC_EventsHolder::pEventsHolder->button.ConnectFirstDown(std::move(func)) : ZC_SConnection();
}