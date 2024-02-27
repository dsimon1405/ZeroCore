#pragma once

#include <ZC/Tools/Math/ZC_Vec3.h>

struct ZC_Quad
{
    ZC_Vec3<float> bl, tr, tl, br;

    constexpr ZC_Quad() = default;
    constexpr ZC_Quad(const ZC_Vec3<float>& _bl, const ZC_Vec3<float>& _tr, const ZC_Vec3<float>& _tl, const ZC_Vec3<float>& _br)
        : bl(_bl),
        tr(_tr),
        tl(_tl),
        br(_br)
    {}

    constexpr ZC_Vec3<float> Center() const noexcept
    {
        return (bl + tr + tl + br) / 4.f;
    }
};

struct ZC_Triangle
{
    ZC_Vec3<float> bl, tc, br;

    constexpr ZC_Triangle() = default;
    constexpr ZC_Triangle(const ZC_Vec3<float>& _bl, const ZC_Vec3<float> _tc, const ZC_Vec3<float>& _br)
        : bl(_bl),
        tc(_tc),
        br(_br)
    {}

    constexpr ZC_Vec3<float> Center() const noexcept
    {
        return (bl + tc + br) / 3.f;
    }
};