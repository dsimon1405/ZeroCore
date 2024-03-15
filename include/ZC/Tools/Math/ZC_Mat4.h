#pragma once

#include "ZC_Vec4.h"
#include "ZC_Vec.h"

/*
Matrix 4x4 for working with vectors. Order of operations: Scale->Rotate->Translate
(methods must be called in the reverse order to correctly compile the matrix).

in method
{
    Translate (to the world positon)

    Translate (to the starting position)
    Rotate
    Scale
    Translate (sentralize object)
}
*/
template<typename TValue = float>
struct ZC_Mat4
{
    constexpr ZC_Mat4() = default;
    constexpr ZC_Mat4(TValue value) noexcept;
    constexpr ZC_Mat4(const ZC_Vec4<TValue>& v1, const ZC_Vec4<TValue>& v2, const ZC_Vec4<TValue>& v3, const ZC_Vec4<TValue>& v4) noexcept;

    ZC_Vec4<TValue>& operator [] (int index);
    constexpr const ZC_Vec4<TValue>& operator [] (int index) const;

    ZC_Mat4<TValue>& operator *= (TValue value) noexcept;
    constexpr ZC_Mat4<TValue> operator * (TValue value) const noexcept;
    constexpr ZC_Vec4<TValue> operator * (const ZC_Vec4<TValue>& vec) const noexcept;
    ZC_Mat4<TValue>& operator *= (const ZC_Mat4& mat) noexcept;
    constexpr ZC_Mat4<TValue> operator * (const ZC_Mat4& mat) const noexcept;

    /*
    Converts a matrix for vector axises scaling.

    Params:
    axises - scale axises.

    Return:
    Scaling matrix. 
    */
    ZC_Mat4<TValue>& Scale(const ZC_Vec3<TValue>& axises) noexcept;

    /*
    Converts a matrix for vector axises scaling.

    Params:
    x - scale along axis X.
    Y - scale along axis Y.
    Z - scale along axis Z.

    Return:
    Scaling matrix. 
    */
    ZC_Mat4<TValue>& Scale(TValue x, TValue y, TValue z) noexcept;

    /*
    Converts a matrix for rotation along vector axes.

    Params:
    angle - the angle of rotation in degrees.
    axiss - vector with axes of rotation фтв degree of rotation for each.

    Return:
    Rotation matrix.
    */
    ZC_Mat4<TValue>& Rotate(TValue angle, const ZC_Vec3<TValue>& axiss);

    /*
    Transform the matrix for vector translation.

    Params:
    axises - translate along axises.

    Return:
    Translation matrix.
    */
    ZC_Mat4<TValue>& Translate(const ZC_Vec3<TValue>& axises) noexcept;

    /*
    Transform the matrix for vector translation.

    Params:
    x - translate along axis X.
    Y - translate along axis Y.
    Z - translate along axis Z.

    Return:
    Translation matrix.
    */
    ZC_Mat4<TValue>& Translate(TValue x, TValue y, TValue z) noexcept;

    /*
    Returns a pointer to the first element of the matrix.
    */
    TValue* Begin() noexcept;

    /*
    Returns a const pointer to the first element of the matrix.
    */
    const TValue* Begin() const noexcept;

private:
    ZC_Vec4<TValue> columns[4];
};

template<typename TValue>
constexpr ZC_Mat4<TValue>::ZC_Mat4(TValue value) noexcept
    : columns
    {
        {static_cast<TValue>(value), 0, 0, 0},
        {0, static_cast<TValue>(value), 0, 0},
        {0, 0, static_cast<TValue>(value), 0},
        {0, 0, 0, static_cast<TValue>(value)}
    }
{}

template<typename TValue>
constexpr ZC_Mat4<TValue>::ZC_Mat4(const ZC_Vec4<TValue>& v1, const ZC_Vec4<TValue>& v2, const ZC_Vec4<TValue>& v3, const ZC_Vec4<TValue>& v4) noexcept
    : columns{v1,v2,v3,v4}
{}

template<typename TValue>
ZC_Vec4<TValue>& ZC_Mat4<TValue>::operator [] (int index)
{
    return columns[index];
}

template<typename TValue>
constexpr const ZC_Vec4<TValue>& ZC_Mat4<TValue>::operator [] (int index) const
{
    return const_cast<const ZC_Vec4<TValue>&>(columns[index]);
}

template<typename TValue>
ZC_Mat4<TValue>& ZC_Mat4<TValue>::operator *= (TValue value) noexcept
{
    for (short i = 0; i < 4; ++i) columns[i] *= value;
    return *this;
}

