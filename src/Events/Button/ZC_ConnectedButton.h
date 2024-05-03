#pragma once

#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Events/ZC_ButtonID.h>

template<typename TFunc>    //  TFunc -> ZC_Function<void(float)> or ZC_Function<void(float)>*
struct ZC_ConnectedButton
{
    ZC_ButtonID buttonId;
    TFunc func;

    bool operator == (ZC_ButtonID _buttonId) const noexcept
    {
        return buttonId == _buttonId;
    }

    bool operator == (const void* pFunc) const noexcept
    {
        if constexpr (std::is_pointer<TFunc>())
        {
            return (*func).GetPointerOnData() == pFunc;
        }
        else
        {
            return func.GetPointerOnData() == pFunc;
        }
    }

    void Call(float time)
    {
        if constexpr (std::same_as<TFunc, ZC_Function<void(ZC_ButtonID, float)>*>)
        {
            if (func) (*func)(buttonId, time);
        }
        else
        {
            func(buttonId, time);
        }
    }
};