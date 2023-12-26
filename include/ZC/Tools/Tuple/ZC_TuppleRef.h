// #pragma once

// #include <utility>

// template<typename... TArgs>
// struct ZC_TupleRef;
// template<typename TFirst, typename... TTail>
// struct ZC_TupleRef<TFirst, TTail...> : ZC_TupleRef<TTail...>
// {
//     typedef std::pair<bool, TFirst&> TPair;
    
//     TPair pair;
//     ZC_TupleRef<TTail...>& tail;

//     ZC_TupleRef() = delete;
    
//     ZC_TupleRef(TFirst& _first, TTail&... _tail)
//         : ZC_TupleRef<TTail...>(_tail...),
//         pair(std::is_lvalue_reference<TFirst>::value, _first),
//         tail(static_cast<ZC_TupleRef<TTail...>&>(*this))
//     {}
// };

// template<>
// struct ZC_TupleRef<>
// {};