#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/GUI/ZC_GUI_ButtonState.h>

#include "ZC_GUI_IconUV.h"
struct ZC_GUI_ButtonKeyboard : public ZC_GUI_ButtonState
{
    ZC_ButtonID buttonId;
    const bool usePress;
    static inline long waitPressNanoseconds = 300000000;   //  how long wait to start use VKeyboardButtonPressed_BK(), instead VKeyboardButtonDown_BK()

        //  may be override
    virtual void VKeyboardButtonDown_BK(float time) {}
    virtual void VKeyboardButtonUp_BK(float time) {}
    virtual void VKeyboardButtonPressed_BK(float time) {}

    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, bool _pressOnDown)
        : ZC_GUI_ButtonKeyboard(_buttonId, width, height, _pressOnDown, ZC_GUI_IconUV::button)
    {}

    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, bool _pressOnDown, const ZC_GUI_UV &uv)
        : ZC_GUI_ButtonState(ZC_GUI_ObjData{ .width = width, .height = height, .uv = uv }),
        buttonId(_buttonId),
        usePress(_pressOnDown)
    {}
        //  _waitPressNanoseconds - how long wait to start use VKeyboardButtonPressed_BK(), instead VKeyboardButtonDown_BK(), default 300000000 nanoseconds (uses for all object ZC_GUI_ButtonKeyboard)
    static void SetWaitPressTime(long _waitPressNanoseconds)
    {
        waitPressNanoseconds = _waitPressNanoseconds;
    }

    bool VIsButtonKeyboard_Obj() override
    {
        return true;
    }

    void VStopEventActivity_Obj() override
    {
        this->StopEventActivity_BS();   //  same in ZC_GUI_ButtonMouse
    }
    
    bool VKeyboardButtonDown_Obj(float time) override
    {
        if (this->bs_mouseButton == BS_Pressed) return false;   //  don't do anything while uses another button down event
        if (this->bs_mouseButton == BS_Released)
        {
            if (usePress)
            {
                if (waitPressNanoseconds == 0)  //  no wait time, use VKeyboardButtonPressed_BK() on start
                {
                    VKeyboardButtonPressed_BK(time);
                    return true;
                }
                this->clock.Start();
            }
            VKeyboardButtonDown_BK(time);
        }
        else if (usePress && (waitPressNanoseconds == 0 || (this->clock.Time<ZC_Nanoseconds>() >= waitPressNanoseconds))) VKeyboardButtonPressed_BK(time);
        return true;
    }

    void VKeyboardButtonUp_Obj(float time) override {}

};