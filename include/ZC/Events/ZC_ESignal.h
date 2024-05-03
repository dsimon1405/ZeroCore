#pragma once

#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_ECTargetPointer.h"
#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <forward_list>
#include <list>

template<typename TFunction>
class ZC_ESignal;

//  Light signal for events. Thread unsafe.
template<typename TReturn, typename... TParams>
class ZC_ESignal<TReturn(TParams...)>
{
public:
    //  Creates a ZC_Signal with the function signature <TReturn(TParams...)>.
    ZC_ESignal() = default;

    /*
    Connects ZC_Function.

    Params:
    func - ZC_Function for connection.

    Return:
    ZC_SConnection pointer.
    */
    ZC_EC Connect(ZC_Function<TReturn(TParams...)>&& func);

    /*
    Calls all connected ZC_Function.

    Params:
    params - parameters of the functions.
    */
    void operator()(TParams... params);
    
    //  Return: functions are connected or not.
    bool IsEmpty();

    /*
    Calls last connected ZC_Function.

    Params:
    params - parameters of the functions.
    */
    void CallLastConnected(TParams... params);

    void Disconnect(const void* pFuncData);

private:
    std::list<ZC_Function<TReturn(TParams...)>> functions;
    std::list<ZC_Function<TReturn(TParams...)>> connectFuncs;
    std::list<const void*> disconnectPFuncData;

    void MakeReconnection();
};


template<typename TReturn, typename... TParams>
ZC_EC ZC_ESignal<TReturn(TParams...)>::Connect(ZC_Function<TReturn(TParams...)>&& func)
{
    return { new ZC_ECTargetPointer<ZC_ESignal<TReturn(TParams...)>*, const void*>{ this, connectFuncs.emplace_back(std::move(func)).GetPointerOnData() } };
}

template<typename TReturn, typename... TParams>
void ZC_ESignal<TReturn(TParams...)>::operator () (TParams... params)
{
    MakeReconnection();
    for (auto& function : functions) function(params...);
}

template<typename TReturn, typename... TParams>
bool ZC_ESignal<TReturn(TParams...)>::IsEmpty()
{
    return functions.empty();
}

template<typename TReturn, typename... TParams>
void ZC_ESignal<TReturn(TParams...)>::CallLastConnected(TParams... params)
{
    MakeReconnection();
    if (!functions.empty()) (*(--(functions.end())))(params...);
}

template<typename TReturn, typename... TParams>
void ZC_ESignal<TReturn(TParams...)>::Disconnect(const void* pFuncData)
{
    disconnectPFuncData.emplace_back(pFuncData);
}

template<typename TReturn, typename... TParams>
void ZC_ESignal<TReturn(TParams...)>::MakeReconnection()
{
    if (!connectFuncs.empty())
    {
        for (auto& connectFunc : connectFuncs) functions.emplace_back(std::move(connectFunc));
        connectFuncs.clear();
    }
    if (!disconnectPFuncData.empty())
    {
        for (auto pFuncData : disconnectPFuncData) std::erase(functions, pFuncData);
        disconnectPFuncData.clear();
    }

}