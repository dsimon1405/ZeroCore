// #pragma once

// #include "ZC_Signal.h"

// #include <map>
// #include <vector>
// #include <set>

// template<typename T>
// concept ZC_cLessCompare = requires(const T& t1, const T& t2)
// {
//     {t1 < t2} -> std::same_as<bool>;
// };

// template<ZC_cLessCompare TCondition, typename TFunction>
// class ZC_Signals;

// //  Collection of the Signals sorted by some condition type. Condition type must realize operator less than. 
// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// class ZC_Signals<TCondition, TReturn(TParams...)>
// {
// public:
//     ZC_Signals() = default;

//     ZC_Signals(const ZC_Signals<TCondition, TReturn(TParams...)>&) = delete;
//     ZC_Signals<TCondition, TReturn(TParams...)>& operator = (const ZC_Signals<TCondition, TReturn(TParams...)>&) = delete;

//     ZC_Signals(ZC_Signals<TCondition, TReturn(TParams...)>&& sig) noexcept;
//     ZC_Signals<TCondition, TReturn(TParams...)>& operator = (ZC_Signals<TCondition, TReturn(TParams...)>&& sig) noexcept;
    
//     /*
//     Connects ZC_Function with ZC_Signal according to the corresponding condition.

//     Params:
//     cond - condition (uses copy constructor).
//     func - ZC_Function for connection.

//     Return:
//     ZC_SConnection pointer.
//     */
//     ZC_SConnection<TReturn(TParams...)> Connect(const TCondition& cond, ZC_Function<TReturn(TParams...)>&& func);

//     /*
//     Connects ZC_Function with ZC_Signal according to the corresponding condition.

//     Params:
//     cond - condition (uses move constructor).
//     func - ZC_Function for connection.

//     Return:
//     ZC_SConnection pointer.
//     */
//     ZC_SConnection<TReturn(TParams...)> Connect(TCondition&& cond, ZC_Function<TReturn(TParams...)>&& func);
    
//     /*
//     Calls all connected ZC_Function.

//     Params:
//     params - parameters of the functions.
//     */
//     void operator () (TParams... params);

//     /*
//     Calls signals that match the conditions (set of Conditions).

//     Params:
//     cond - condition.
//     params - parameters of the functions.
//     */
//     void operator () (const TCondition& cond, TParams... params);

//     struct Conditions;
//     /*
//     Calls signals that match the conditions (set of Conditions).

//     Params:
//     conds - conditions.
//     params - parameters of the functions.
//     */
//     void operator () (const typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions& conds, TParams... params);
    
//     struct Container;
//     /*
//     Calls all signals and fills the vector with the values ​​returned from the called functions.

//     Params:
//     container - container.
//     params - parameters of the functions.
//     */
//     void operator () (std::vector<TReturn>& container, TParams... params);
    
//     /*
//     Calls signals that match the conditions added to the container and fills the container with the values ​​returned from the called functions.

//     Params:
//     container - container.
//     params - parameters of the functions.
//     */
//     void operator () (typename ZC_Signals<TCondition, TReturn(TParams...)>::Container& container, TParams... params);

//     //  Create Conditions using copy to parameter cond.
//     typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions GetConditions(const TCondition& cond) noexcept;
//     //  Create Conditions using move to parameter cond.
//     typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions GetConditions(TCondition&& cond) noexcept;

//     //  Create Container using copy to parameter cond.
//     typename ZC_Signals<TCondition, TReturn(TParams...)>::Container GetContainer(const TCondition& cond) noexcept;
//     //  Create Container using move to parameter cond.
//     typename ZC_Signals<TCondition, TReturn(TParams...)>::Container GetContainer(TCondition&& cond) noexcept;

// private:
//     std::map<TCondition, ZC_Signal<TReturn(TParams...)>> signals {};
//     ZC_sptr<std::mutex> signalsMutex = ZC_sptrMake<std::mutex>();

//     template<typename TCond>
//     ZC_Signal<TReturn(TParams...)>& GetSignal(TCond&& cond);

// public:
//     //  Wrapper for set of conditions.
//     struct Conditions
//     {
//         std::set<TCondition> conditions;
        
//         explicit Conditions(const TCondition& cond) noexcept;
//         explicit Conditions(TCondition&& cond) noexcept;

//         Conditions(const typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions&) = delete;
//         Conditions& operator = (const typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions&) = delete;

//         Conditions(typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions&& cont) noexcept;
//         typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions& operator = (typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions&& cont) noexcept;
        
//         /*
//         Copy a condition if it doesn't exist, if the condition allready exists, nothing happens.

//         Params:
//         cond - signals condition.

//         Return:
//         Pointer to itself.
//         */
//         typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions& Add(const TCondition& cond) noexcept;

//         /*
//         Move a condition if it doesn't exist, if the condition allready exists, nothing happens.

//         Params:
//         cond - signals condition.

//         Return:
//         Pointer to itself.
//         */
//         typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions& Add(TCondition&& cond) noexcept;
//     };

