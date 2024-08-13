#pragma once

#include "ZC_GUI_ObjData.h"

namespace ZC_GUI_IconUV
{
    const float tex_length = 44.f;
    const float icon_border = 1.f;       //  icon's border thickness (border pixel)
    const float icon_length = 20.f;      //  withought border
    
        //  counting from bl corner of the texture, start counting from 0.
    ZC_GUI_UV GetUV(float cell_x, float cell_y, float _tex_length = tex_length, float _icon_border = icon_border, float _icon_length = icon_length);
    
    extern ZC_GUI_UV quad;
    extern ZC_GUI_UV button;
    extern ZC_GUI_UV arrowDown;
    extern ZC_GUI_UV arrowRight;
};