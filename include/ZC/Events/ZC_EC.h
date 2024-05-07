#pragma once

#include "ZC_EventConnection.h"

#include <ZC/Tools/ZC_uptr.h>

class ZC_EC
{
public:
    ZC_EC() = default;
    ZC_EC(ZC_uptr<ZC_EventConnection>&& _eventConnection);
    
    ZC_EC(ZC_EC&&) = delete;

    void Disconnect();
    bool IsConnected() const noexcept;
    //  Disconnects connected event, and store new event connection.
    void NewConnection(ZC_EC&& ec);

private:
    ZC_uptr<ZC_EventConnection> eventConnection;
};