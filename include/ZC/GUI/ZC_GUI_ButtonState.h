#pragma once

#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/GUI/ZC_GUI_Obj.h>
#include <ZC/Tools/Time/ZC_Clock.h>

struct ZC_GUI_ButtonState : public ZC_GUI_Obj
{
    static inline uint color_default = 0;
    static inline uint color_under_cursor = ZC_PackColorUCharToUInt(30, 30, 30);
    static inline uint color_pressed = ZC_PackColorUCharToUInt(50, 50, 50);

    enum ButtonState
    {
        BS_Pressed,
        BS_Released,
        BS_HoldUntilRelease
    };
    ButtonState bs_mouseButton = BS_Released;   //  mouse button left
    ButtonState bs_keyboardButton = BS_Released;

    ZC_Clock clock; //  uses for ZC_GUI_MB__DoubleCLick, ZC_GUI_MB__MBLPress events

    ZC_GUI_ButtonState(const ZC_GUI_ObjData& od);

        //  uses in method VStopEventActivity_Obj() from ZC_GUI_ButtonMouse and ZC_GUI_ButtonKeyboard
    void StopEventActivity_BS();
};