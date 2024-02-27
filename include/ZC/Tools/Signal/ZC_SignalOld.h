// #pragma once

// #include "ZC_SConnection.h"

// #include <vector>

// template<typename TFunction>
// class ZC_Signal;
// /*
// Class for calling any number of ZC_Functions with the same signature. It is not possible to create a signal for a function/method that accepts rvalue
// reference type parameters.
// */
// template<typename TReturn, ZC_cNotRValueRef... TParams>
// class ZC_Signal<TReturn(TParams...)>
// {
// public:
//     //  Creates a ZC_Signal with the function signature <TReturn(TParams...)>.
//     ZC_Signal() = default;
//     ZC_Signal(const ZC_sptr<std::mutex>& spMutex) noexcept;

//     ZC_Signal(ZC_Signal<TReturn(TParams...)>&& sig) noexcept;
//     ZC_Signal<TReturn(TParams...)>& operator = (ZC_Signal<TReturn(TParams...)>&& sig) noexcept;

//     /*
//     Connects ZC_Function.

//     Params:
//     func - ZC_Function for connection.

//     Return:
//     ZC_SConnection pointer.
//     */
//     ZC_SConnection<TReturn(TParams...)> Connect(ZC_Function<TReturn(TParams...)>&& func);

//     //  Return the number of added functions.
//     // unsigned long Size() const noexcept;
    
//     /*
//     Calls all connected ZC_Function.

//     Params:
//     params - parameters of the functions.
//     */
//     void operator()(TParams... params);
    
//     /*
//     Calls all connected ZC_Function and store return values in vector.

//     Params:
//     vec - a reference to a vector that will be filled with the function's return values.
//     params - parameters of the functions.
//     */
//     void operator()(std::vector<TReturn>& container, TParams... params);

// private:
//     ZC_SCData<TReturn(TParams...)> scd {};
// };

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_Signal<TReturn(TParams...)>::ZC_Signal(const ZC_sptr<std::mutex>& spMutex) noexcept
//     : scd(spMutex)
// {}

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_Signal<TReturn(TParams...)>::ZC_Signal(ZC_Signal<TReturn(TParams...)>&& sig) noexcept
//     : scd(std::move(sig.scd))
// {}

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_Signal<TReturn(TParams...)>& ZC_Signal<TReturn(TParams...)>::operator = (ZC_Signal<TReturn(TParams...)>&& sig) noexcept
// {
//     scd = std::move(sig.scd);
//     return *this;
// }

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_SConnection<TReturn(TParams...)> ZC_Signal<TReturn(TParams...)>::Connect(ZC_Function<TReturn(TParams...)>&& func)
// {
//     return { &scd, scd.EmplaceBack(std::move(func)) };
// }

// // template<typename TReturn, ZC_cNotRValueRef... TParams>
// // unsigned long ZC_Signal<TReturn(TParams...)>::Size() const noexcept
// // {
// //     return scd.functions.size();
// // }

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// void ZC_Signal<TReturn(TParams...)>::operator () (TParams... params)
// {
//     if (scd.functionsMutex.UseCount() == 1) //  use in ZC_Singnal lock here
//     {
//         std::lock_guard<std::mutex> lock(*scd.functionsMutex);
//         for (auto& func : scd.functions) func(params...);
//     }
//     else    //  use in ZC_Signals and locked there 
//     {
//         for (auto& func : scd.functions) func(params...);
//     }
// }

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// void ZC_Signal<TReturn(TParams...)>::operator () (std::vector<TReturn>& container, TParams... params)
// {
//     if (scd.functionsMutex.UseCount() == 1) //  use in ZC_Singnal lock here
//     {
//         std::lock_guard<std::mutex> lock(*scd.functionsMutex);
//         container.reserve(scd.functions.size());
//         for (auto& func : scd.functions) container.emplace_back(func(params...));
//     }
//     else    //  use in ZC_Signals and locked there 
//     {
//         for (auto& func : scd.functions) container.emplace_back(func(params...));
//     }
// }






















// #pragma once

// #include "ZC_SignalConnector.h"

