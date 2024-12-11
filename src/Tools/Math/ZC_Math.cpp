#include <ZC/Tools/Math/ZC_Math.h>

#include <cmath>

uint ZC_Pack_Float_To_UInt_2x10x10x10(const ZC_Vec3<float>& rgb) noexcept
{
    return ZC_Pack_Float_To_UInt_2x10x10x10(rgb[0], rgb[1], rgb[2]);
}

uint ZC_Pack_Float_To_UInt_2x10x10x10(float r, float g, float b) noexcept
{
    if (r < 0.f || r > 1.f || g < 0.f || g > 1.f || b < 0.f || b > 1.f) return 0;
    return ((uint)(r * 1023.f) << 10 | (uint)(g * 1023.f)) << 10 | (uint)(b * 1023.f);      //  1023 ( ...001111111111 )
}

uint ZC_Pack_Float_To_UInt_8x8x8x8(const ZC_Vec4<float>& v) noexcept
{
    return ZC_Pack_Float_To_UInt_8x8x8x8(v[0], v[1], v[2], v[3]);
}

uint ZC_Pack_Float_To_UInt_8x8x8x8(float r, float g, float b, float a) noexcept
{
    if (r < 0.f || r > 1.f || g < 0.f || g > 1.f || b < 0.f || b > 1.f || a < 0.f || a > 1.f) return 0;
    return (((uint)(r * 255.f) << 8 | (uint)(g * 255.f)) << 8 | (uint)(b * 255.f)) << 8 | (uint)(a * 255.f);
}

uint ZC_Pack_UChar_To_UInt_2x10x10x10(const ZC_Vec3<uch_zc>& rgb) noexcept
{
    return ZC_Pack_UChar_To_UInt_2x10x10x10(rgb[0], rgb[1], rgb[2]);
}

uint ZC_Pack_UChar_To_UInt_2x10x10x10(uchar r, uchar g, uchar b) noexcept
{
    return ((static_cast<uint>(r) * 1023u / 255u) << 10 | (static_cast<uint>(g)) * 1023u / 255u) << 10 | (static_cast<uint>(b) * 1023u / 255u);
}

uint ZC_Pack_UChar_To_UInt_8x8x8x8(const ZC_Vec4<uch_zc>& rgba) noexcept
{
    return ZC_Pack_UChar_To_UInt_8x8x8x8(rgba[0], rgba[1], rgba[2], rgba[3]);
}

uint ZC_Pack_UChar_To_UInt_8x8x8x8(uchar r, uchar g, uchar b, uchar a) noexcept
{
    return (((static_cast<uint>(r) << 8 | static_cast<uint>(g)) << 8 | static_cast<uint>(b))) << 8 | static_cast<uint>(a);
}

ZC_Vec3<f_zc> ZC_Unpack_UInt_2x10x10x10_To_Float(uint rgb) noexcept
{
    return { ((rgb >> 20) & 1023u) / 1023.f, ((rgb >> 10) & 1023u) / 1023.f, (rgb & 1023u) / 1023.f };
}

ZC_Vec3<uch_zc> ZC_Unpack_UInt_2x10x10x10_To_UChar(uint rgb) noexcept
{
    return { uch_zc(((rgb >> 20) & 1023u) * 255u / 1023u), uch_zc(((rgb >> 10) & 1023u) * 255u / 1023u), uch_zc((rgb & 1023u) * 255u / 1023u) };
}

ZC_Vec4<f_zc> ZC_Unpack_UInt_8x8x8x8_To_Float(uint rgba) noexcept
{
    return { ((rgba >> 24) & 255u) / 255.f, ((rgba >> 16) & 255u) / 255.f, ((rgba >> 8) & 255u) / 255.f, (rgba & 255u) / 255.f };
}

ZC_Vec4<uch_zc> ZC_Unpack_UInt_8x8x8x8_To_UChar(uint rgba) noexcept
{
    return { uch_zc(rgba >> 24), uch_zc(rgba >> 16), uch_zc(rgba >> 8), uch_zc(rgba & 255u) };
}

int ZC_Pack_INT_2_10_10_10_REV(float x, float y, float z, char bytes_2)
{
    //  pack float in signed byte array[10]:
    //  array[0] - sign (0 is pluss, 1 is minus);
    //  array[1 - 9] - number;
    //  512(min), 511(max) signed byte[9] values.
    auto packIn10Bytes = [](float val) -> int
    {
        return val < 0 ?
            512 | static_cast<int>(std::round((val * 511.f) + 511.f))
            : static_cast<int>(std::round(val * 511.f));
    };
    return (((bytes_2 << 30) | packIn10Bytes(z) << 20) | (packIn10Bytes(y) << 10)) | packIn10Bytes(x);
}

