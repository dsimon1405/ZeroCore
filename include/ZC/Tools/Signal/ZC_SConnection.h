#pragma once

#include "ZC_SCData.h"

template<typename TFunc>
class ZC_SConnection;
template<typename TReturn, ZC_cNotRValueRef... TParams>
//  Class for managing connection to the ZC_Signal.
class ZC_SConnection<TReturn(TParams...)>
{
public:
    ZC_SConnection() = default;
    ZC_SConnection(ZC_SCData<TReturn(TParams...)>* _pScd, typename std::list<ZC_Function<TReturn(TParams...)>>::iterator functionsIter) noexcept;

    //  Disconnects from the signal if connected to it.
    void Disconnect();

private:
    ZC_SCData<TReturn(TParams...)>* pScd;
    typename std::list<ZC_Function<TReturn(TParams...)>>::iterator functionsIter;
};

template<typename TReturn, ZC_cNotRValueRef... TParams>
ZC_SConnection<TReturn(TParams...)>::ZC_SConnection(ZC_SCData<TReturn(TParams...)>* _pScd, typename std::list<ZC_Function<TReturn(TParams...)>>::iterator _functionsIter) noexcept
    : pScd(_pScd),
    functionsIter(_functionsIter)
{}

template<typename TReturn, ZC_cNotRValueRef... TParams>
void ZC_SConnection<TReturn(TParams...)>::Disconnect()
{
    auto emptyIter = typename std::list<ZC_Function<TReturn(TParams...)>>::iterator();
    if (functionsIter != emptyIter)
    {
        pScd->Erase(functionsIter);
        functionsIter = emptyIter;
    }
}