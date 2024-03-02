#include <ZC/Tools/Signal/ZC_SConnection.h>

void ZC_SConnection::Disconnect()
{
    if (isConnected)
    {
        *isConnected = false;
        isConnected = nullptr;
    }
}
bool ZC_SConnection::IsConnected()
{
    return isConnected;
}

ZC_SConnection::ZC_SConnection(ZC_sptr<bool>& _isConnected)
    : isConnected(_isConnected)
{}