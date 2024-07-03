#pragma once

#include "ZC_GUI_ObjData.h"

#include <cassert>

struct ZC_GUI_IconUV
{
        //  counting from bl corner of the texture, start counting from 0.
    static ZC_GUI_UV GetUV(float cell_x, float cell_y)
    {
        static const float tex_length = 512.f;
        static const float icon_border = 1.f;      //  icon's border thickness (border pixel)
        static const float icon_length = 126.f;
        static const float icon_with_border = icon_length + (icon_border * 2);
        static const float cells_in_row = tex_length / icon_with_border;    //  same as cells in column

        assert(cell_x < cells_in_row || cell_y < cells_in_row);       //  out of texture range

        float bl_x = (cell_x * icon_with_border) + icon_border;
        float bl_y = (cell_y * icon_with_border) + icon_border;
        return ZC_GUI_UV{ .bl{ bl_x / tex_length, bl_y / tex_length },
            .tr{ (bl_x + icon_length) / tex_length, (bl_y + icon_length) / tex_length } };
    }

    static inline ZC_GUI_UV window   = GetUV(0, 3);
    static inline ZC_GUI_UV button   = GetUV(1, 3);
    static inline ZC_GUI_UV checkBox = GetUV(2, 3);
};