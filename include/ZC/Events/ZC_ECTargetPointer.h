#pragma once

#include <ZC/Events/ZC_EC.h>

template<typename TpTarget>
struct ZC_ECTargetPointer : public ZC_EventConnection
{
    TpTarget pTarget;
    const void* pFuncData;

    ZC_ECTargetPointer(TpTarget _pSignal, const void* _pFuncData);
    
    void Disconnect() override;
    bool IsConnected() const noexcept override;
};


template<typename TpTarget>
ZC_ECTargetPointer<TpTarget>::ZC_ECTargetPointer(TpTarget _pSignal, const void* _pFuncData)
    : pTarget(_pSignal),
    pFuncData(_pFuncData)
{}

template<typename TpTarget>
void ZC_ECTargetPointer<TpTarget>::Disconnect()
{
    pTarget->Disconnect(pFuncData);
    pTarget = nullptr;
}

template<typename TpTarget>
bool ZC_ECTargetPointer<TpTarget>::IsConnected() const noexcept
{
    return pTarget != nullptr;
}