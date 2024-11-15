#pragma once

typedef char ch_zc;                 //  char 1 byte [-128, 127]
typedef signed char sch_zc;         //  sigh=ned char 1 byte [-128, 127]
typedef unsigned char uch_zc;       //  unsigned char 1 byte [0, 255]

typedef short sh_zc;                //  short 2 bytes [-32 768, 32 767]
typedef unsigned short ush_zc;      //  unsigned short 2 bytes [0, 65 535]

typedef int i_zc;                   //  int 4 bytes [-2 147 483 648, 2 147 483 647]
typedef unsigned int ui_zc;         //  unsigned int 4 bytes [0, 4 294 967 295]

typedef long long l_zc;             //  long 4 bytes [-9 223 372 036 854 775 808, 9 223 372 036 854 775 807]
typedef unsigned long long ul_zc;   //  unsigned long 4 bytes [0, 18 446 744 073 709 551 615] 

typedef float f_zc;                 //  float 4 bytes (3.4E +/- 38 (seven numbers))
typedef double d_zc;                //  double 8 bytes (1.7E +/- 308 (fifteen numbers))
typedef double ld_zc;               //  long double: unix 16 bytes; win 8 bytes

typedef wchar_t wch_zc;             //  wchat_t: unix 4 bytes [0, 4 294 967 295]; win 2 bytes [0, 65 535]