#pragma once

namespace ZC_GUI_Bindings
{
    const unsigned int bind_Border = 0;
    const unsigned int bind_BL = 1;
    const unsigned int bind_ObjData = 2;
    
    const unsigned int bind_tex_Icons = 0;
    const unsigned int bind_tex_Text = 1;
    const unsigned int bind_quad_colored = 10;    //  it's not real binding, that value manages in gs shader to set color rgbw colors for each corner of quad, and don't using texture (ZC_GUI_ButtonColor::ZC_GUI_ButtonColor())
    const unsigned int bind_quad_black_white = 11;    //  it's not real binding, that value manages in gs shader to set color black and white colors for each corner of quad, and don't using texture
}