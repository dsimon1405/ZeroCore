#pragma once

#include <ZC/Tools/Math/ZC_Vec3.h>

struct ZC_Quad
{
    ZC_Vec3<float> bl;
    ZC_Vec3<float> tr;
    ZC_Vec3<float> tl;
    ZC_Vec3<float> br;

    ZC_Quad() = default;
    ZC_Quad(const ZC_Vec3<float>& _bl, const ZC_Vec3<float>& _tr, const ZC_Vec3<float>& _tl, const ZC_Vec3<float>& _br)
        : bl(_bl),
        tr(_tr),
        tl(_tl),
        br(_br)
    {}

    ZC_Vec3<float> Center() const noexcept
    {
        return (bl + tr + tl + br) / 4.f;
    }
};