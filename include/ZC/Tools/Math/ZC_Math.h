#pragma once

#include "ZC_Vec2.h"
#include "ZC_Vec4.h"
#include <ZC/ZC_Types.h>

#include <climits>
// #define ZC_CHAR_BIT 	8
// #define ZC_CHAR_MIN 	-128
// #define ZC_CHAR_MAX 	127
// #define ZC_UCHAR_MAX 	255     //  (0xff)
// #define ZC_SHRT_MIN 	-32768
// #define ZC_SHRT_MAX 	32767
// #define ZC_USHRT_MAX 	65535   //  (0xffff)
// #define ZC_INT_MIN 	    -2147483648
// #define ZC_INT_MAX 	    2147483647
// #define ZC_UINT_MAX 	4294967295 //   (0xffffffff)
// #define ZC_LONG_MIN 	-2147483648
// #define ZC_LONG_MAX 	2147483647
// #define ZC_ULONG_MAX 	4294967295 //   (0xffffffff)
// #define ZC_LLONG_MIN 	-9223372036854775808
// #define ZC_LLONG_MAX 	9223372036854775807
// #define ZC_ULLONG_MAX 	18446744073709551615 // (0xffffffffffffffff)

typedef signed char schar;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;
typedef double long ldouble;

#define ZC_ToZeroOneRange(val) ((val + 1.0) / 2.0)  //  from range [-1,1] to range [0,1]
#define ZC_ToMinusPlusOneRange(val) ((val * 2.0) - 1.0)  //  from range [0,1] to range [-1,1]

constexpr int ZC_angle_0f = 0.f;
constexpr int ZC_angle_0i = 0;
constexpr float ZC_angle_90f = 90.f;
constexpr int ZC_angle_90i = 90;
constexpr float ZC_angle_180f = 180.f;
constexpr int ZC_angle_180i = 180;
constexpr float ZC_angle_270f = 270.f;
constexpr int ZC_angle_270i = 270;
constexpr float ZC_angle_360f = 360.f;
constexpr int ZC_angle_360i = 360;

/*
Packs color from 3 float channels into one uint[32] indices -> [0-1] nothing, [2-11] red, [12-21] green, [22-31] blue.

Params:
- v - colors rgb in range [0.f, 1.f].

Return:
On success packed color, otherwise 0.
*/
uint ZC_Pack_Float_To_UInt_2x10x10x10(const ZC_Vec3<float>& rgb) noexcept;

/*
Packs color from 3 float channels into one uint[32] indices -> [0-1] nothing, [2-11] red, [12-21] green, [22-31] blue.

Params:
- r - red color (range 0.f - 1.f).
- g - green color (range 0.f - 1.f).
- b - blue color (range 0.f - 1.f).

Return:
On success packed color, otherwise 0.
*/
uint ZC_Pack_Float_To_UInt_2x10x10x10(float r, float g, float b) noexcept;

/*
Packs color from 3 float channels into one uint[32] indices -> [0-7] red, [8-15] greed, [16-24] blue, [24-31] alpha.

Params:
- v - colors rgba in range [0.f, 1.f].

Return:
On success packed color, otherwise 0.
*/
uint ZC_Pack_Float_To_UInt_8x8x8x8(const ZC_Vec4<float>& v) noexcept;

/*
Packs color from 3 float channels into one uint[32] indices -> [0-7] red, [8-15] greed, [16-24] blue, [24-31] alpha.

Params:
- r - red color (range 0.f - 1.f).
- g - green color (range 0.f - 1.f).
- b - blue color (range 0.f - 1.f).
- a - alpha (range 0.f - 1.f).

Return:
On success packed color, otherwise 0.
*/
uint ZC_Pack_Float_To_UInt_8x8x8x8(float r, float g, float b, float a) noexcept;

/*
Packs color from 3 uchar channels into one uint[32] indices -> [0-1] nothing, [2-11] red, [12-21] green, [22-31] blue.

Params:
- v - colors rgb in range [0.f, 1.f].

Return:
On success packed color, otherwise 0.
*/
uint ZC_Pack_UChar_To_UInt_2x10x10x10(const ZC_Vec3<uch_zc>& rgb) noexcept;

