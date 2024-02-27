#pragma once

#include <concepts>

template<typename TValue = float>
struct ZC_Vec3
{
    constexpr ZC_Vec3() noexcept;
    constexpr ZC_Vec3(TValue x, TValue y, TValue z) noexcept;

    TValue& operator [] (int index);
    constexpr const TValue& operator [] (int index) const;

    ZC_Vec3<TValue>& operator += (TValue addable) noexcept;
    constexpr ZC_Vec3<TValue> operator + (TValue addable) const noexcept;
    ZC_Vec3<TValue>& operator += (const ZC_Vec3<TValue>& addable) noexcept;
    constexpr ZC_Vec3<TValue> operator + (const ZC_Vec3<TValue>& addable) const noexcept;

    ZC_Vec3<TValue>& operator -= (TValue subtrahend) noexcept;
    constexpr ZC_Vec3<TValue> operator - (TValue subtrahend) const noexcept;
    ZC_Vec3<TValue>& operator -= (const ZC_Vec3<TValue>& subtrahend) noexcept;
    constexpr ZC_Vec3<TValue> operator - (const ZC_Vec3<TValue>& subtrahend) const noexcept;

    ZC_Vec3<TValue>& operator *= (TValue factor) noexcept;
    constexpr ZC_Vec3<TValue> operator * (TValue factor) const noexcept;

    ZC_Vec3<TValue>& operator /= (TValue divisor) noexcept;
    constexpr ZC_Vec3<TValue> operator / (TValue divisor) const noexcept;

    constexpr bool operator == (const ZC_Vec3<TValue> vec) const noexcept;
    constexpr bool operator < (const ZC_Vec3<TValue> vec) const noexcept;

private:
    TValue values[3];
};

template<typename TValue>
constexpr ZC_Vec3<TValue>::ZC_Vec3() noexcept
    : values{0,0,0}
{}

template<typename TValue>
constexpr ZC_Vec3<TValue>::ZC_Vec3(TValue x, TValue y, TValue z) noexcept
    : values{x, y ,z}
{}

template<typename TValue>
TValue& ZC_Vec3<TValue>::operator [] (int index)
{
    return values[index];
}

template<typename TValue>
constexpr const TValue& ZC_Vec3<TValue>::operator [] (int index) const
{
    return const_cast<const TValue&>(values[index]);
}

template<typename TValue>
ZC_Vec3<TValue>& ZC_Vec3<TValue>::operator += (TValue addable) noexcept
{
    for (short i = 0; i < 3; ++i) values[i] += addable;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec3<TValue> ZC_Vec3<TValue>::operator + (TValue addable) const noexcept
{
    return ZC_Vec3<TValue>(values[0] + addable, values[1] + addable, values[2] + addable);
}

template<typename TValue>
ZC_Vec3<TValue>& ZC_Vec3<TValue>::operator += (const ZC_Vec3<TValue>& addable) noexcept
{
    for (short i = 0; i < 3; ++i) values[i] += addable.values[i];
    return *this;
}

template<typename TValue>
constexpr ZC_Vec3<TValue> ZC_Vec3<TValue>::operator + (const ZC_Vec3<TValue>& addable) const noexcept
{
    return ZC_Vec3<TValue>(values[0] + addable.values[0], values[1] + addable.values[1], values[2] + addable.values[2]);
}

template<typename TValue>
ZC_Vec3<TValue>& ZC_Vec3<TValue>::operator -= (TValue subtrahend) noexcept
{
    for (short i = 0; i < 3; ++i) values[i] -= subtrahend;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec3<TValue> ZC_Vec3<TValue>::operator - (TValue subtrahend) const noexcept
{
    return ZC_Vec3<TValue>(values[0] - subtrahend, values[1] - subtrahend, values[2] - subtrahend);
}

template<typename TValue>
ZC_Vec3<TValue>& ZC_Vec3<TValue>::operator -= (const ZC_Vec3<TValue>& subtrahend) noexcept
{
    for (short i = 0; i < 3; ++i) values[i] -= subtrahend.values[i];
    return *this;
}

template<typename TValue>
constexpr ZC_Vec3<TValue> ZC_Vec3<TValue>::operator - (const ZC_Vec3<TValue>& subtrahend) const noexcept
{
    return ZC_Vec3<TValue>(values[0] - subtrahend.values[0], values[1] - subtrahend.values[1], values[2] - subtrahend.values[2]);
}

template<typename TValue>
ZC_Vec3<TValue>& ZC_Vec3<TValue>::operator *= (TValue factor) noexcept
{
    for (short i = 0; i < 3; ++i) values[i] *= factor;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec3<TValue> ZC_Vec3<TValue>::operator * (TValue factor) const noexcept
{
    return ZC_Vec3<TValue>(values[0] * factor, values[1] * factor, values[2] * factor);
}

template<typename TValue>
ZC_Vec3<TValue>& ZC_Vec3<TValue>::operator /= (TValue divisor) noexcept
{
    for (short i = 0; i < 3; ++i) values[i] /= divisor;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec3<TValue> ZC_Vec3<TValue>::operator / (TValue divisor) const noexcept
{
    return ZC_Vec3<TValue>(values[0] / divisor, values[1] / divisor, values[2] / divisor);
}

template<typename TValue>
constexpr bool ZC_Vec3<TValue>::operator == (const ZC_Vec3<TValue> vec) const noexcept
{
    return values[0] == vec.values[0] && values[1] == vec.values[1] && values[2] == vec.values[2];
}