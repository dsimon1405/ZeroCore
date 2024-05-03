#pragma once

#include <forward_list>

/*
Erases from forward list first found data. Type(TData) storing in forward list may be pointer on TCont or must determine for searching type(TFind) operator:
bool operator == (const TFind&) const {}

Params:
- rCont - reference on forward list.
- find - value for searching.

Return:
true if value was erased, otherwise false.
*/
template<typename TData, typename TFind>
bool ZC_ForwardListErase(std::forward_list<TData>& rCont, const TFind& find)
{
    auto prev = rCont.before_begin();
    for (auto rContI = rCont.begin(); rContI != rCont.end(); ++rContI)
    {
        if constexpr (std::same_as<TData*, TFind>)  //  TFind is pointer on TData
        {
            if (&*rContI == find)   //  take pointer on TData for compare with TFind
            {
                rCont.erase_after(prev);
                return true;
            }
        }
        else
        {
            if (*rContI == find)
            {
                rCont.erase_after(prev);
                return true;
            }
        }
        prev = rContI;
    }
    return false;
}

/*
Find data in stl container. Type(TData) storing in container may be pointer on TCont or must determine for searching type(TFind) operator:
bool operator == (const TFind&) const {}

Params:
- rCont - reference on stl container.
- find - value for searching.

Return:
If found, pointer on store data, otherwise - nullptr.
*/
template<typename TData, typename TAllocator, template<typename, typename> typename TCont, typename TFind>
TData* ZC_Find(TCont<TData, TAllocator>& container, const TFind& find)
{
    for (auto& rContData : container)
    {
        if constexpr (std::same_as<TData*, TFind>)
        {
            if (&rContData == find) return &rContData;
        }
        else
        {
            if (rContData == find) return &rContData;
        }
    }
    return nullptr;
}

/*
Find data in stl container of pointers. Type(TData) storing in container must determine for searching type(TFind) operator:
bool operator == (const TFind&) const {}

Params:
- rCont - reference on stl container.
- find - value for searching.

Return:
If found, pointer on store data, otherwise - nullptr.
*/
template<typename TData, typename TAllocator, template<typename, typename> typename TCont, typename TFind>
TData ZC_FindInPointers(TCont<TData, TAllocator>& container, const TFind& find)
{
    for (auto rContData : container) if (*rContData == find) return rContData;
    return nullptr;
}