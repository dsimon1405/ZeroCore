#pragma once

#include <ZC/Tools/ZC_sptr.h>

class ZC_SConnection
{
public:
    void Disconnect()
    {
        if (isConnected) *isConnected = false;
    }

    ZC_SConnection() = default;
    ZC_SConnection(ZC_sptr<bool>& _isConnected)
        : isConnected(_isConnected)
    {}

private:
    ZC_sptr<bool> isConnected {};
};