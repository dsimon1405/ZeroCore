#pragma once

template<typename TValue = float>
struct ZC_Vec2
{
    constexpr ZC_Vec2() noexcept;
    constexpr ZC_Vec2(TValue x, TValue y) noexcept;

    TValue& operator [] (int index);
    constexpr const TValue& operator [] (int index) const;

    ZC_Vec2<TValue>& operator += (TValue addable) noexcept;
    constexpr ZC_Vec2<TValue> operator + (TValue addable) const noexcept;
    ZC_Vec2<TValue>& operator += (const ZC_Vec2<TValue>& addable) noexcept;
    constexpr ZC_Vec2<TValue> operator + (const ZC_Vec2<TValue>& addable) const noexcept;

    ZC_Vec2<TValue>& operator -= (TValue subtrahend) noexcept;
    constexpr ZC_Vec2<TValue> operator - (TValue subtrahend) const noexcept;
    ZC_Vec2<TValue>& operator -= (const ZC_Vec2<TValue>& subtrahend) noexcept;
    constexpr ZC_Vec2<TValue> operator - (const ZC_Vec2<TValue>& subtrahend) const noexcept;

    ZC_Vec2<TValue>& operator *= (TValue factor) noexcept;
    constexpr ZC_Vec2<TValue> operator * (TValue factor) const noexcept;

    ZC_Vec2<TValue>& operator /= (TValue divisor) noexcept;
    constexpr ZC_Vec2<TValue> operator / (TValue divisor) const noexcept;

    constexpr bool operator == (const ZC_Vec2<TValue>& vec) const noexcept;

    TValue* Begin() noexcept;
    const TValue* Begin() const noexcept;

private:
    TValue values[2];
};

template<typename TValue>
constexpr ZC_Vec2<TValue>::ZC_Vec2() noexcept
    :values{0, 0}
{}

template<typename TValue>
constexpr ZC_Vec2<TValue>::ZC_Vec2(TValue x, TValue y) noexcept
    :values{x, y}
{}

template<typename TValue>
TValue& ZC_Vec2<TValue>::operator [] (int index)
{
    return values[index];
}

template<typename TValue>
constexpr const TValue& ZC_Vec2<TValue>::operator [] (int index) const
{
    return const_cast<const TValue&>(values[index]);
}

template<typename TValue>
ZC_Vec2<TValue>& ZC_Vec2<TValue>::operator += (TValue addable) noexcept
{
    for (short i = 0; i < 2; ++i) values[i] += addable;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec2<TValue> ZC_Vec2<TValue>::operator + (TValue addable) const noexcept
{
    return ZC_Vec2<TValue>(values[0] + addable, values[1] + addable);
}

template<typename TValue>
ZC_Vec2<TValue>& ZC_Vec2<TValue>::operator += (const ZC_Vec2<TValue>& addable) noexcept
{
    for (short i = 0; i < 2; ++i) values[i] += addable.values[i];
    return *this;
}

template<typename TValue>
constexpr ZC_Vec2<TValue> ZC_Vec2<TValue>::operator + (const ZC_Vec2<TValue>& addable) const noexcept
{
    return ZC_Vec2<TValue>(values[0] + addable.values[0], values[1] + addable.values[1]);
}

template<typename TValue>
ZC_Vec2<TValue>& ZC_Vec2<TValue>::operator -= (TValue subtrahend) noexcept
{
    for (short i = 0; i < 2; ++i) values[i] -= subtrahend;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec2<TValue> ZC_Vec2<TValue>::operator - (TValue subtrahend) const noexcept
{
    return ZC_Vec2<TValue>(values[0] - subtrahend, values[1] - subtrahend);
}

template<typename TValue>
ZC_Vec2<TValue>& ZC_Vec2<TValue>::operator -= (const ZC_Vec2<TValue>& subtrahend) noexcept
{
    for (short i = 0; i < 2; ++i) values[i] -= subtrahend.values[i];
    return *this;
}

template<typename TValue>
constexpr ZC_Vec2<TValue> ZC_Vec2<TValue>::operator - (const ZC_Vec2<TValue>& subtrahend) const noexcept
{
    return ZC_Vec2<TValue>(values[0] - subtrahend.values[0], values[1] - subtrahend.values[1]);
}

template<typename TValue>
ZC_Vec2<TValue>& ZC_Vec2<TValue>::operator *= (TValue factor) noexcept
{
    for (short i = 0; i < 2; ++i) values[i] *= factor;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec2<TValue> ZC_Vec2<TValue>::operator * (TValue factor) const noexcept
{
    return ZC_Vec2<TValue>(values[0] * factor, values[1] * factor);
}

template<typename TValue>
ZC_Vec2<TValue>& ZC_Vec2<TValue>::operator /= (TValue factor) noexcept
{
    for (short i = 0; i < 2; ++i) values[i] /= factor;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec2<TValue> ZC_Vec2<TValue>::operator / (TValue factor) const noexcept
{
    return ZC_Vec4<TValue>(values[0] / factor, values[1] / factor);
}

template<typename TValue>
constexpr bool ZC_Vec2<TValue>::operator == (const ZC_Vec2<TValue>& vec) const noexcept
{
    return values[0] == vec.values[0] && values[1] == vec.values[1];
}

template<typename TValue>
TValue* ZC_Vec2<TValue>::Begin() noexcept
{
    return values;
}

template<typename TValue>
const TValue* ZC_Vec2<TValue>::Begin() const noexcept
{
    return values;
}