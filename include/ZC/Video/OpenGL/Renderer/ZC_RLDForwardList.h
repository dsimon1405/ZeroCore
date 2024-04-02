#pragma once

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <forward_list>
#include <cassert>

template<typename... TArgs>
struct ZC_RLDForwardList;
template<typename TFirst, typename... TTail>
struct ZC_RLDForwardList<TFirst, TTail...> : public ZC_RLDForwardList<TTail...>
{
    struct Pair
    {
        const TFirst first;     //  must realize method ActivateOpenGL()
        ZC_RLDForwardList<TTail...> second;

        bool operator == (const TFirst& find) const noexcept;
    };
    
    std::forward_list<Pair> pairs;

    ZC_RLDForwardList() = default;
    virtual ~ZC_RLDForwardList() = default;
    
    ZC_RLDForwardList(const TFirst& _first, const TTail&... _tail);
    void Add(const TFirst& _first, const TTail&... _tail);
    //  returns true if empty
    bool Erase(const TFirst& _first, const TTail&... _tail);
    void Draw();
};

template<typename TRLData>
struct ZC_RLDForwardList<TRLData>
{
    std::forward_list<TRLData> datas;     //  must realize method Draw()

    ZC_RLDForwardList() = default;
    virtual ~ZC_RLDForwardList() = default;

    ZC_RLDForwardList(const TRLData& data);
    void Add(const TRLData& data);
    //  returns true if empty
    bool Erase(const TRLData& data);
    void Draw();
};


//  ZC_RLDFL<TFirst, TTail...>

template<typename TFirst, typename... TTail>
ZC_RLDForwardList<TFirst, TTail...>::ZC_RLDForwardList(const TFirst& _first, const TTail&... _tail)
{
    pairs.emplace_front(_first, ZC_RLDForwardList<TTail...>(_tail...));
}

template<typename TFirst, typename... TTail>
void ZC_RLDForwardList<TFirst, TTail...>::Add(const TFirst& _first, const TTail&... _tail)
{
    auto pPair = ZC_Find(pairs, _first);
    if (pPair) pPair->second.Add(_tail...);
    else pairs.emplace_front(_first, ZC_RLDForwardList<TTail...>(_tail...));
}

template<typename TFirst, typename... TTail>
bool ZC_RLDForwardList<TFirst, TTail...>::Erase(const TFirst& _first, const TTail&... _tail)
{
    auto prevIter = pairs.before_begin(),
        curIter = pairs.begin();
    for ( ; ; ++curIter)
    {
        if (*curIter == _first) break;
        assert(curIter != pairs.end());    //  can't find
        prevIter = curIter;
    }
    if (curIter->second.Erase(_tail...)) prevIter = pairs.erase_after(prevIter);  //  next level is empty, erase (need to assign to previous iterator to avoid exception in msvc)
    return pairs.empty();
}

template<typename TFirst, typename... TTail>
void ZC_RLDForwardList<TFirst, TTail...>::Draw()
{
    for (auto& pair : pairs)
    {
        if constexpr (std::is_pointer<TFirst>()) pair.first->ActivateOpenGL();
        else pair.first.ActivateOpenGL();
        pair.second.Draw();
    }
}


//  Pair

template<typename TFirst, typename... TTail>
bool ZC_RLDForwardList<TFirst, TTail...>::Pair::operator == (const TFirst& find) const noexcept
{
    return first == find;
}


//  ZC_RLDFL<TRLData>

template<typename TRLData>
ZC_RLDForwardList<TRLData>::ZC_RLDForwardList(const TRLData& data)
{
    datas.emplace_front(data);
}

template<typename TRLData>
void ZC_RLDForwardList<TRLData>::Add(const TRLData& data)
{
    datas.emplace_front(data);
}

template<typename TRLData>
bool ZC_RLDForwardList<TRLData>::Erase(const TRLData& data)
{
    if (!ZC_ForwardListErase(datas, data)) ZC_ErrorLogger::Err("ZC_RLDFL<TRLData>::Erase(), Can't find renderer level data for erase!", __FILE__, __LINE__);
    return datas.empty();
}

template<typename TRLData>
void ZC_RLDForwardList<TRLData>::Draw()
{
    for (auto& data : datas) data.Draw();
}