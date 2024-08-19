#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouse.h>

#include <ZC/GUI/Text/ZC_GUI_TextManager.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>
#include <ZC/GUI/ZC_GUI_IconUV.h>

struct ZC_GUI_ButtonColor : public ZC_GUI_ButtonMouse
{       //  cursors in Color and Saturation quads
    struct Cursor : public ZC_GUI_Obj
    {
        static inline float size = 4.f;
        Cursor()
            : ZC_GUI_Obj(ZC_GUI_ObjData(size, size, 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons))
        {}
    };

    struct Saturation
    {

    };

    enum ButtonsLocation
    {
        Right,
        Down
    };

    ZC_GUI_ButtonColor(int width, int height, ButtonsLocation buttonsLocation)
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData(CalculateWidth(), CalculateWidth(), 0, {}, ZC_GUI_Bindings::bind_quad_colored), ZC_GUI_BF__MBLPress),
        ZC_GUI_ButtonMouse(0.f, 0.f, 0)
    {
        this->VAddObj_Obj(&cursor_colored, nullptr);
    }

    static inline float distance_x;
    static inline float distance_y;

    Cursor cursor_colored;

    float CalculateQuadSize(int width, int height, ButtonsLocation buttonsLocation)
    {
        if (buttonsLocation == Right)
        {
            int min_size = ZC_GUI_TextManager::GetFontHeight() * 4;     //  4 is rgba
            if (height < min_size) height = min_size;
            if (width < min_size) width = min_size;
            distance_y = (height - min_size) / 5;
            distance_x = 
        }

        static float quadSize = 0.f;
        if (quadSize == 0.f)
        {
            distance_x = ZC_GUI_TextManager::GetFontHeight();
            distance_y = ZC_GUI_TextManager::GetFontHeight() / 2.f;
            quadSize = (distance_x * 3.f) + (distance_y * 4.f);     //  3 is count of manipulating buttons; 4 is distances between buttons (2 between buttons and 2 for indent from top and button)
        }
        return quadSize;
    }
};