// #pragma once

// #include <ZC/Tools/Function/ZC_Function.h>
// #include <ZC/Tools/ZC_sptr.h>

// #include <list>
// #include <mutex>
// #include <concepts>

// template<typename T>
// concept ZC_cNotRValueRef = !std::is_rvalue_reference<T>::value;

// template<typename TFunc>
// struct ZC_SCData;
// template<typename TReturn, ZC_cNotRValueRef... TParams>
// struct ZC_SCData<TReturn(TParams...)>
// {
//     std::list<ZC_Function<TReturn(TParams...)>> functions {};
//     ZC_sptr<std::mutex> functionsMutex = ZC_sptrMake<std::mutex>();

//     ZC_SCData() = default;
//     ZC_SCData(const ZC_sptr<std::mutex>& _functionsMutex) noexcept;

//     ZC_SCData(ZC_SCData<TReturn(TParams...)>&& scd) noexcept;
//     ZC_SCData<TReturn(TParams...)>& operator = (ZC_SCData<TReturn(TParams...)>&& scd) noexcept;

//     typename std::list<ZC_Function<TReturn(TParams...)>>::iterator EmplaceBack(ZC_Function<TReturn(TParams...)>&& function);
//     void Erase(typename std::list<ZC_Function<TReturn(TParams...)>>::iterator functionsIter);
// };

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_SCData<TReturn(TParams...)>::ZC_SCData(const ZC_sptr<std::mutex>& _functionsMutex) noexcept
//     : functionsMutex(_functionsMutex)
// {}

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_SCData<TReturn(TParams...)>::ZC_SCData(ZC_SCData<TReturn(TParams...)>&& scd) noexcept
//     : functions(std::move(scd.functions)),
//     functionsMutex(std::move(scd.functionsMutex))
// {}

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_SCData<TReturn(TParams...)>& ZC_SCData<TReturn(TParams...)>::operator = (ZC_SCData<TReturn(TParams...)>&& scd) noexcept
// {
//     functions = std::move(scd.functions);
//     functionsMutex = std::move(scd.functionsMutex);
//     return *this;
// }

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// typename std::list<ZC_Function<TReturn(TParams...)>>::iterator ZC_SCData<TReturn(TParams...)>::EmplaceBack(ZC_Function<TReturn(TParams...)>&& function)
// {
//     std::lock_guard<std::mutex> lock(*functionsMutex);
//     functions.emplace_back(std::move(function));
//     return --functions.end();
// }

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// void ZC_SCData<TReturn(TParams...)>::Erase(typename std::list<ZC_Function<TReturn(TParams...)>>::iterator functionsIter)
// {
//     std::lock_guard<std::mutex> lock(*functionsMutex);
//     functions.erase(functionsIter);
// }