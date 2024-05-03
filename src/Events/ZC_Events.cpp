#include <ZC/Events/ZC_Events.h>

#include "ZC_EventsHolder.h"
#include <ZC/Objects/Camera/ZC_Camera.h>

ZC_EC ZC_Events::ConnectButtonPressDown(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& function)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->buttonHolder.buttonPressedDown.Connect(buttonId, std::move(function)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectButtonClick(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& funcDown, ZC_Function<void(ZC_ButtonID, float)>&& funcUp)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->buttonHolder.buttonClick.Connect(buttonId, std::move(funcDown), std::move(funcUp)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectButtonUp(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& funcDown)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->buttonHolder.buttonUp.Connect(buttonId, std::move(funcDown)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectFirstDownButton(ZC_Function<void(ZC_ButtonID, float)>&& funcDown)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->buttonHolder.esigFirstDownButton.Connect(std::move(funcDown)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectMouseMove(ZC_Function<void(float,float,float,float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->mouse.ConnectMove(std::move(func)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectMouseMoveOnceInFrame(ZC_Function<void(float,float,float,float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->mouse.ConnectMoveOnceInFrame(std::move(func)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectMouseScroll(ZC_Function<void(float,float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->mouse.ConnectScroll(std::move(func)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectMouseScrollOnceInFrame(ZC_Function<void(float,float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->mouse.ConnectScrollOnceInFrame(std::move(func)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectWindowResize(ZC_Function<void(float,float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->sigWindowResize.Connect(std::move(func)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectHandleEventsEnd(ZC_Function<void(float)>&& func)
{
    return ZC_EventsHolder::pEventsHolder ? ZC_EventsHolder::pEventsHolder->sigHandleEventsEnd.Connect(std::move(func)) : ZC_EC();
}

ZC_EC ZC_Events::ConnectActiveCameraChangePosition(ZC_Function<void(const ZC_Vec3<float>&)>&& func)
{
    return ZC_Camera::GetActiveCamera() ? ZC_Camera::GetActiveCamera()->ConnectChangeCameraPosition(std::move(func)) : ZC_EC();
}