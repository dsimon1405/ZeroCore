#pragma once

//  Buttons of keyboard and mouse.
enum ZC_ButtonID
{
    K_UNKNOWN = 0,

    K_A = 4,
    K_B = 5,
    K_C = 6,
    K_D = 7,
    K_E = 8,
    K_F = 9,
    K_G = 10,
    K_H = 11,
    K_I = 12,
    K_J = 13,
    K_K = 14,
    K_L = 15,
    K_M = 16,
    K_N = 17,
    K_O = 18,
    K_P = 19,
    K_Q = 20,
    K_R = 21,
    K_S = 22,
    K_T = 23,
    K_U = 24,
    K_V = 25,
    K_W = 26,
    K_X = 27,
    K_Y = 28,
    K_Z = 29,

    K_1 = 30,
    K_2 = 31,
    K_3 = 32,
    K_4 = 33,
    K_5 = 34,
    K_6 = 35,
    K_7 = 36,
    K_8 = 37,
    K_9 = 38,
    K_0 = 39,

    K_RETURN = 40,
    K_ESCAPE = 41,
    K_BACKSPACE = 42,
    K_TAB = 43,
    K_SPACE = 44,

    K_MINUS = 45,
    K_EQUALS = 46,
    K_LEFTBRACKET = 47,
    K_RIGHTBRACKET = 48,
    K_BACKSLASH = 49, /**< Located at the lower left of the return
    K                  *   key on ISO keyboards and at the right end
    K                  *   of the QWERTY row on ANSI keyboards.
    K                  *   Produces REVERSE SOLIDUS (backslash) and
    K                  *   VERTICAL LINE in a US layout, REVERSE
    K                  *   SOLIDUS and VERTICAL LINE in a UK Mac
    K                  *   layout, NUMBER SIGN and TILDE in a UK
    K                  *   Windows layout, DOLLAR SIGN and POUND SIGN
    K                  *   in a Swiss German layout, NUMBER SIGN and
    K                  *   APOSTROPHE in a German layout, GRAVE
    K                  *   ACCENT and POUND SIGN in a French Mac
    K                  *   layout, and ASTERISK and MICRO SIGN in a
    K                  *   French Windows layout.
    K                  */
    K_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
    K                  *   instead of 49 for the same key, but all
    K                  *   OSes I've seen treat the two codes
    K                  *   identically. So, as an implementor, unless
    K                  *   your keyboard generates both of those
    K                  *   codes and your OS treats them differently,
    K                  *   you should generate SDL_SCANCODE_BACKSLASH
    K                  *   instead of this code. As a user, you
    K                  *   should not rely on this code because SDL
    K                  *   will never generate it with most (all?)
    K                  *   keyboards.
    K                  */
    K_SEMICOLON = 51,
    K_APOSTROPHE = 52,
    K_GRAVE = 53, /**< Located in the top left corner (on both ANSI
    K              *   and ISO keyboards). Produces GRAVE ACCENT and
    K              *   TILDE in a US Windows layout and in US and UK
    K              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
    K              *   and NOT SIGN in a UK Windows layout, SECTION
    K              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
    K              *   layouts on ISO keyboards, SECTION SIGN and
    K              *   DEGREE SIGN in a Swiss German layout (Mac:
    K              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
    K              *   DEGREE SIGN in a German layout (Mac: only on
    K              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
    K              *   French Windows layout, COMMERCIAL AT and
    K              *   NUMBER SIGN in a French Mac layout on ISO
    K              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
    K              *   SIGN in a Swiss German, German, or French Mac
    K              *   layout on ANSI keyboards.
    K              */
    K_COMMA = 54,
    K_PERIOD = 55,
    K_SLASH = 56,

    K_CAPSLOCK = 57,

    K_F1 = 58,
    K_F2 = 59,
    K_F3 = 60,
    K_F4 = 61,
    K_F5 = 62,
    K_F6 = 63,
    K_F7 = 64,
    K_F8 = 65,
    K_F9 = 66,
    K_F10 = 67,
    K_F11 = 68,
    K_F12 = 69,