/*
Packs color from 3 uchar channels into one uint[32] indices -> [0-1] nothing, [2-11] red, [12-21] green, [22-31] blue.

Params:
- r - red color. Range [0, 255].
- g - green color. Range [0, 255].
- b - blue color. Range [0, 255].

Return:
On success packed color, otherwise 0.
*/
uint ZC_Pack_UChar_To_UInt_2x10x10x10(uchar r, uchar g, uchar b) noexcept;

/*
Packs color from 3 uchar channels into one uint[32] indices -> [0-7] red, [8-15] greed, [16-24] blue, [24-31] alpha.

Params:
- v - colors rgba in range [0.f, 1.f].

Return:
On success packed color, otherwise 0.
*/
uint ZC_Pack_UChar_To_UInt_8x8x8x8(const ZC_Vec4<uch_zc>& rgba) noexcept;

/*
Packs color from 3 uchar channels into one uint[32] indices -> [0-7] red, [8-15] greed, [16-24] blue, [24-31] alpha.

Params:
- r - red color. Range [0, 255].
- g - green color. Range [0, 255].
- b - blue color. Range [0, 255].
- a - alpha. Range [0, 255].

Return:
On success packed color, otherwise 0.
*/
uint ZC_Pack_UChar_To_UInt_8x8x8x8(uchar r, uchar g, uchar b, uchar a) noexcept;

//  Order in unpacked vector vec(r, g, b)
ZC_Vec3<f_zc> ZC_Unpack_UInt_2x10x10x10_To_Float(uint rgb) noexcept;

//  Order in unpacked vector vec(r, g, b)
ZC_Vec3<uch_zc> ZC_Unpack_UInt_2x10x10x10_To_UChar(uint rgb) noexcept;

//  Order in unpacked vector vec(r, g, b, a)
ZC_Vec4<f_zc> ZC_Unpack_UInt_8x8x8x8_To_Float(uint rgba) noexcept;

//  Order in unpacked vector vec(r, g, b, a)
ZC_Vec4<uch_zc> ZC_Unpack_UInt_8x8x8x8_To_UChar(uint rgba) noexcept;

int ZC_Pack_INT_2_10_10_10_REV(float x, float y, float z, char bytes_2 = 0);

ushort ZC_PackTexCoordFloatToUShort(float coord);

/*
Params:
- a, b, c - triangle points coords.
- p - point to check.

Return:
true if point belong triangle, otherwise false.
*/
bool ZC_IsPointBelongTriangle_2D(const ZC_Vec2<float>& a, const ZC_Vec2<float>& b, const ZC_Vec2<float>& c, const ZC_Vec2<float>& p);

/*
Calculate barycenters of point in triangle.

Params:
- a, b, c - triangle points coords.
- p - point to calculate barycanters.

Return:
ZC_Vec3 of barycenters acoding a,b,c corners of triangle.
*/
ZC_Vec3<float> ZC_CalculateTriangleBarycenters(const ZC_Vec2<float>& a, const ZC_Vec2<float>& b, const ZC_Vec2<float>& c, const ZC_Vec2<float>& p);

/*
Calculate barycenters and check is point belong triangle.

Params:
- a, b, c - triangle points coords.
- p - point to check.
- barycenters - here will be set barycenters.

Return:
true if (p) belong triangle (a,b,c), otherwise false.
*/
bool ZC_IsPointBelongTriangle_2D(const ZC_Vec2<float>& a, const ZC_Vec2<float>& b, const ZC_Vec2<float>& c, const ZC_Vec2<float>& p, ZC_Vec3<float>& barycenters);

/*
Find intercection point of two lines in 2D.

Params:
- p1, p2 - first line coords.
- p3, p4 - second line coords.
- rPoint - place for intersection point.

Return:
true if lines intersects, otherwise false and rPoint value don't changing.
*/
bool ZC_FindLineIntercectionPoint_2D(const ZC_Vec2<float>& p1, const ZC_Vec2<float>& p2, const ZC_Vec2<float>& p3, const ZC_Vec2<float>& p4, ZC_Vec2<float>& rPoint);

/*
Finds shortest (orthogonal) distance from point to plane. If result > 0, point is in direction of normal, if result < 0 point behind the plane. In each case modulus of the result is the distance to the plane.

Params:
- point_in_plane - some point in lpane.
- normal_plane - plane normal. Must be normalized!
- point - point to find distance for.

Return:
Shortest distance from point to plane.
*/
float ZC_DistanceFromPointToPlane(ZC_Vec3<float> point_in_plane, ZC_Vec3<float> normal_plane, ZC_Vec3<float> point);