#include <ZC/Events/ZC_EC.h>

ZC_EC::ZC_EC(ZC_uptr<ZC_EventConnection>&& _eventConnection)
    : eventConnection(std::move(_eventConnection))
{}

void ZC_EC::Disconnect()
{
    if (!eventConnection) return;
    eventConnection->Disconnect();
    eventConnection = nullptr;
}

bool ZC_EC::IsConnected() const noexcept
{
    return eventConnection ? eventConnection->IsConnected() : false;
}

void ZC_EC::NewConnection(ZC_EC&& ec)
{
    if (eventConnection) eventConnection->Disconnect();
    eventConnection = std::move(ec.eventConnection);
}