    K_PRINTSCREEN = 70,
    K_SCROLLLOCK = 71,
    K_PAUSE = 72,
    K_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                       does send code 73, not 117) */
    K_HOME = 74,
    K_PAGEUP = 75,
    K_DELETE = 76,
    K_END = 77,
    K_PAGEDOWN = 78,
    K_RIGHT = 79,
    K_LEFT = 80,
    K_DOWN = 81,
    K_UP = 82,

    K_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                         */
    K_KP_DIVIDE = 84,
    K_KP_MULTIPLY = 85,
    K_KP_MINUS = 86,
    K_KP_PLUS = 87,
    K_KP_ENTER = 88,
    K_KP_1 = 89,
    K_KP_2 = 90,
    K_KP_3 = 91,
    K_KP_4 = 92,
    K_KP_5 = 93,
    K_KP_6 = 94,
    K_KP_7 = 95,
    K_KP_8 = 96,
    K_KP_9 = 97,
    K_KP_0 = 98,
    K_KP_PERIOD = 99,

    K_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                            *   keyboards have over ANSI ones,
                            *   located between left shift and Y.
                            *   Produces GRAVE ACCENT and TILDE in a
                            *   US or UK Mac layout, REVERSE SOLIDUS
                            *   (backslash) and VERTICAL LINE in a
                            *   US or UK Windows layout, and
                            *   LESS-THAN SIGN and GREATER-THAN SIGN
                            *   in a Swiss German, German, or French
                            *   layout. */
    K_APPLICATION = 101, /**< windows contextual menu, compose */
    K_POWER = 102, /**< The USB document says this is a status flag,
                    *   not a physical key - but some Mac keyboards
                    *   do have a power key. */
    K_KP_EQUALS = 103,
    K_F13 = 104,
    K_F14 = 105,
    K_F15 = 106,
    K_F16 = 107,
    K_F17 = 108,
    K_F18 = 109,
    K_F19 = 110,
    K_F20 = 111,
    K_F21 = 112,
    K_F22 = 113,
    K_F23 = 114,
    K_F24 = 115,
    K_EXECUTE = 116,
    K_HELP = 117,    /**< AL Integrated Help Center */
    K_MENU = 118,    /**< Menu (show menu) */
    K_SELECT = 119,
    K_STOP = 120,    /**< AC Stop */
    K_AGAIN = 121,   /**< AC Redo/Repeat */
    K_UNDO = 122,    /**< AC Undo */
    K_CUT = 123,     /**< AC Cut */
    K_COPY = 124,    /**< AC Copy */
    K_PASTE = 125,   /**< AC Paste */
    K_FIND = 126,    /**< AC Find */
    K_MUTE = 127,
    K_VOLUMEUP = 128,
    K_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     K_LOCKINGCAPSLOCK = 130,  */
