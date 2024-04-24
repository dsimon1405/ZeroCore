#pragma once

#include <forward_list>

/*
Erases from forward list first found data. Type(TCont) storing in forward list must determine for searching type(TFind) operator:
bool operator == (const TFind&) const {}

Params:
rCont - reference on forward list.
find - value for searching.

Return:
true if value was erased, otherwise false.
*/
template<typename TCont, typename TFind>
bool ZC_ForwardListErase(std::forward_list<TCont>& rCont, const TFind& find)
{
    auto prev = rCont.before_begin();
    for (auto rContI = rCont.begin(); rContI != rCont.end(); ++rContI)
    {
        if (*rContI == find)
        {
            rCont.erase_after(prev);
            return true;
        }
        prev = rContI;
    }
    return false;
}

// /*
// Erases from forward list first found data.

// Params:
// rCont - reference on forward list.
// find - pointer on object from forward list.

// Return:
// true if value was erased, otherwise false.
// */
// template<typename TCont>
// bool ZC_ForwardListEraseByPointer(std::forward_list<TCont>& rCont, const TCont* pFind)
// {
//     auto prev = rCont.before_begin();
//     for (auto rContI = rCont.begin(); rContI != rCont.end(); ++rContI)
//     {
//         if (&*rContI == pFind)
//         {
//             rCont.erase_after(prev);
//             return true;
//         }
//         prev = rContI;
//     }
//     return false;
// }

template<typename TData, typename TAllocator, template<typename, typename> typename TCont, typename TFind>
TData* ZC_Find(TCont<TData, TAllocator>& fl, const TFind& find)
{
    for (auto& rContData : fl) if (rContData == find) return &rContData;
    return nullptr;
}