#pragma once

template<typename... TArgs>
class ZC_Tuple;

template<typename THead, typename... TTail>
class ZC_Tuple<THead, TTail...>
{
public:
    ZC_Tuple() = default;
    ZC_Tuple(THead _val, TTail... _tail);

    /*
    Returns reference on value of tuple by index. Template type TVal must equal to value at index, otherwise in: Debug - assert exeption, Release - undefined behaviour.
    On index out of range, in: Debug - assert exception, Release - undefined behaviour.
    */
    template<typename TVal>
    TVal& Get(unsigned int index);
    /*
    Sets value by index. Template type TVal must be convertable to value at index, otherwise in: Debug - assert exeption, Release - return false.
    On index out of range, in: Debug - assert exception, Release - return false.
    */
    template<typename TVal>
    bool Set(unsigned int index, TVal _val);

private:
    THead val;
    ZC_Tuple<TTail...> tail;
};

template<typename TEnd>
class ZC_Tuple<TEnd>
{
public:
    ZC_Tuple() = default;
    ZC_Tuple(TEnd _val);

    template<typename TVal>
    TVal& Get(unsigned int index);
    template<typename TVal>
    bool Set(unsigned int index, TVal _val);

private:
    TEnd val;
};


//  ZC_Tuple<THead, TTail...>

template<typename THead, typename... TTail>
ZC_Tuple<THead, TTail...>::ZC_Tuple(THead _val, TTail... _tail)
    : val(std::forward<THead>(_val)),
    tail(std::forward<TTail>(_tail)...)
{}

template<typename THead, typename... TTail>
template<typename TVal>
TVal& ZC_Tuple<THead, TTail...>::Get(unsigned int index)
{
    if (index == 0)
    {
        if constexpr (std::same_as<TVal, THead>) return val;
        else assert(false);     //  index of the wrong type
    }
    else return tail.template Get<TVal>(--index);   //  tail.template => explanation  https://stackoverflow.com/questions/3786360/confusing-template-error
}

template<typename THead, typename... TTail>
template<typename TVal>
bool ZC_Tuple<THead, TTail...>::Set(unsigned int index, TVal _val)
{
    if (index == 0)
    {
        if constexpr (std::convertible_to<TVal, THead>) val = _val;
        else
        {
            assert(false);     //  wrong index or not convertable type
            return false;
        }
    }
    else return tail.template Set<TVal>(--index, std::forward<TVal>(_val));

    return true;
}


//  ZC_Tuple<TEnd>

template<typename TEnd>
ZC_Tuple<TEnd>::ZC_Tuple(TEnd _val)
    : val(std::forward<TEnd>(_val))
{}

template<typename TEnd>
template<typename TVal>
TVal& ZC_Tuple<TEnd>::Get(unsigned int index)
{
    if (index == 0)
    {
        if constexpr (std::same_as<TVal, TEnd>) return val;
        else assert(false);     //  index of the wrong type
    }
    else assert(false);     //  index out or range
}

template<typename TEnd>
template<typename TVal>
bool ZC_Tuple<TEnd>::Set(unsigned int index, TVal _val)
{
    if (index == 0)
    {
        if constexpr (std::convertible_to<TVal, TEnd>)
        {
            val = static_cast<TEnd>(_val);
            return true;
        }
        else assert(false);     //  wrong index or not convertable type
    }
    assert(false);     //  index out or range
    return false;
}