/*     K_LOCKINGNUMLOCK = 131, */
/*     K_LOCKINGSCROLLLOCK = 132, */
    K_KP_COMMA = 133,
    K_KP_EQUALSAS400 = 134,

    K_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                 footnotes in USB doc */
    K_INTERNATIONAL2 = 136,
    K_INTERNATIONAL3 = 137, /**< Yen */
    K_INTERNATIONAL4 = 138,
    K_INTERNATIONAL5 = 139,
    K_INTERNATIONAL6 = 140,
    K_INTERNATIONAL7 = 141,
    K_INTERNATIONAL8 = 142,
    K_INTERNATIONAL9 = 143,
    K_LANG1 = 144, /**< Hangul/English toggle */
    K_LANG2 = 145, /**< Hanja conversion */
    K_LANG3 = 146, /**< Katakana */
    K_LANG4 = 147, /**< Hiragana */
    K_LANG5 = 148, /**< Zenkaku/Hankaku */
    K_LANG6 = 149, /**< reserved */
    K_LANG7 = 150, /**< reserved */
    K_LANG8 = 151, /**< reserved */
    K_LANG9 = 152, /**< reserved */

    K_ALTERASE = 153,    /**< Erase-Eaze */
    K_SYSREQ = 154,
    K_CANCEL = 155,      /**< AC Cancel */
    K_CLEAR = 156,
    K_PRIOR = 157,
    K_RETURN2 = 158,
    K_SEPARATOR = 159,
    K_OUT = 160,
    K_OPER = 161,
    K_CLEARAGAIN = 162,
    K_CRSEL = 163,
    K_EXSEL = 164,

    K_KP_00 = 176,
    K_KP_000 = 177,
    K_THOUSANDSSEPARATOR = 178,
    K_DECIMALSEPARATOR = 179,
    K_CURRENCYUNIT = 180,
    K_CURRENCYSUBUNIT = 181,
    K_KP_LEFTPAREN = 182,
    K_KP_RIGHTPAREN = 183,
    K_KP_LEFTBRACE = 184,
    K_KP_RIGHTBRACE = 185,
    K_KP_TAB = 186,
    K_KP_BACKSPACE = 187,
    K_KP_A = 188,
    K_KP_B = 189,
    K_KP_C = 190,
    K_KP_D = 191,
    K_KP_E = 192,
    K_KP_F = 193,
    K_KP_XOR = 194,
    K_KP_POWER = 195,
    K_KP_PERCENT = 196,
    K_KP_LESS = 197,
    K_KP_GREATER = 198,
    K_KP_AMPERSAND = 199,
    K_KP_DBLAMPERSAND = 200,
    K_KP_VERTICALBAR = 201,
    K_KP_DBLVERTICALBAR = 202,
    K_KP_COLON = 203,
    K_KP_HASH = 204,
    K_KP_SPACE = 205,
    K_KP_AT = 206,
    K_KP_EXCLAM = 207,
    K_KP_MEMSTORE = 208,
    K_KP_MEMRECALL = 209,
    K_KP_MEMCLEAR = 210,
    K_KP_MEMADD = 211,
    K_KP_MEMSUBTRACT = 212,
    K_KP_MEMMULTIPLY = 213,
    K_KP_MEMDIVIDE = 214,
    K_KP_PLUSMINUS = 215,
    K_KP_CLEAR = 216,
    K_KP_CLEARENTRY = 217,
    K_KP_BINARY = 218,
    K_KP_OCTAL = 219,
    K_KP_DECIMAL = 220,
    K_KP_HEXADECIMAL = 221,

    K_LCTRL = 224,
    K_LSHIFT = 225,
    K_LALT = 226, /**< alt, option */
    K_LGUI = 227, /**< windows, command (apple), meta */
    K_RCTRL = 228,
    K_RSHIFT = 229,
    K_RALT = 230, /**< alt gr, option */
    K_RGUI = 231, /**< windows, command (apple), meta */

    K_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special SDL_KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *  See https://usb.org/sites/default/files/hut1_2.pdf
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
    /* @{ */

    K_AUDIONEXT = 258,
    K_AUDIOPREV = 259,
    K_AUDIOSTOP = 260,
    K_AUDIOPLAY = 261,
    K_AUDIOMUTE = 262,
    K_MEDIASELECT = 263,
    K_WWW = 264,             /**< AL Internet Browser */
    K_MAIL = 265,
    K_CALCULATOR = 266,      /**< AL Calculator */
    K_COMPUTER = 267,
    K_AC_SEARCH = 268,       /**< AC Search */
    K_AC_HOME = 269,         /**< AC Home */
    K_AC_BACK = 270,         /**< AC Back */
    K_AC_FORWARD = 271,      /**< AC Forward */
    K_AC_STOP = 272,         /**< AC Stop */
    K_AC_REFRESH = 273,      /**< AC Refresh */
    K_AC_BOOKMARKS = 274,    /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    K_BRIGHTNESSDOWN = 275,
    K_BRIGHTNESSUP = 276,
    K_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                switch, video mode switch */
    K_KBDILLUMTOGGLE = 278,
    K_KBDILLUMDOWN = 279,
    K_KBDILLUMUP = 280,
    K_EJECT = 281,
    K_SLEEP = 282,           /**< SC System Sleep */

    K_APP1 = 283,
    K_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    K_AUDIOREWIND = 285,
    K_AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    K_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                           used as a multi-function feature key for selecting
                           a software defined function shown on the bottom left
                           of the display. */
    K_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                            used as a multi-function feature key for selecting
                            a software defined function shown on the bottom right
                            of the display. */
    K_CALL = 289, /**< Used for accepting phone calls. */
    K_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    K_SCANCODES = 512, /**< not a key, just marks the number of scancodes
                           for array bounds */

    //  Mouse
    
    M_LEFT = 513,
    M_MIDLE = 514,
    M_RIGHT = 515,
    M_X1 = 516,
    M_X2 = 517,
};