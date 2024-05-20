#pragma once

#include "ZC_IFunctionHolder.h"

#include <utility>

template<typename TFunc, typename TClass = void>
class ZC_FunctionHolder;

template<typename TVal, typename... TParams>
class ZC_FunctionHolder<TVal(TParams...)> : public ZC_IFunctionHolder<TVal(TParams...)>
{
public:
    ZC_FunctionHolder() = delete;
    ZC_FunctionHolder(TVal(* _pFunc)(TParams...)) noexcept;
    
    ~ZC_FunctionHolder() override = default;
    
    TVal operator()(TParams&&... params) const override;

private:
    TVal(*pFunc)(TParams...);
};

template <typename TVal, typename... TParams, typename TClass>
class ZC_FunctionHolder<TVal(TParams...), TClass> : public ZC_IFunctionHolder<TVal(TParams...)>
{
public:
    ZC_FunctionHolder() = delete;
    ZC_FunctionHolder(TVal(TClass::* _pFunc)(TParams...), TClass* _pClass) noexcept;
    ZC_FunctionHolder(TVal(TClass::* _pFunc)(TParams...) const, TClass* _pClass) noexcept;
    ZC_FunctionHolder(TVal(TClass::* _pFunc)(TParams...) const, const TClass* _pClass) noexcept;

    ~ZC_FunctionHolder() override = default;
    
    TVal operator()(TParams&&... params) const override;

private:
    TClass* pClass;
    TVal(TClass::*pFunc)(TParams...);
};

//  start   template<typename TReturn, typename... TParams> ZC_FunctionHolder<TReturn(TParams...)>

template<typename TVal, typename... TParams>
ZC_FunctionHolder<TVal(TParams...)>::ZC_FunctionHolder(TVal(*_pFunc)(TParams...)) noexcept
    : pFunc(_pFunc)
{}

template<typename TVal, typename... TParams>
TVal ZC_FunctionHolder<TVal(TParams...)>::operator()(TParams&&... params) const
{
    return (*pFunc)(std::forward<TParams>(params)...);
}
//  end   template <typename TReturn, typename... TParams> ZC_FunctionHolder<TReturn(TParams...)>

//  start   template<typename TReturn, typename... TParams, typename TClass> class ZC_FunctionHolder<TReturn(TParams...), TClass>

template<typename TVal, typename... TParams, typename TClass>
ZC_FunctionHolder<TVal(TParams...), TClass>::ZC_FunctionHolder(TVal(TClass::* _pFunc)(TParams...), TClass* _pClass) noexcept
    : pClass(_pClass),
    pFunc(_pFunc)
{}

template<typename TVal, typename... TParams, typename TClass>
ZC_FunctionHolder<TVal(TParams...), TClass>::ZC_FunctionHolder(TVal(TClass::* _pFunc)(TParams...) const, TClass* _pClass) noexcept
    : pClass(_pClass),
    pFunc((TVal(TClass::*)(TParams...))(_pFunc))
{}

template<typename TVal, typename... TParams, typename TClass>
ZC_FunctionHolder<TVal(TParams...), TClass>::ZC_FunctionHolder(TVal(TClass::* _pFunc)(TParams...) const, const TClass* _pClass) noexcept
    : pClass(const_cast<TClass*>(_pClass)),
    pFunc((TVal(TClass::*)(TParams...))(_pFunc))
{}

template<typename TVal, typename... TParams, typename TClass>
TVal ZC_FunctionHolder<TVal(TParams...), TClass>::operator()(TParams&&... params) const
{
    return (pClass->*pFunc)(std::forward<TParams>(params)...);
}
//  end   template<typename TReturn, typename... TParams, typename TClass> class ZC_FunctionHolder<TReturn(TParams...), TClass>