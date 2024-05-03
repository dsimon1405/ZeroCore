#pragma once

#include <ZC/Tools/ZC_uptr.h>

struct ZC_EventConnection
{
    virtual ~ZC_EventConnection() = default;

    virtual void Disconnect() = 0;
    virtual bool IsConnected() const noexcept = 0;
};

// using ZC_EC = ZC_uptr<ZC_EventConnection>;

struct ZC_EC
{
    ZC_EC() = default;
    ZC_EC(ZC_uptr<ZC_EventConnection>&& _eventConnection)
        : eventConnection(std::move(_eventConnection))
    {}
    
    ZC_EC(ZC_EC&&) = delete;

    void Disconnect()
    {
        if (!eventConnection) return;
        eventConnection->Disconnect();
        eventConnection = nullptr;
    }

    bool IsConnected() const noexcept
    {
        return eventConnection ? eventConnection->IsConnected() : false;
    }

    //  Disconnects connected event, and store new event connection.
    void NewConnection(ZC_EC&& ec)
    {
        if (eventConnection) eventConnection->Disconnect();
        eventConnection = std::move(ec.eventConnection);
    }

private:
    ZC_uptr<ZC_EventConnection> eventConnection;
};