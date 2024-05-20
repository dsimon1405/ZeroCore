#pragma once

#include "ZC_SConnection.h"
#include <ZC/Tools/Function/ZC_Function.h>

#include <mutex>
#include <concepts>
#include <forward_list>
#include <list>
#include <vector>

template<typename T>
concept ZC_cNotRValueRef1 = !std::is_rvalue_reference<T>::value;

template<typename TFunction>
class ZC_Signal;

/*
Class for calling any number of ZC_Functions with the same signature. It is not possible to create a signal for a function/method that accepts rvalue
reference type parameters.
*/
template<typename TVal, ZC_cNotRValueRef1... TParams>
class ZC_Signal<TVal(TParams...)>
{
public:
    //  Creates a ZC_Signal with the function signature <TReturn(TParams...)>.
    ZC_Signal(bool useThreadSafety);
    // ZC_Signal1(const ZC_sptr<std::mutex>& spMutex) noexcept;

    ZC_Signal(ZC_Signal<TVal(TParams...)>&& sig);
    ZC_Signal<TVal(TParams...)>& operator = (ZC_Signal<TVal(TParams...)>&& sig);

    /*
    Connects ZC_Function.

    Params:
    func - ZC_Function for connection.

    Return:
    ZC_SConnection pointer.
    */
    ZC_SConnection Connect(ZC_Function<TVal(TParams...)>&& func);

    /*
    Calls all connected ZC_Function.

    Params:
    params - parameters of the functions.
    */
    void operator()(TParams... params);
    
    /*
    Calls all connected ZC_Function and store return values in vector.

    Params:
    container - a reference to a vector that will be filled with the function's return values.
    params - parameters of the functions.
    */
    void operator()(std::vector<TVal>& container, TParams... params);

    //  Return: functions are connected or not.
    bool IsEmpty();

    /*
    Calls last connected ZC_Function.

    Params:
    params - parameters of the functions.
    */
    void CallLastConnected(TParams... params);

    /*
    Calls last connected ZC_Function.

    Params:
    container - a reference to a unic pointer that will be filled with the function's return values.
    params - parameters of the functions.
    */
    void CallLastConnected(ZC_uptr<TVal>& container, TParams... params);

private:
    struct FuncAndState
    {
        ZC_Function<TVal(TParams...)> function;
        ZC_sptr<bool> spIsConnected = ZC_sptrMake<bool>(true);
    };

    std::list<FuncAndState> funcAndStates;
    std::list<FuncAndState> funcAndStatesToAdd;
    ZC_uptr<std::mutex> upMutexFuncAndStatesToAdd;
    //  from funcAndStatesToAdd to funcAndStates if still spIsConnected == true
    void AddFunctions();
};

template<typename TVal, ZC_cNotRValueRef1... TParams>
ZC_Signal<TVal(TParams...)>::ZC_Signal(bool useThreadSafety)
    : upMutexFuncAndStatesToAdd(useThreadSafety ? new std::mutex() : nullptr)
{}

template<typename TVal, ZC_cNotRValueRef1... TParams>
ZC_Signal<TVal(TParams...)>::ZC_Signal(ZC_Signal<TVal(TParams...)>&& sig)
    : funcAndStates(std::move(sig.funcAndStates)),
    funcAndStatesToAdd(std::move(sig.funcAndStatesToAdd)),
    upMutexFuncAndStatesToAdd(std::move(sig.upMutexFuncAndStatesToAdd))
{}

template<typename TVal, ZC_cNotRValueRef1... TParams>
ZC_Signal<TVal(TParams...)>& ZC_Signal<TVal(TParams...)>::operator = (ZC_Signal<TVal(TParams...)>&& sig)
{   
    funcAndStates = std::move(sig.funcAndStates);
    funcAndStatesToAdd = std::move(sig.funcAndStatesToAdd);
    upMutexFuncAndStatesToAdd = std::move(sig.upMutexFuncAndStatesToAdd);
    return *this;
}

