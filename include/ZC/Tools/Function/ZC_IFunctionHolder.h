#pragma once

template<typename TFunc>
class ZC_IFunctionHolder;
template<typename TVal, typename... TParams>
class ZC_IFunctionHolder<TVal(TParams...)>
{
public:
    virtual ~ZC_IFunctionHolder() = default;

    virtual TVal operator()(TParams&&... params) const = 0;

protected:
    ZC_IFunctionHolder() = default;
};