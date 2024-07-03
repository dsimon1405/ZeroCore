#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/GUI/ZC_GUI_ButtonState.h>

#include "ZC_GUI_IconUV.h"
struct ZC_GUI_ButtonKeyboard : public ZC_GUI_ButtonState
{
    ZC_ButtonID buttonId;

    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, bool _pressOnDown)
        : ZC_GUI_ButtonKeyboard(_buttonId, width, height, _pressOnDown, ZC_GUI_IconUV::button)
    {}

    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, bool _pressOnDown, const ZC_GUI_UV &uv)
        : ZC_GUI_ButtonState(ZC_GUI_ObjData{ .width = width, .height = height, .uv = uv }),
        buttonId(_buttonId)
    {}

    bool VIsButtonKeyboard_Obj() override
    {
        return true;
    }
    
    bool ButtonDown(float time)
    {
        
        VKeyboardButtonDown_Obj(time);
    }
    
    virtual void VKeyboardButtonDown_Obj(float time) {}
    virtual void VKeyboardButtonUp_Obj(float time) {}
    virtual void VKeyboardButtonPressed_Obj(float time) {}
};