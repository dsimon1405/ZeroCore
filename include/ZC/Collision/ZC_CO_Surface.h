#pragma once

#include <ZC/Tools/Math/ZC_Vec3.h>

#include <vector>
#include <concepts>

template <typename T>
concept ZC_cCO_Point = std::same_as<T, ZC_Vec3<float>> || std::same_as<T, ZC_Vec3<float>*>;

template <ZC_cCO_Point TPoint>
struct ZC_CO_Surface
{
    std::vector<TPoint> points;   //  planes points. When user creates, is just a ZC_Vec3<float>, but in ZC_CO_Figure::surfaces is pointes to points in ZC_CO_Figure::all_points (to avoid multiplying identical points with the model matrix)
    TPoint normal;  //  normal of plane (normalized)

    /*
    Params:
    - _points - surface points.
    - _normal - surface normal.
    */
    ZC_CO_Surface(std::vector<TPoint>&& _points, TPoint _normal)
        : points(std::move(_points)),
        normal(_normal)
    {}
};