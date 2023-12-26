// #pragma once

// #include "ZC_TuppleRef.h"

// template<unsigned long TIndex, typename TFirst, typename... TTail>
// struct ZC_TupleGetter
// {
//    typedef typename ZC_TupleGetter<TIndex-1, TTail...>::TReturn TReturn;
//    static TReturn& Get(ZC_TupleRef<TFirst, TTail...>& tuple)
//    {
//        return ZC_TupleGetter<TIndex-1, TTail...>::Get(tuple);
//    }
// };

// template<typename TFirst, typename... TTail>
// struct ZC_TupleGetter<0, TFirst, TTail...>
// {
//    typedef typename ZC_TupleRef<TFirst, TTail...>::TPair TReturn;

//    static TReturn& Get(ZC_TupleRef<TFirst, TTail...>& tuple)
//    {
//        return tuple.pair;
//    }
// };

// template<unsigned long TIndex, typename TFirst, typename... TTail>
// ZC_TupleGetter<TIndex, TFirst, TTail...>::TReturn ZC_TupleGet(ZC_TupleRef<TFirst, TTail...>& tuple)
// {
//    return ZC_TupleGetter<TIndex, TFirst, TTail...>::Get(tuple);
// };