#pragma once

#include <ZC/Tools/Function/ZC_Function.h>
#include "ZC_ECTargetPointer.h"
#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <forward_list>
#include <list>

template<typename TFunction>
class ZC_ESignal;

//  Light signal for events. Thread unsafe.
template<typename TVal, typename... TParams>
class ZC_ESignal<TVal(TParams...)>
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
    ZC_EC Connect(ZC_Function<TVal(TParams...)>&& func);

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
    std::list<ZC_Function<TVal(TParams...)>> functions;
    std::list<ZC_Function<TVal(TParams...)>> connectFuncs;
    std::list<const void*> disconnectPFuncData;

    void MakeReconnection();
};


template<typename TVal, typename... TParams>
ZC_EC ZC_ESignal<TVal(TParams...)>::Connect(ZC_Function<TVal(TParams...)>&& func)
{
    return { new ZC_ECTargetPointer<ZC_ESignal<TVal(TParams...)>*>{ this, connectFuncs.emplace_back(std::move(func)).GetPointerOnData() } };
}

template<typename TVal, typename... TParams>
void ZC_ESignal<TVal(TParams...)>::operator () (TParams... params)
{
    MakeReconnection();
    for (auto& function : functions) function(params...);
}

template<typename TVal, typename... TParams>
bool ZC_ESignal<TVal(TParams...)>::IsEmpty()
{
    return functions.empty();
}

template<typename TVal, typename... TParams>
void ZC_ESignal<TVal(TParams...)>::CallLastConnected(TParams... params)
{
    MakeReconnection();
    if (!functions.empty()) (*(--(functions.end())))(params...);
}

template<typename TVal, typename... TParams>
void ZC_ESignal<TVal(TParams...)>::Disconnect(const void* pFuncData)
{
    disconnectPFuncData.emplace_back(pFuncData);
}

template<typename TVal, typename... TParams>
void ZC_ESignal<TVal(TParams...)>::MakeReconnection()
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