ushort ZC_PackTexCoordFloatToUShort(float coord)
{
    return static_cast<ushort>(coord * USHRT_MAX);
}

bool ZC_IsPointBelongTriangle_2D(const ZC_Vec2<float>& a, const ZC_Vec2<float>& b, const ZC_Vec2<float>& c, const ZC_Vec2<float>& p)
{
    float v1 = (a[0] - p[0]) * (b[1] - a[1]) - (b[0] - a[0]) * (a[1] - p[1]);
    float v2 = (b[0] - p[0]) * (c[1] - b[1]) - (c[0] - b[0]) * (b[1] - p[1]);
    float v3 = (c[0] - p[0]) * (a[1] - c[1]) - (a[0] - c[0]) * (c[1] - p[1]);
    return (v1 >= 0.f && v2 >= 0.f && v3 >= 0.f) || (v1 <= 0.f && v2 <= 0.f && v3 <= 0.f);
}

ZC_Vec3<float> ZC_CalculateTriangleBarycenters(const ZC_Vec2<float>& a, const ZC_Vec2<float>& b, const ZC_Vec2<float>& c, const ZC_Vec2<float>& p)
{
    float u = ((b[1] - c[1]) * (p[0] - c[0]) + (c[0] - b[0]) * (p[1] - c[1])) / ((b[1] - c[1]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[1] - c[1]));
    float v = ((c[1] - a[1]) * (p[0] - c[0]) + (a[0] - c[0]) * (p[1] - c[1])) / ((b[1] - c[1]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[1] - c[1]));
    float w = 1.f - u - v;
    return { u, v, w };
}

bool ZC_IsPointBelongTriangle_2D(const ZC_Vec2<float>& a, const ZC_Vec2<float>& b, const ZC_Vec2<float>& c, const ZC_Vec2<float>& p, ZC_Vec3<float>& barycenters)
{
    barycenters = ZC_CalculateTriangleBarycenters(a, b, c, p);
    return barycenters[0] >= 0.f && barycenters[0] <= 1.f && barycenters[1] >= 0.f && barycenters[1] <= 1.f && barycenters[2] >= 0.f && barycenters[2] <= 1.f;
}

bool ZC_FindLineIntercectionPoint_2D(const ZC_Vec2<float>& p1, const ZC_Vec2<float>& p2, const ZC_Vec2<float>& p3, const ZC_Vec2<float>& p4, ZC_Vec2<float>& rPoint)
{       //  https://ip76.ru/theory-and-practice/cross-lines/
    float a1 = p2[1] - p1[1];
    float a2 = p4[1] - p3[1];
    float b1 = p1[0] - p2[0];
    float b2 = p3[0] - p4[0];
    float v = a1 * b2 - a2 * b1;
    if (v == 0.f) return false;     //  lines parallel or coincident
    
    float c1 = p2[0] * p1[1] - p1[0] * p2[1];
    float c2 = p4[0] * p3[1] - p3[0] * p4[1];
    rPoint = { - (c1 * b2 - c2 * b1) / v,
            - (a1 * c2 - a2 * c1) / v };
    return true;
}

float ZC_DistanceFromPointToPlane(ZC_Vec3<float> point_in_plane, ZC_Vec3<float> normal_plane, ZC_Vec3<float> point)
{       //  https://ru.stackoverflow.com/questions/779995/%D0%A0%D0%B0%D1%81%D1%81%D1%82%D0%BE%D1%8F%D0%BD%D0%B8%D0%B5-%D0%BE%D1%82-%D1%82%D0%BE%D1%87%D0%BA%D0%B8-a-%D0%B4%D0%BE-%D0%BF%D0%BB%D0%BE%D1%81%D0%BA%D0%BE%D1%81%D1%82%D0%B8-%D0%BE%D0%BF%D1%80%D0%B5%D0%B4%D0%B5%D0%BB%D0%B5%D0%BD%D0%BD%D0%BE%D0%B9-%D0%BD%D0%BE%D1%80%D0%BC%D0%B0%D0%BB%D1%8C%D1%8E-pnormal-%D0%B8-%D1%82%D0%BE%D1%87%D0%BA%D0%BE%D0%B9
    return normal_plane[0] * (point[0] - point_in_plane[0]) + normal_plane[1] * (point[1] - point_in_plane[1]) + normal_plane[2] * (point[2] - point_in_plane[2]);
}