#pragma once

#define ZC_CHAR_BIT 	8
#define ZC_SCHAR_MIN 	-128
#define ZC_SCHAR_MAX 	127
#define ZC_UCHAR_MAX 	255     //  (0xff)
#define ZC_CHAR_MIN 	-128
#define ZC_CHAR_MAX 	127
#define ZC_SHRT_MIN 	-32768
#define ZC_SHRT_MAX 	32767
#define ZC_USHRT_MAX 	65535   //  (0xffff)
#define ZC_INT_MIN 	    -2147483648
#define ZC_INT_MAX 	    2147483647
#define ZC_UINT_MAX 	4294967295 //   (0xffffffff)
#define ZC_LONG_MIN 	-2147483648
#define ZC_LONG_MAX 	2147483647
#define ZC_ULONG_MAX 	4294967295 //   (0xffffffff)
#define ZC_LLONG_MIN 	-9223372036854775808
#define ZC_LLONG_MAX 	9223372036854775807
#define ZC_ULLONG_MAX 	18446744073709551615 // (0xffffffffffffffff)

typedef unsigned long size_t;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

#define ZC_ROUND(val) (val + 0.5)   //  round float or double to integer value
#define ZC_ToZeroOneRange(val) ((val + 1.0) / 2.0)  //  from range [-1,1] to range [0,1]
#define ZC_ToMinusPlusOneRange(val) ((val * 2.0) - 1.0)  //  from range [0,1] to range [-1,1]

/*
Packs color from 3 float channels into one uint[32] indices -> [0-1] nothing, [2-11] red, [12-21] green, [22-31] blue.

Params:
r - red color (range 0.f - 1.f).
g - green color (range 0.f - 1.f).
b - blue color (range 0.f - 1.f).

Return:
On success packed color, otherwise 0.
*/
constexpr uint ZC_PackColorFloatToUInt(float r, float g, float b) noexcept
{
    if (r < 0.f || r > 1.f || g < 0.f || g > 1.f || b < 0.f || b > 1.f) return 0;
    return ((uint)(r * 255.f) << 10 | (uint)(g * 255.f)) << 10 | (uint)(b * 255.f);
};

/*
Packs color from 3 float channels into one uint[32] indices -> [0-1] nothing, [2-11] red, [12-21] green, [22-31] blue.

Params:
r - red color (range 0.f - 1.f).
g - green color (range 0.f - 1.f).
b - blue color (range 0.f - 1.f).

Return:
On success packed color, otherwise 0.
*/
constexpr uint ZC_PackColorUCharToUInt(uchar r, uchar g, uchar b) noexcept
{
    return ((static_cast<uint>(r) << 10) | static_cast<uint>(g)) << 10 | static_cast<uint>(b);
};

constexpr ushort ZC_PackTexCoordFloatToUShort(float coord)
{
    return coord * ZC_USHRT_MAX;
}