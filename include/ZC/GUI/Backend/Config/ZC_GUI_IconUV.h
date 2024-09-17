#pragma once

#include <ZC/GUI/Backend/ZC_GUI_ObjData.h>

namespace ZC_GUI_IconUV
{
    const float tex_width = 66.f;
    const float tex_height = 44.f;
    const float border_length = 1.f;       //  icon's border thickness (border pixel)
    const float icon_length = 20.f;      //  withought border
    
        //  counting from bl corner of the texture, start counting from 0.
    ZC_GUI_UV GetUV(float cell_x, float cell_y, float _tex_width = tex_width, float _tex_height = tex_height, float _border_length = border_length, float _icon_length = icon_length);
    
    extern ZC_GUI_UV quad;
    extern ZC_GUI_UV button;
    extern ZC_GUI_UV arrow_down;
    extern ZC_GUI_UV arrow_right;
    extern ZC_GUI_UV quad_colored;
    extern ZC_GUI_UV background_alpha;
};