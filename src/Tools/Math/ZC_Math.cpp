#include <ZC/Tools/Math/ZC_Math.h>

uint ZC_PackColorFloatToUInt_RGB(float r, float g, float b) noexcept
{
    if (r < 0.f || r > 1.f || g < 0.f || g > 1.f || b < 0.f || b > 1.f) return 0;
    return ((uint)(r * 255.f) << 10 | (uint)(g * 255.f)) << 10 | (uint)(b * 255.f);
}

uint ZC_PackColorFloatToUInt_RGBA(float r, float g, float b, float a) noexcept
{
    if (r < 0.f || r > 1.f || g < 0.f || g > 1.f || b < 0.f || b > 1.f || a < 0.f || a > 1.f) return 0;
    return (((uint)(r * 255.f) << 8 | (uint)(g * 255.f)) << 8 | (uint)(b * 255.f)) << 8 | (uint)(a * 255.f);
}

uint ZC_PackColorUCharToUInt_RGB(uchar r, uchar g, uchar b) noexcept
{
    return (static_cast<uint>(r) << 10 | static_cast<uint>(g)) << 10 | static_cast<uint>(b);
}

uint ZC_PackColorUcharToUInt_RGBA(uchar r, uchar g, uchar b, uchar a) noexcept
{
    return (((static_cast<uint>(r) << 8 | static_cast<uint>(g)) << 8 | static_cast<uint>(b))) << 8 | static_cast<uint>(a);
}

ushort ZC_PackTexCoordFloatToUShort(float coord)
{
    return static_cast<ushort>(coord * USHRT_MAX);
}

int ZC_Pack_INT_2_10_10_10_REV(float x, float y, float z)
{
    //  pack float in signed byte array[10]:
    //  array[0] - sign (0 is pluss, 1 is minus);
    //  array[1 - 9] - number;
    //  512(min), 511(max) signed byte[9] values.
    auto packIn10Bytes = [](float val) -> int
    {
        return  val < 0 ?
        512 | static_cast<int>(ZC_ROUND(512.f + val * 512.f))
        : static_cast<int>(ZC_ROUND(val * 511.f));
    };
    return ((packIn10Bytes(z) << 20) | (packIn10Bytes(y) << 10)) | packIn10Bytes(x);
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