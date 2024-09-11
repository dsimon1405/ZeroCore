#pragma once

#include "ZC_GUI__Obj.h"
#include <ZC/GUI/Backend/ZC_GUI_ColorManipulator.h>

class ZC_GUI__ColorManipulator;

struct ZC_GUI__CM : public ZC_GUI_ColorManipulator
{
    ZC_GUI__ColorManipulator* pHolder;

    ZC_GUI__CM(ZC_GUI__ColorManipulator* _pHolder, bool _rgbCallback);

    void VColorChanged_CM(float r, float g, float b) override;
    void VAlphaChanged_CM(float a) override;
};

/*
Class for creation color.
Must be overrid:
- void VColorChanged(float r, float g, float b) -> Calls on color rgb change.
- void VAlphaChanged(float a) -> Calls on alpha chanel change.
*/
class ZC_GUI__ColorManipulator : public ZC_GUI__Obj<ZC_GUI__CM>
{
public:
        //  - rgbCallback - if true, VColorChanged() will calls with values in range [0, 255], otherwise in range [0, 1]. Haven't influence to users input type mode.
    ZC_GUI__ColorManipulator(bool _rgbCallback);

    virtual void VColorChanged(float r, float g, float b) {}
    virtual void VAlphaChanged(float a) {}
};