//     // Wrapper for a map of conditions that have vectors with a return values.
//     struct Container
//     {
//         std::map<TCondition, std::vector<TReturn>> datas;

//         Container() = default;
//         Container(const TCondition& cond) noexcept;
//         Container(TCondition&& cond) noexcept;

//         Container(const typename ZC_Signals<TCondition, TReturn(TParams...)>::Container&) = delete;
//         Container& operator = (const typename ZC_Signals<TCondition, TReturn(TParams...)>::Container&) = delete;

//         Container(typename ZC_Signals<TCondition, TReturn(TParams...)>::Container&& cont) noexcept;
//         typename ZC_Signals<TCondition, TReturn(TParams...)>::Container& operator = (typename ZC_Signals<TCondition, TReturn(TParams...)>::Container&& cont) noexcept;
        
//         /*
//         Copy a condition if it doesn't exist, if the condition allready exists, nothing happens.

//         Params:
//         cond - signals condition.

//         Return:
//         Pointer to itself.
//         */
//         typename ZC_Signals<TCondition, TReturn(TParams...)>::Container& Add(const TCondition& cond) noexcept;

//         /*
//         Move a condition if it doesn't exist, if the condition allready exists, nothing happens.

//         Params:
//         cond - signals condition.

//         Return:
//         Pointer to itself.
//         */
//         typename ZC_Signals<TCondition, TReturn(TParams...)>::Container& Add(TCondition&& cond) noexcept;

//         /*
//         Return vector of values add in ZC_Signals call.

//         Params:
//         cond - signals condition.

//         Return:
//         Vector of values.
//         */
//         std::vector<TReturn>* GetData(const TCondition& cond) noexcept;
//     };
// };

// //  Signals start

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_Signals<TCondition, TReturn(TParams...)>::ZC_Signals(ZC_Signals<TCondition, TReturn(TParams...)>&& sig) noexcept
// {
//     signals = std::move(sig.signals);
//     signalsMutex = std::move(sig.signalsMutex);
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_Signals<TCondition, TReturn(TParams...)>& ZC_Signals<TCondition, TReturn(TParams...)>::operator = (ZC_Signals<TCondition, TReturn(TParams...)>&& sig) noexcept
// {
//     signals = std::move(sig.signals);
//     signalsMutex = std::move(sig.signalsMutex);
//     return *this;
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_SConnection<TReturn(TParams...)> ZC_Signals<TCondition, TReturn(TParams...)>::Connect(const TCondition& cond, ZC_Function<TReturn(TParams...)>&& func)
// {
//     return GetSignal(cond).Connect(std::move(func));
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_SConnection<TReturn(TParams...)> ZC_Signals<TCondition, TReturn(TParams...)>::Connect(TCondition&& cond, ZC_Function<TReturn(TParams...)>&& func)
// {
//     return GetSignal(std::move(cond)).Connect(std::move(func));
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// void ZC_Signals<TCondition, TReturn(TParams...)>::operator () (TParams... params)
// {
//     std::lock_guard<std::mutex> lock(*this->signalsMutex);
//     for (auto signalsIter = this->signals.begin(); signalsIter != this->signals.end();)
//     {
//         if (signalsIter->second.Size() == 0)
//         {
//             signalsIter = this->signals.erase(signalsIter);
//             continue;
//         }
//         signalsIter->second(params...);
//         ++signalsIter;
//     }
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// void ZC_Signals<TCondition, TReturn(TParams...)>::operator () (const TCondition& cond, TParams... params)
// {
//     std::lock_guard<std::mutex> lock(*this->signalsMutex);
//     auto signalsIter = this->signals.find(cond);
//     if (signalsIter == this->signals.end()) return;
//     if (signalsIter->second.Size() == 0)
//     {
//         this->signals.erase(signalsIter);
//         return;
//     }
//     signalsIter->second(params...);
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// void ZC_Signals<TCondition, TReturn(TParams...)>::operator () (const typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions& conds, TParams... params)
// {
//     std::lock_guard<std::mutex> lock(*this->signalsMutex);
//     for (auto& cond : conds.conditions)
//     {
//         auto signalsIter = this->signals.find(cond);
//         if (signalsIter == this->signals.end()) continue;
//         if (signalsIter->second.Size() == 0)
//         {
//             this->signals.erase(signalsIter);
//             continue;
//         }
//         signalsIter->second(params...);
//     }
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// void ZC_Signals<TCondition, TReturn(TParams...)>::operator () (std::vector<TReturn>& container, TParams... params)
// {
//     std::lock_guard<std::mutex> lock(*this->signalsMutex);
//     size_t containerSize = 0;
//     for (auto signalsIter = this->signals.begin(); signalsIter != this->signals.end();)
//     {
//         auto size = signalsIter->second.Size();
//         containerSize += size;
//         signalsIter = size == 0 ? this->signals.erase(signalsIter) : ++signalsIter;
//     }
//     container.reserve(containerSize);
    