// #include <vector>

// template<typename TFunction>
// class ZC_Signal;
// /*
// Class for calling any number of ZC_Functions with the same signature. It is not possible to create a signal for a function/method that accepts rvalue
// reference type parameters.
// */
// template<typename TReturn, ZC_cNotRValueRef... TParams>
// class ZC_Signal<TReturn(TParams...)> : public ZC_SignalConnector<TReturn(TParams...)>
// {
// public:
//     //  Creates a ZC_Signal with the function signature <TReturn(TParams...)>.
//     ZC_Signal() = default;

//     ZC_Signal(const ZC_Signal<TReturn(TParams...)>&) = delete;
//     ZC_Signal<TReturn(TParams...)>& operator = (const ZC_Signal<TReturn(TParams...)>&) = delete;

//     ZC_Signal(ZC_Signal<TReturn(TParams...)>&& sig) noexcept;
//     ZC_Signal<TReturn(TParams...)>& operator = (ZC_Signal<TReturn(TParams...)>&& sig) noexcept;

//     ~ZC_Signal() override = default;
    
//     /*
//     Calls all connected ZC_Function.

//     Params:
//     params - parameters of the functions.
//     */
//     void operator()(TParams... params) noexcept;
    
//     /*
//     Calls all connected ZC_Function.

//     Params:
//     vec - a reference to a vector that will be filled with the function's return values.
//     params - parameters of the functions.
//     */
//     void operator()(std::vector<TReturn>& container, TParams... params, bool needReserve = true) noexcept;
    
//     //  Create Connector to the signal. Usefull for hide signal in private zone.
//     ZC_SignalConnector<TReturn(TParams...)>* GetConnector() noexcept;
// };

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_Signal<TReturn(TParams...)>::ZC_Signal(ZC_Signal<TReturn(TParams...)>&& sig) noexcept
// {
//     this->functions = std::move(sig.functions);
// }

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_Signal<TReturn(TParams...)>& ZC_Signal<TReturn(TParams...)>::operator = (ZC_Signal<TReturn(TParams...)>&& sig) noexcept
// {
//     this->functions = std::move(sig.functions);
// }

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// void ZC_Signal<TReturn(TParams...)>::operator () (TParams... params) noexcept
// {
//     std::lock_guard<std::mutex> lock(this->functionsMutex);
//     for (auto functionsIter = this->functions.begin(); functionsIter != this->functions.end();)
//     {
//         switch (functionsIter->first->GetState())
//         {
//             case ZC_SignalConnection::State::Connected:
//                 functionsIter->second(params...);
//                 ++functionsIter;
//                 break;
//             case ZC_SignalConnection::State::Disconnected:
//                 ++functionsIter;
//                 break;
//             case ZC_SignalConnection::State::Terminated:
//                 functionsIter = this->functions.erase(functionsIter);
//                 break;
//         }
//     }
// }

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// void ZC_Signal<TReturn(TParams...)>::operator () (std::vector<TReturn>& container, TParams... params, bool needReserve) noexcept
// {
//     if (needReserve) container.reserve(this->Size());
//     std::lock_guard<std::mutex> lock(this->functionsMutex);
//     for (auto functionsIter = this->functions.begin(); functionsIter != this->functions.end();)
//     {
//         switch (functionsIter->first->GetState())
//         {
//             case ZC_SignalConnection::State::Connected:
//                 container.emplace_back(functionsIter->second(params...));
//                 ++functionsIter;
//                 break;
//             case ZC_SignalConnection::State::Disconnected:
//                 ++functionsIter;
//                 break;
//             case ZC_SignalConnection::State::Terminated:
//                 functionsIter = this->functions.erase(functionsIter);
//                 break;
//         }
//     }
// }

// template<typename TReturn, ZC_cNotRValueRef... TParams>
// ZC_SignalConnector<TReturn(TParams...)>* ZC_Signal<TReturn(TParams...)>::GetConnector() noexcept
// {
//     return dynamic_cast<ZC_SignalConnector<TReturn(TParams...)>*>(this);
// }