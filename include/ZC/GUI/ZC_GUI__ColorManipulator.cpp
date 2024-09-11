#include "ZC_GUI__ColorManipulator.h"

    //  ZC_GUI__CM

ZC_GUI__CM::ZC_GUI__CM(ZC_GUI__ColorManipulator* _pHolder, bool _rgbCallback)
    : ZC_GUI_ColorManipulator(_rgbCallback),
    pHolder(_pHolder)
{}

void ZC_GUI__CM::VColorChanged_CM(float r, float g, float b)
{
    pHolder->VColorChanged(r, g, b);
}

void ZC_GUI__CM::VAlphaChanged_CM(float a)
{
    pHolder->VAlphaChanged(a);
}


    //  ZC_GUI__ColorManipulator

ZC_GUI__ColorManipulator::ZC_GUI__ColorManipulator(bool _rgbCallback)
    : ZC_GUI__Obj<ZC_GUI__CM>(ZC_GUI__CM(this, _rgbCallback))
{}