template<typename TVal, ZC_cNotRValueRef1... TParams>
ZC_SConnection ZC_Signal<TVal(TParams...)>::Connect(ZC_Function<TVal(TParams...)>&& func)
{
    if (upMutexFuncAndStatesToAdd)
    {
        std::lock_guard<std::mutex> lock(*upMutexFuncAndStatesToAdd);
        return { funcAndStatesToAdd.emplace_back(FuncAndState{ std::move(func) }).spIsConnected };
    }
    else return { funcAndStatesToAdd.emplace_back(FuncAndState{ std::move(func) }).spIsConnected };
}

template<typename TVal, ZC_cNotRValueRef1... TParams>
void ZC_Signal<TVal(TParams...)>::operator () (TParams... params)
{
    AddFunctions();
    for (auto funcAndStatesIter = funcAndStates.begin(); funcAndStatesIter != funcAndStates.end(); )
    {
        if (*(funcAndStatesIter->spIsConnected))    //  if function still must be connected calls it, otherwise erases from funcAndStates
        {
            funcAndStatesIter->function(params...);
            funcAndStatesIter++;
        }
        else funcAndStatesIter = funcAndStates.erase(funcAndStatesIter);
    }
}

template<typename TVal, ZC_cNotRValueRef1... TParams>
void ZC_Signal<TVal(TParams...)>::operator () (std::vector<TVal>& container, TParams... params)
{
    AddFunctions();
    container.reserve(funcAndStates.size());
    for (auto funcAndStatesIter = funcAndStates.begin(); funcAndStatesIter != funcAndStates.end(); )
    {
        if (*(funcAndStatesIter->spIsConnected))    //  if function still must be connected calls it, otherwise erases from funcAndStates
        {
            container.emplace_back(funcAndStatesIter->function(params...));
            funcAndStatesIter++;
        }
        else funcAndStatesIter = funcAndStates.erase(funcAndStatesIter);
    }
}

template<typename TVal, ZC_cNotRValueRef1... TParams>
bool ZC_Signal<TVal(TParams...)>::IsEmpty()
{
    AddFunctions();
    for (auto& funcAndState : funcAndStates) if (*(funcAndState.spIsConnected)) return false;
    return true;
}

template<typename TVal, ZC_cNotRValueRef1... TParams>
void ZC_Signal<TVal(TParams...)>::CallLastConnected(TParams... params)
{
    AddFunctions();
    auto beforeBeginIter = --funcAndStates.begin();
    for (auto funcAndStatesIter = --funcAndStates.end(); funcAndStatesIter != beforeBeginIter; )
    {
        if (*(funcAndStatesIter->spIsConnected))
        {
            funcAndStatesIter->function(params...);
            return;
        }
        else
        {
            auto eraseIter = funcAndStatesIter;
            --funcAndStatesIter;
            funcAndStates.erase(eraseIter);
        }
    }
}

template<typename TVal, ZC_cNotRValueRef1... TParams>
void ZC_Signal<TVal(TParams...)>::CallLastConnected(ZC_uptr<TVal>& container, TParams... params)
{
    AddFunctions();
    auto beforeBeginIter = --funcAndStates.begin();
    for (auto funcAndStatesIter = --funcAndStates.end(); funcAndStatesIter != beforeBeginIter; )
    {
        if (*(funcAndStatesIter->spIsConnected))
        {
            container = ZC_uptrMake<TVal>(funcAndStatesIter->function(params...));
            return;
        }
        else
        {
            auto eraseIter = funcAndStatesIter;
            --funcAndStatesIter;
            funcAndStates.erase(eraseIter);
        }
    }
}

template<typename TVal, ZC_cNotRValueRef1... TParams>
void ZC_Signal<TVal(TParams...)>::AddFunctions()
{
    if (funcAndStatesToAdd.empty()) return;
    if (upMutexFuncAndStatesToAdd)
    {
        std::lock_guard<std::mutex> lock(*upMutexFuncAndStatesToAdd);
        for (auto& rFuncAndState : funcAndStatesToAdd)      // use emplace front for call in CallLastConnected()
            if (*(rFuncAndState.spIsConnected)) funcAndStates.emplace_back(std::move(rFuncAndState));  //  if still need to connect, connects
    }
    else for (auto& rFuncAndState : funcAndStatesToAdd)
            if (*(rFuncAndState.spIsConnected)) funcAndStates.emplace_back(std::move(rFuncAndState));  //  if still need to connect, connects

    funcAndStatesToAdd.clear();
}