#pragma once

#include <ZC/Tools/ZC_sptr.h>

class ZC_SConnection
{
public:
    ZC_SConnection() = default;
    ZC_SConnection(ZC_sptr<bool>& _isConnected);
    
    void Disconnect();
    bool IsConnected() const noexcept;

private:
    ZC_sptr<bool> isConnected {};
};