//     for (auto& signal : this->signals) signal.second(container, params...);
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// void ZC_Signals<TCondition, TReturn(TParams...)>::operator () (typename ZC_Signals<TCondition, TReturn(TParams...)>::Container& container, TParams... params)
// {
//     std::lock_guard<std::mutex> lock(*this->signalsMutex);
//     for (auto datasIter = container.datas.begin(); datasIter != container.datas.end();)
//     {
//         auto signalsIter = this->signals.find(datasIter->first);
//         if (signalsIter == this->signals.end())
//         {
//             datasIter = container.datas.erase(datasIter);
//             continue;
//         }
//         auto signalSize = signalsIter->second.Size();
//         if (signalSize == 0)
//         {
//             datasIter = container.datas.erase(datasIter);
//             this->signals.erase(signalsIter);
//             continue;
//         }
//         datasIter->second.reserve(signalSize);
//         signalsIter->second(datasIter->second, params...);
//         ++datasIter;
//     }
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions ZC_Signals<TCondition, TReturn(TParams...)>::GetConditions(const TCondition& cond) noexcept
// {
//     return ZC_Signals<TCondition, TReturn(TParams...)>::Conditions(cond);
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions ZC_Signals<TCondition, TReturn(TParams...)>::GetConditions(TCondition&& cond) noexcept
// {
//     return ZC_Signals<TCondition, TReturn(TParams...)>::Conditions(std::move(cond));
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Container ZC_Signals<TCondition, TReturn(TParams...)>::GetContainer(const TCondition& cond) noexcept
// {
//     return ZC_Signals<TCondition, TReturn(TParams...)>::Container(cond);
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Container ZC_Signals<TCondition, TReturn(TParams...)>::GetContainer(TCondition&& cond) noexcept
// {
//     return ZC_Signals<TCondition, TReturn(TParams...)>::Container(std::move(cond));
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// template<typename TCond>
// ZC_Signal<TReturn(TParams...)>& ZC_Signals<TCondition, TReturn(TParams...)>::GetSignal(TCond&& cond)
// {
//     std::lock_guard<std::mutex> lock(*signalsMutex);
//     auto signalsIter = signals.find(cond);
//     if (signalsIter == signals.end()) return signals.emplace(std::forward<TCond>(cond), ZC_Signal<TReturn(TParams...)>(signalsMutex)).first->second;
//     return signalsIter->second;
// }
// //  Signals end

// //  Conditions start

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_Signals<TCondition, TReturn(TParams...)>::Conditions::Conditions(const TCondition& cond) noexcept
// {
//     conditions.emplace(cond);
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_Signals<TCondition, TReturn(TParams...)>::Conditions::Conditions(TCondition&& cond) noexcept
// {
//     conditions.emplace(std::move(cond));
// }
        
// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_Signals<TCondition, TReturn(TParams...)>::Conditions::Conditions(typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions&& cond) noexcept
//     : conditions(std::move(cond.conditions))
// {}

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions& ZC_Signals<TCondition, TReturn(TParams...)>::Conditions::operator = (typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions&& cond) noexcept
// {
//     conditions = std::move(cond.conditions);
//     return *this;
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions& ZC_Signals<TCondition, TReturn(TParams...)>::Conditions::Add(const TCondition& cond) noexcept
// {
//     conditions.emplace(cond);
//     return *this;
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Conditions& ZC_Signals<TCondition, TReturn(TParams...)>::Conditions::Add(TCondition&& cond) noexcept
// {
//     conditions.emplace(std::move(cond));
//     return *this;
// }
// //  Conditions end

// //  Container start

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_Signals<TCondition, TReturn(TParams...)>::Container::Container(const TCondition& cond) noexcept
// {
//     datas.emplace(cond, std::vector<TReturn>());
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_Signals<TCondition, TReturn(TParams...)>::Container::Container(TCondition&& cond) noexcept
// {
//     datas.emplace(std::move(cond), std::vector<TReturn>());
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// ZC_Signals<TCondition, TReturn(TParams...)>::Container::Container(typename ZC_Signals<TCondition, TReturn(TParams...)>::Container&& cont) noexcept
//     : datas(std::move(cont.datas))
// {}

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Container& ZC_Signals<TCondition, TReturn(TParams...)>::Container::operator = (typename ZC_Signals<TCondition, TReturn(TParams...)>::Container&& cont) noexcept
// {
//     datas = std::move(cont.datas);
//     return *this;
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Container& ZC_Signals<TCondition, TReturn(TParams...)>::Container::Add(const TCondition& cond) noexcept
// {
//     datas.emplace(cond, std::vector<TReturn>());
//     return *this;
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// typename ZC_Signals<TCondition, TReturn(TParams...)>::Container& ZC_Signals<TCondition, TReturn(TParams...)>::Container::Add(TCondition&& cond) noexcept
// {
//     datas.emplace(std::move(cond), std::vector<TReturn>());
//     return *this;
// }

// template<ZC_cLessCompare TCondition, ZC_cNotRValueRef TReturn, typename... TParams>
// std::vector<TReturn>* ZC_Signals<TCondition, TReturn(TParams...)>::Container::GetData(const TCondition& cond) noexcept
// {
//     auto datasIter = datas.find(cond);
//     if (datasIter == datas.end()) return nullptr;
//     return &datasIter->second;
// }
// //  Container end