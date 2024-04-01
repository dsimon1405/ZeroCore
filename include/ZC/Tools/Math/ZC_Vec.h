#pragma once

#include "ZC_Vec3.h"

#include <cmath>

#define ZC_PI       3.14159265358979323846   // pi
#define ZC_PI_RAD_COEF (ZC_PI / 180.0)

namespace ZC_Vec
{
    /*
    Normalize the vector - make its length = 1, preserving the direction.

    Params:
    vec - vector for normalization.

    Return:
    Normalized vector;
    */
    template<typename TValue>
    ZC_Vec3<TValue> Normalize(const ZC_Vec3<TValue>& vec)
    {
        return vec * static_cast<TValue>(1.0 / sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]));
    }

    /*
    Creates a vector orthogonal (90 degrees) to two others. If you look from vec2 to vec1, the result will be on the right.

    Params:
    vec1 - first vector for creation orthogonal vector.
    vec2 - second vector for creation orthogonal vector.

    Return:
    Orthogonal vector.
    */
    template<typename TValue>
    constexpr ZC_Vec3<TValue> Cross(const ZC_Vec3<TValue>& vec1, const ZC_Vec3<TValue>& vec2) noexcept
    {
        return { (vec1[1] * vec2[2]) - (vec1[2] * vec2[1]), (vec1[2] * vec2[0]) - (vec1[0] * vec2[2]), (vec1[0] * vec2[1]) - (vec1[1] * vec2[0]) };
    }

    /*
    Convert degrees to radians.

    Params:
    angle - angle in degrees.

    Return:
    Angle in radians.
    */
    template<typename TValue>
    constexpr TValue Radians(TValue angle) noexcept
    {
        return angle * static_cast<TValue>(ZC_PI_RAD_COEF);
    }

    /*
    Scalar product of two vectors (if both normalized - cosine of the angle between them).
    If 1 then the vectors are aligned (cos0 = 1).
    If 0 then the vectors are orthogonal (cos90 = 0).
    If -1 then they are opposite (cos180 = -1).
    Then cos270 = 0 and return to cos360 = 1 = cos0.

    Params:
    vec1 - first vector.
    vec2 - second vector.

    Return:
    Cosine of the angle between vec1 and vec2.
    */
    template<typename TValue>
    constexpr TValue Dot(const ZC_Vec3<TValue>& vec1, const ZC_Vec3<TValue>& vec2) noexcept
    {
        return (vec1[0] * vec2[0]) + (vec1[1] * vec2[1]) + (vec1[2] * vec2[2]);
    }

    /*
    Vector length.

    Params:
    vec1 - vector.

    Return:
    Vector length.
    */
    template<typename TValue>
    constexpr TValue Length(const ZC_Vec3<TValue>& vec1) noexcept
    {
        return static_cast<TValue>(sqrt(vec1[0] * vec1[0] + vec1[1] * vec1[1] + vec1[2] * vec1[2]));
    }

    /*
    Moves a vector in a specified direction (may not be normalized) by a specified length.

    Params:
    v - vector to move.
    direction - where to move.
    length - on what length to move.

    Return:
    Vector with new position.
    */
    template<typename TValue>
    ZC_Vec3<TValue> MoveByLength(const ZC_Vec3<TValue>& v, const ZC_Vec3<TValue>& direction, TValue length) noexcept
    {
        return v + (direction * static_cast<TValue>(length / sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2])));
    }
}