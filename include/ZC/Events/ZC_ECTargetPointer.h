#pragma once

#include <ZC/Events/ZC_EC.h>

template<typename TpTarget, typename TData>
struct ZC_ECTargetPointer : public ZC_EventConnection
{
    TpTarget pTarget;
    TData data;

    ZC_ECTargetPointer(TpTarget _pTarget, TData _data);
    
    void Disconnect() override;
    bool IsConnected() const noexcept override;
};


template<typename TpTarget, typename TData>
ZC_ECTargetPointer<TpTarget, TData>::ZC_ECTargetPointer(TpTarget _pTarget, TData _data)
    : pTarget(_pTarget),
    data(std::move(_data))
{}

template<typename TpTarget, typename TData>
void ZC_ECTargetPointer<TpTarget, TData>::Disconnect()
{
    pTarget->Disconnect(std::move(data));
    pTarget = nullptr;
}

template<typename TpTarget, typename TData>
bool ZC_ECTargetPointer<TpTarget, TData>::IsConnected() const noexcept
{
    return pTarget != nullptr;
}