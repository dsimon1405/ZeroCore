#pragma once

#include "ZC_Mat4.h"

/*
Namespace of orientation matrices in space. By default, the coordinate system is LEFT, to change to RIGHT,
you need to uncomment ZC_RIGHT_COORDINATE_SYSTEM and comment out ZC_LEFT_COORDINATE_SYSTEM in this file.
*/
namespace ZC_Mat
{
    #define ZC_LEFT_COORDINATE_SYSTEM
//     #define ZC_RIGHT_COORDINATE_SYSTEM

    /*
    Create a perspertive projection matrix (frustum).

    Params:
    fovy - viewing angle in degrees.
    aspect - window width to height ratio.
    near - length to the top of the frustum.
    far - length to base of frustum.

    Return:
    Perspective matrix.
    */
    template<ZC_cVecTypes TValue>
    ZC_Mat4<TValue> Perspective(TValue fovy, TValue aspect, TValue nearPlane, TValue farPlane)
    {
        ZC_Mat4<TValue> result;

        const TValue tanHalfFovy = tan(ZC_Vec::Radians(fovy) / static_cast<TValue>(2));
        result[0][0] = static_cast<TValue>(1) / (aspect * tanHalfFovy);
        result[1][1] = static_cast<TValue>(1) / tanHalfFovy;

        const TValue farMinusNear = farPlane - nearPlane;
#ifdef ZC_RIGHT_COORDINATE_SYSTEM
        result[2][2] = (farPlane + nearPlane) / farMinusNear;
        result[2][3] = static_cast<TValue>(1);
#elif defined ZC_LEFT_COORDINATE_SYSTEM
        result[2][2] = - (farPlane + nearPlane) / farMinusNear;
        result[2][3] = - static_cast<TValue>(1);
#endif
        result[3][2] = - (static_cast<TValue>(2) * farPlane * nearPlane) / farMinusNear;
        return result;
    }

    /*
    Create view matrix.

    Params:
    position - camera location coordinates (your eye).
    lookOn - coordinates where the camera is looking (the place where your eye looks).
    up - a normalized direction pointing to the top. Typically, it is assumed to be (0,0,1) in world space.
    
    Return:
    View matrix.
    */
    template<ZC_cVecTypes TValue>
    ZC_Mat4<TValue> LookAt(const ZC_Vec3<TValue>& position, const ZC_Vec3<TValue>& lookOn, const ZC_Vec3<TValue>& up)
    {
        ZC_Vec3<TValue> frontDir(ZC_Vec::Normalize(lookOn - position));
#ifdef ZC_RIGHT_COORDINATE_SYSTEM
        ZC_Vec3<TValue> rightDir(ZC_Vec::Normalize(ZC_Vec::Cross(up, frontDir)));
        ZC_Vec3<TValue> upDir(ZC_Vec::Cross(frontDir, rightDir));
#elif defined ZC_LEFT_COORDINATE_SYSTEM
        ZC_Vec3<TValue> rightDir(ZC_Vec::Normalize(ZC_Vec::Cross(frontDir, up)));
        ZC_Vec3<TValue> upDir(ZC_Vec::Cross(rightDir, frontDir));
#endif
        ZC_Mat4<TValue> result(static_cast<TValue>(1));
        result[0][0] = rightDir[0];
        result[1][0] = rightDir[1];
        result[2][0] = rightDir[2];
        result[0][1] = upDir[0];
        result[1][1] = upDir[1];
        result[2][1] = upDir[2];
#ifdef ZC_RIGHT_COORDINATE_SYSTEM
        result[0][2] = frontDir[0];
        result[1][2] = frontDir[1];
        result[2][2] = frontDir[2];
        result[3][2] = -ZC_Vec::Dot(frontDir, position);
#elif defined ZC_LEFT_COORDINATE_SYSTEM
        result[0][2] = -frontDir[0];
        result[1][2] = -frontDir[1];
        result[2][2] = -frontDir[2];
        result[3][2] = ZC_Vec::Dot(frontDir, position);
#endif
        result[3][0] = -ZC_Vec::Dot(rightDir, position);
        result[3][1] = -ZC_Vec::Dot(upDir, position);
        return result;
    }

    template<ZC_cVecTypes T>
    ZC_Mat4<T> Ortho(T left, T right, T bottom, T top) noexcept
    {
        return ZC_Mat4<T>({ static_cast<T>(2) / (right - left), 0, 0, 0 },
                          { 0, static_cast<T>(2) / (top - bottom), 0, 0 },
                          { 0, 0, -static_cast<T>(1), 0 },
                          { -(right + left) / (right - left), -(top + bottom) / (top - bottom), 0, static_cast<T>(1) });
    }
}