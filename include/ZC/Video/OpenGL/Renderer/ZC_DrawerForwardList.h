#pragma once

#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <forward_list>
#include <cassert>

template<typename... TArgs>
struct ZC_DrawerForwardList;
template<typename TFirst, typename... TTail>
struct ZC_DrawerForwardList<TFirst, TTail...>
{
    struct Pair
    {
        const TFirst first;     //  must realize method ActivateOpenGL()
        ZC_DrawerForwardList<TTail...> second;

        bool operator == (const TFirst& find) const noexcept;
    };
    
    std::forward_list<Pair> pairs;

    ZC_DrawerForwardList() = default;
    virtual ~ZC_DrawerForwardList() = default;
    
    ZC_DrawerForwardList(TFirst _first, TTail... _tail);
    void Add(TFirst _first, TTail... _tail);
    //  returns true if empty
    bool Erase(const TFirst& _first, const TTail&... _tail);
    void Draw();
    //  Specifies the need to call ActivateOpenGL() on concrete level or not. All TStart, TEnd must be bool types.
    //  Count of parameters must correspond count of types that realize ActivateOpenGL() function (classes like: ZC_ShProg, ZC_VAO, ZC_TexturesHolder).
    template<typename TStart, typename... TEnd>
    void Draw(TStart activateOpenGLHead, TEnd... activateOpenGLEnd);
    bool Empty() const noexcept;

// private:
    void Draw(bool activateOpenGLEnd);
};

template<typename TRLData>
struct ZC_DrawerForwardList<TRLData>
{
    std::forward_list<TRLData> datas;     //  must realize method Draw()

    ZC_DrawerForwardList() = default;
    virtual ~ZC_DrawerForwardList() = default;

    ZC_DrawerForwardList(const TRLData& data);
    void Add(const TRLData& data);
    //  returns true if empty
    bool Erase(const TRLData& data);
    void Draw();
    bool Empty() const noexcept;
};


//  ZC_DrawerForwardList<TFirst, TTail...>

template<typename TFirst, typename... TTail>
ZC_DrawerForwardList<TFirst, TTail...>::ZC_DrawerForwardList(TFirst _first, TTail... _tail)
{
    pairs.emplace_front(std::forward<TFirst>(_first), ZC_DrawerForwardList<TTail...>(std::forward<TTail>(_tail)...));
}

template<typename TFirst, typename... TTail>
void ZC_DrawerForwardList<TFirst, TTail...>::Add(TFirst _first, TTail... _tail)
{
    auto pPair = ZC_Find(pairs, _first);
    if (pPair) pPair->second.Add(_tail...);
    else pairs.emplace_front(std::forward<TFirst>(_first), ZC_DrawerForwardList<TTail...>(std::forward<TTail>(_tail)...));
}

template<typename TFirst, typename... TTail>
bool ZC_DrawerForwardList<TFirst, TTail...>::Erase(const TFirst& _first, const TTail&... _tail)
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
void ZC_DrawerForwardList<TFirst, TTail...>::Draw()
{
    for (auto& pair : pairs)
    {
        if constexpr (std::is_pointer<TFirst>()) pair.first->ActivateOpenGL();
        else pair.first.ActivateOpenGL();
        pair.second.Draw();
    }
}

template<typename TFirst, typename... TTail>
template<typename TStart, typename... TEnd>
void ZC_DrawerForwardList<TFirst, TTail...>::Draw(TStart activateOpenGLHead, TEnd... activateOpenGLEnd)
{
    static_assert(std::same_as<bool, TStart>);
    for (auto& pair : pairs)
    {
        if (activateOpenGLHead)
        {
            if constexpr (std::is_pointer<TFirst>()) pair.first->ActivateOpenGL();
            else pair.first.ActivateOpenGL();
        }
        pair.second.Draw(activateOpenGLEnd...);
    }
}

template<typename TFirst, typename... TTail>
bool ZC_DrawerForwardList<TFirst, TTail...>::Empty() const noexcept
{
    return pairs.empty();
}

template<typename TFirst, typename... TTail>
void ZC_DrawerForwardList<TFirst, TTail...>::Draw(bool activateOpenGLEnd)
{
    for (auto& pair : pairs)
    {
        if (activateOpenGLEnd)
        {
            if constexpr (std::is_pointer<TFirst>()) pair.first->ActivateOpenGL();
            else pair.first.ActivateOpenGL();
        }
        pair.second.Draw();
    }
}



//  Pair

template<typename TFirst, typename... TTail>
bool ZC_DrawerForwardList<TFirst, TTail...>::Pair::operator == (const TFirst& find) const noexcept
{
    return first == find;
}


//  ZC_RLDFL<TRLData>

template<typename TRLData>
ZC_DrawerForwardList<TRLData>::ZC_DrawerForwardList(const TRLData& data)
{
    datas.emplace_front(data);
}

template<typename TRLData>
void ZC_DrawerForwardList<TRLData>::Add(const TRLData& data)
{
    datas.emplace_front(data);
}

template<typename TRLData>
bool ZC_DrawerForwardList<TRLData>::Erase(const TRLData& data)
{
    if (!ZC_ForwardListErase(datas, data)) { assert(false); }   //  an't find renderer level data for erase
    return datas.empty();
}

template<typename TRLData>
void ZC_DrawerForwardList<TRLData>::Draw()
{
    for (auto& data : datas) data.Draw();
}

template<typename TRLData>
bool ZC_DrawerForwardList<TRLData>::Empty() const noexcept
{
    return datas.empty();
}