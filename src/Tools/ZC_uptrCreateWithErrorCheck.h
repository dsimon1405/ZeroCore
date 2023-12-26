#pragma once

#include <ZC/Tools/ZC_uptr.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

template<typename TParant, typename TChild, typename... TParams>
ZC_uptr<TParant> ZC_uptrCreateWithErrorCheck(TParams&&... params)
{
    ZC_ErrorLogger::Clear();
    ZC_uptr<TParant> result = ZC_uptrMakeFromChild<TParant, TChild>(std::forward<TParams>(params)...);
    if (ZC_ErrorLogger::WasError()) return nullptr;
    return result;
}