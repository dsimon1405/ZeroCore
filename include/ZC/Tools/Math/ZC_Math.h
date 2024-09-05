#pragma once

#include "ZC_Vec2.h"
#include "ZC_Vec3.h"

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

#define ZC_ROUND(val) (val + 0.5)   //  round float or double to integer value
#define ZC_ToZeroOneRange(val) ((val + 1.0) / 2.0)  //  from range [-1,1] to range [0,1]
#define ZC_ToMinusPlusOneRange(val) ((val * 2.0) - 1.0)  //  from range [0,1] to range [-1,1]

/*
Packs color from 3 float channels into one uint[32] indices -> [0-1] nothing, [2-11] red, [12-21] green, [22-31] blue.

Params:
- r - red color (range 0.f - 1.f).
- g - green color (range 0.f - 1.f).
- b - blue color (range 0.f - 1.f).

Return:
On success packed color, otherwise 0.
*/
uint ZC_PackColorFloatToUInt_RGB(float r, float g, float b) noexcept;

uint ZC_PackColorFloatToUInt_RGBA(float r, float g, float b, float a) noexcept;

/*
Packs color from 3 float channels into one uint[32] indices -> [0-1] nothing, [2-11] red, [12-21] green, [22-31] blue.

Params:
- r - red color (range 0.f - 1.f).
- g - green color (range 0.f - 1.f).
- b - blue color (range 0.f - 1.f).

Return:
On success packed color, otherwise 0.
*/
uint ZC_PackColorUCharToUInt_RGB(uchar r, uchar g, uchar b) noexcept;

uint ZC_PackColorUcharToUInt_RGBA(uchar r, uchar g, uchar b, uchar a) noexcept;

ushort ZC_PackTexCoordFloatToUShort(float coord);

int ZC_Pack_INT_2_10_10_10_REV(float x, float y, float z);

/*
Params:
- a, b, c - triangle points coords.
- p - point to check.

Return:
true if point belong triangle, otherwise false.
*/
bool ZC_IsPointBelongTriangle_2D(const ZC_Vec2<float>& a, const ZC_Vec2<float>& b, const ZC_Vec2<float>& c, const ZC_Vec2<float>& p);

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