#pragma once

#include "ZC_FunctionHolder.h"
#include <ZC/Tools/Exception/ZC_Exception.h>
#include <ZC/Tools/ZC_uptr.h>

/*
Function/method pointer wrapper.
*/
template<typename TFunc>
class ZC_Function;
template<typename TReturn, typename... TParams>
class ZC_Function<TReturn(TParams...)>
{
public:
    ZC_Function() = default;
    
    /*
    Create function wrapper.

    Params:
    pFunc - function pointer.
    */
    ZC_Function(TReturn(* pFunc)(TParams...)) noexcept;
    
    /*
    Create method wrapper.

    Params:
    pFunc - function pointer.
    pClass - class pointer.
    */
    template<typename TClass>
    ZC_Function(TReturn(TClass::*pFunc)(TParams...), TClass* pClass) noexcept;
    
    /*
    Create method wrapper.

    Params:
    pFunc - function pointer.
    pClass - class pointer.
    */
    template<typename TClass>
    ZC_Function(TReturn(TClass::*pFunc)(TParams...) const, TClass* pClass) noexcept;
    
    /*
    Create method wrapper.

    Params:
    pFunc - function pointer.
    pClass - class pointer.
    */
    template<typename TClass>
    ZC_Function(TReturn(TClass::*pFunc)(TParams...) const, const TClass* pClass) noexcept;

    ZC_Function(ZC_Function<TReturn(TParams...)>&& pFunc) noexcept;
    ZC_Function<TReturn(TParams...)>& operator = (ZC_Function<TReturn(TParams...)>&& func) noexcept;

    /*
    Call function(method). If function is nullptr throw exception.

    Params:
    params... - parameters of the function(method).

    Return:
    TReturn type object.
    */
    TReturn operator () (TParams... params) const;

    operator bool () noexcept;

private:
    ZC_uptr<ZC_IFunctionHolder<TReturn(TParams...)>> pFuncHolder;
};

template<typename TReturn, typename... TParams>
ZC_Function<TReturn(TParams...)>::ZC_Function(TReturn(* pFunc)(TParams...)) noexcept
    : pFuncHolder(pFunc ? ZC_uptrMakeFromChild<ZC_IFunctionHolder<TReturn(TParams...)>,
        ZC_FunctionHolder<TReturn(TParams...)>>(pFunc) : nullptr)
{}

template<typename TReturn, typename... TParams>
template<typename TClass>
ZC_Function<TReturn(TParams...)>::ZC_Function(TReturn(TClass::*pFunc)(TParams...), TClass* pClass) noexcept
    : pFuncHolder(pFunc && pClass ? ZC_uptrMakeFromChild<ZC_IFunctionHolder<TReturn(TParams...)>,
        ZC_FunctionHolder<TReturn(TParams...), TClass>>(pFunc, pClass) : nullptr)
{}

template<typename TReturn, typename... TParams>
template<typename TClass>
ZC_Function<TReturn(TParams...)>::ZC_Function(TReturn(TClass::*pFunc)(TParams...) const, TClass* pClass) noexcept
    : pFuncHolder(pFunc && pClass ? ZC_uptrMakeFromChild<ZC_IFunctionHolder<TReturn(TParams...)>,
        ZC_FunctionHolder<TReturn(TParams...), TClass>>(pFunc, pClass) : nullptr)
{}

template<typename TReturn, typename... TParams>
template<typename TClass>
ZC_Function<TReturn(TParams...)>::ZC_Function(TReturn(TClass::*pFunc)(TParams...) const, const TClass* pClass) noexcept
    : pFuncHolder(pFunc && pClass ? ZC_uptrMakeFromChild<ZC_IFunctionHolder<TReturn(TParams...)>,
        ZC_FunctionHolder<TReturn(TParams...), TClass>>(pFunc, pClass) : nullptr)
{}

template<typename TReturn, typename... TParams>
ZC_Function<TReturn(TParams...)>::ZC_Function(ZC_Function<TReturn(TParams...)>&& pFunc) noexcept
    : pFuncHolder(std::move(pFunc.pFuncHolder))
{}

template<typename TReturn, typename... TParams>
ZC_Function<TReturn(TParams...)>& ZC_Function<TReturn(TParams...)>::operator = (ZC_Function<TReturn(TParams...)>&& func) noexcept
{
    pFuncHolder = std::move(func.pFuncHolder);
    return *this;
}

template<typename TReturn, typename... TParams>
TReturn ZC_Function<TReturn(TParams...)>::operator () (TParams... params) const
{
    if (!pFuncHolder) throw ZC_Exception(ZC_MakeErrorString("Call ZC_Function() with function pointer = nullptr!", __FILE__, __LINE__));
    return (*pFuncHolder.Get())(std::forward<TParams>(params)...);
}

template<typename TReturn, typename... TParams>
ZC_Function<TReturn(TParams...)>::operator bool () noexcept
{
    return pFuncHolder != nullptr;
}