#pragma once

struct ZC_EventConnection
{
    virtual ~ZC_EventConnection() = default;

    virtual void Disconnect() = 0;
    virtual bool IsConnected() const noexcept = 0;
};