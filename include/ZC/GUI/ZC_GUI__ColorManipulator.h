#pragma once

#include "ZC_GUI__Obj.h"
#include <ZC/GUI/Backend/ZC_GUI_ColorManipulator.h>

//  Class for creation color.
class ZC_GUI__ColorManipulator : public ZC_GUI__Obj<ZC_GUI_ColorManipulator>
{
public:
    /*
    Params:
    - _callback - calls if one of value changed. Params: red, green, blue, alpha. Colors red, gree, blue may be in range [0, 255] or [0, 1], look next parameter. Alpha always in range [0, 1].
    - range_255 - if true, callback r,g,b params will have values in range [0, 255], otherwise in range [0, 1]. No metter in wich range data was input from gui.
    */
    ZC_GUI__ColorManipulator(ZC_Function<void(float,float,float,float)>&& _callback, bool _range_255);

    /*
    Set new colors. Type Trgb must be explicitly specified. If Trgb is 'uchar' expected value in range [0, 255], if 'float' expected value in range [0, 1].
    Channel wich value must not be changed must be nullpt.

    Params:
    - pRed - pointer on red channel value.  
    - pGreen - pointer on green channel value.  
    - pBlue - pointer on blue channel value.  
    - use_callback - if true will use callback setted in constructor.
    */
    template <ZC_cGUI_UCharOrFloat Trgb>
    void SetColor(Trgb* pRed, Trgb* pGreen, Trgb* pBlue, bool use_callback);

    /*
    Update alpha channel value.

    Params:
    - alpha - new alpha value in range [0, 1].
    - use_callback - if true will use callback setted in constructor.
    */
    void SetAlpha(float alpha, bool use_callback);
};


template <ZC_cGUI_UCharOrFloat Trgb>
void ZC_GUI__ColorManipulator::SetColor(Trgb* pRed, Trgb* pGreen, Trgb* pBlue, bool use_callback)
{
    this->obj.SetColor(pRed, pGreen, pBlue, use_callback);
}