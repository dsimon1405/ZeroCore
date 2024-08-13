#include "ZC_GUI_IconUV.h"

#include <cassert>

namespace ZC_GUI_IconUV
{
    ZC_GUI_UV GetUV(float cell_x, float cell_y, float _tex_length, float _icon_border, float _icon_length)
    {
        static const float icon_with_border = _icon_length + (_icon_border * 2);
        static const float cells_in_row = _tex_length / icon_with_border;    //  same as cells in column

        assert(cell_x < cells_in_row || cell_y < cells_in_row);       //  out of texture range

        float bl_x = (cell_x * icon_with_border) + _icon_border;
        float bl_y = (cell_y * icon_with_border) + _icon_border;
        return ZC_GUI_UV{ .bl{ bl_x / _tex_length, bl_y / _tex_length },
            .tr{ (bl_x + _icon_length) / _tex_length, (bl_y + _icon_length) / _tex_length } };
    }
    
    ZC_GUI_UV quad = GetUV(0, 1);
    ZC_GUI_UV button = GetUV(1, 1);
    ZC_GUI_UV arrowDown = GetUV(0, 0);
    ZC_GUI_UV arrowRight = GetUV(1, 0);
};