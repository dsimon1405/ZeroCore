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

#define ZC_ROUND(val) (val + 0.5)   //  round float or double to integer value