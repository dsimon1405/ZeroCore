#pragma once

#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/GUI/ZC_GUI_Obj.h>

struct ZC_GUI_ButtonState : public ZC_GUI_Obj
{
    static inline uint color_default = 0;
    static inline uint color_under_cursor = ZC_PackColorUCharToUInt(10, 10, 10);
    static inline uint color_pressed = ZC_PackColorUCharToUInt(50, 50, 50);

    // enum State
    // {
    //     S_Released,
    //     S_FirstDown,
    //     S_Pressed,
    // } state = S_Released;

    ZC_GUI_ButtonState(const ZC_GUI_ObjData& od)
        : ZC_GUI_Obj(od)
    {}
};