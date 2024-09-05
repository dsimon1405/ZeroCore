#include "ZC_GUI_IconUV.h"

#include <cassert>

namespace ZC_GUI_IconUV
{
    ZC_GUI_UV GetUV(float column, float row, float _tex_width, float _tex_height, float _border_length, float _icon_length)
    {
        static const float icon_with_border = _icon_length + (_border_length * 2);
        static const float columns = _tex_width / icon_with_border;    //  same as cells in column
        static const float rows = _tex_height / icon_with_border;    //  same as cells in column

        assert(column < columns && row < rows);       //  out of texture range

        float bl_x = (column * icon_with_border) + _border_length;
        float bl_y = (row * icon_with_border) + _border_length;
        return ZC_GUI_UV{ .bl{ bl_x / _tex_width, bl_y / _tex_height },
            .tr{ (bl_x + _icon_length) / _tex_width, (bl_y + _icon_length) / _tex_height } };
    }
    
    ZC_GUI_UV quad = GetUV(0, 1);
    ZC_GUI_UV button = GetUV(1, 1);
    ZC_GUI_UV arrow_down = GetUV(0, 0);
    ZC_GUI_UV arrow_right = GetUV(1, 0);
    ZC_GUI_UV quad_colored = GetUV(2, 1);       //  ZC_GUI_ColorManipulator
    ZC_GUI_UV background_alpha = GetUV(2, 0);      //  ZC_GUI_ColorManipulator
};