template<typename TValue>
constexpr ZC_Mat4<TValue> ZC_Mat4<TValue>::operator * (TValue value) const noexcept
{
    return ZC_Mat4<TValue>(columns[0] * value, columns[1] * value, columns[2] * value, columns[3] * value);
}

template<typename TValue>
constexpr ZC_Vec4<TValue> ZC_Mat4<TValue>::operator * (const ZC_Vec4<TValue>& vec) const noexcept
{
    auto getValue = [this, &vec](int i)
    { return columns[0][i] * vec[0] + columns[1][i] * vec[1] + columns[2][i] * vec[2] + columns[3][i] * vec[3]; };
    return { getValue(0), getValue(1), getValue(2), getValue(3) };
}

template<typename TValue>
ZC_Mat4<TValue>& ZC_Mat4<TValue>::operator *= (const ZC_Mat4<TValue>& mat) noexcept
{
    auto temp = *this;
    columns[0] = temp * mat.columns[0];
    columns[1] = temp * mat.columns[1];
    columns[2] = temp * mat.columns[2];
    columns[3] = temp * mat.columns[3];
    return *this;
}

template<typename TValue>
constexpr ZC_Mat4<TValue> ZC_Mat4<TValue>::operator * (const ZC_Mat4<TValue>& mat) const noexcept
{
    return { *this * mat.columns[0], *this * mat.columns[1], *this * mat.columns[2], *this * mat.columns[3] };
}

template<typename TValue>
ZC_Mat4<TValue>& ZC_Mat4<TValue>::Scale(const ZC_Vec3<TValue>& axises) noexcept
{
    columns[0] *= axises[0];
    columns[1] *= axises[1];
    columns[2] *= axises[2];
    return *this;
}

template<typename TValue>
ZC_Mat4<TValue>& ZC_Mat4<TValue>::Scale(TValue x, TValue y, TValue z) noexcept
{
    columns[0] *= x;
    columns[1] *= y;
    columns[2] *= z;
    return *this;
}

template<typename TValue>
ZC_Mat4<TValue>& ZC_Mat4<TValue>::Rotate(TValue angle, const ZC_Vec3<TValue>& axises)
{
    const TValue radAngle = ZC_Vec::Radians(angle);
    const TValue c = cos(radAngle);
    const TValue s = sin(radAngle);

    ZC_Vec3<TValue> axis(ZC_Vec::Normalize(axises));
    ZC_Vec3<TValue> temp(axis * (static_cast<TValue>(1) - c));

    ZC_Vec3<TValue> rotateX;
    rotateX[0] = c + temp[0] * axis[0];
    rotateX[1] = temp[0] * axis[1] + s * axis[2];
    rotateX[2] = temp[0] * axis[2] - s * axis[1];
    ZC_Vec4<TValue> columnX = columns[0] * rotateX[0] + columns[1] * rotateX[1] + columns[2] * rotateX[2];

    ZC_Vec3<TValue> rotateY;
    rotateY[0] = temp[1] * axis[0] - s * axis[2];
    rotateY[1] = c + temp[1] * axis[1];
    rotateY[2] = temp[1] * axis[2] + s * axis[0];
    ZC_Vec4<TValue> columnY = columns[0] * rotateY[0] + columns[1] * rotateY[1] + columns[2] * rotateY[2];

    ZC_Vec3<TValue> rotateZ;
    rotateZ[0] = temp[2] * axis[0] + s * axis[1];
    rotateZ[1] = temp[2] * axis[1] - s * axis[0];
    rotateZ[2] = c + temp[2] * axis[2];
    ZC_Vec4<TValue> columnZ = columns[0] * rotateZ[0] + columns[1] * rotateZ[1] + columns[2] * rotateZ[2];

    columns[0] = columnX;
    columns[1] = columnY;
    columns[2] = columnZ;
    return *this;
}

template<typename TValue>
ZC_Mat4<TValue>& ZC_Mat4<TValue>::Translate(const ZC_Vec3<TValue>& axises) noexcept
{
    columns[3] = (columns[0] * axises[0]) + (columns[1] * axises[1]) + (columns[2] * axises[2]) + columns[3];
    return *this;
}

template<typename TValue>
ZC_Mat4<TValue>& ZC_Mat4<TValue>::Translate(TValue x, TValue y, TValue z) noexcept
{
    columns[3] = (columns[0] * x) + (columns[1] * y) + (columns[2] * z) + columns[3];
    return *this;
}

template<typename TValue>
TValue* ZC_Mat4<TValue>::Begin() noexcept
{
    return &columns[0][0];
}

template<typename TValue>
const TValue* ZC_Mat4<TValue>::Begin() const noexcept
{
    return &columns[0][0];
}