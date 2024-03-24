#pragma once

#include "ZC_Vec3.h"

template<typename TValue = float>
struct ZC_Vec4
{
    constexpr ZC_Vec4() noexcept;
    constexpr ZC_Vec4(TValue x, TValue y, TValue z, TValue w) noexcept;
    constexpr ZC_Vec4(const ZC_Vec3<TValue>& vec3, TValue w = 0) noexcept;

    TValue& operator [] (int index);
    constexpr const TValue& operator [] (int index) const;

    ZC_Vec4<TValue>& operator += (TValue addable) noexcept;
    constexpr ZC_Vec4<TValue> operator + (TValue addable) const noexcept;
    ZC_Vec4<TValue>& operator += (const ZC_Vec4<TValue>& addable) noexcept;
    constexpr ZC_Vec4<TValue> operator + (const ZC_Vec4<TValue>& addable) const noexcept;

    ZC_Vec4<TValue>& operator -= (TValue subtrahend) noexcept;
    constexpr ZC_Vec4<TValue> operator - (TValue subtrahend) const noexcept;
    ZC_Vec4<TValue>& operator -= (const ZC_Vec4<TValue>& subtrahend) noexcept;
    constexpr ZC_Vec4<TValue> operator - (const ZC_Vec4<TValue>& subtrahend) const noexcept;

    ZC_Vec4<TValue>& operator *= (TValue factor) noexcept;
    constexpr ZC_Vec4<TValue> operator * (TValue factor) const noexcept;

    TValue* Begin() noexcept;
    const TValue* Begin() const noexcept;

private:
    TValue values[4];
};

template<typename TValue>
constexpr ZC_Vec4<TValue>::ZC_Vec4() noexcept
    :values{0, 0, 0, 0}
{}

template<typename TValue>
constexpr ZC_Vec4<TValue>::ZC_Vec4(TValue x, TValue y, TValue z, TValue w) noexcept
    :values{x, y, z, w}
{}

template<typename TValue>
constexpr ZC_Vec4<TValue>::ZC_Vec4(const ZC_Vec3<TValue>& vec3, TValue w) noexcept
    :values{vec3[0], vec3[1], vec3[2], w}
{}

template<typename TValue>
TValue& ZC_Vec4<TValue>::operator [] (int index)
{
    return values[index];
}

template<typename TValue>
constexpr const TValue& ZC_Vec4<TValue>::operator [] (int index) const
{
    return const_cast<const TValue&>(values[index]);
}

template<typename TValue>
ZC_Vec4<TValue>& ZC_Vec4<TValue>::operator += (TValue addable) noexcept
{
    for (short i = 0; i < 4; ++i) values[i] += addable;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec4<TValue> ZC_Vec4<TValue>::operator + (TValue addable) const noexcept
{
    return ZC_Vec4<TValue>(values[0] + addable, values[1] + addable, values[2] + addable, values[3] + addable);
}

template<typename TValue>
ZC_Vec4<TValue>& ZC_Vec4<TValue>::operator += (const ZC_Vec4<TValue>& addable) noexcept
{
    for (short i = 0; i < 4; ++i) values[i] += addable.values[i];
    return *this;
}

template<typename TValue>
constexpr ZC_Vec4<TValue> ZC_Vec4<TValue>::operator + (const ZC_Vec4<TValue>& addable) const noexcept
{
    return ZC_Vec4<TValue>(values[0] + addable.values[0], values[1] + addable.values[1], values[2] + addable.values[2], values[3] + addable.values[3]);
}

template<typename TValue>
ZC_Vec4<TValue>& ZC_Vec4<TValue>::operator -= (TValue subtrahend) noexcept
{
    for (short i = 0; i < 4; ++i) values[i] -= subtrahend;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec4<TValue> ZC_Vec4<TValue>::operator - (TValue subtrahend) const noexcept
{
    return ZC_Vec4<TValue>(values[0] - subtrahend, values[1] - subtrahend, values[2] - subtrahend, values[3] - subtrahend);
}

template<typename TValue>
ZC_Vec4<TValue>& ZC_Vec4<TValue>::operator -= (const ZC_Vec4<TValue>& subtrahend) noexcept
{
    for (short i = 0; i < 4; ++i) values[i] -= subtrahend.values[i];
    return *this;
}

template<typename TValue>
constexpr ZC_Vec4<TValue> ZC_Vec4<TValue>::operator - (const ZC_Vec4<TValue>& subtrahend) const noexcept
{
    return ZC_Vec4<TValue>(values[0] - subtrahend.values[0], values[1] - subtrahend.values[1], values[2] - subtrahend.values[2], values[3] - subtrahend.values[3]);
}

template<typename TValue>
ZC_Vec4<TValue>& ZC_Vec4<TValue>::operator *= (TValue factor) noexcept
{
    for (short i = 0; i < 4; ++i) values[i] *= factor;
    return *this;
}

template<typename TValue>
constexpr ZC_Vec4<TValue> ZC_Vec4<TValue>::operator * (TValue factor) const noexcept
{
    return ZC_Vec4<TValue>(values[0] * factor, values[1] * factor, values[2] * factor, values[3] * factor);
}

template<typename TValue>
TValue* ZC_Vec4<TValue>::Begin() noexcept
{
    return values;
}

template<typename TValue>
const TValue* ZC_Vec4<TValue>::Begin() const noexcept
{
    return values;
}