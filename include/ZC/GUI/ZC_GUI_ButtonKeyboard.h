#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/GUI/ZC_GUI_ButtonState.h>

#include <iostream>
struct ZC_GUI_ButtonKeyboard : public virtual ZC_GUI_ButtonState
{
    ZC_ButtonID buttonId;
    const bool usePress;
    static inline long waitPressNanoseconds = 300000000;   //  how long wait to start use VKeyboardButtonPressed_BK(), instead VKeyboardButtonDown_BK()

        //  may be override
    virtual void VKeyboardButtonDown_BK(float time)
    // {}
    {std::cout<<"key down"<<std::endl;}
    virtual void VKeyboardButtonUp_BK(float time)
    // {}
    {std::cout<<"key up"<<std::endl;}
    virtual void VKeyboardButtonPressed_BK(float time)
    // {}
    {std::cout<<"key pressed"<<std::endl;}

    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, bool _pressOnDown);
    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, bool _pressOnDown, const ZC_GUI_UV &uv);

    bool operator == (ZC_ButtonID _buttonId) const noexcept override;

        //  _waitPressNanoseconds - how long wait to start use VKeyboardButtonPressed_BK(), instead VKeyboardButtonDown_BK(), default 300000000 nanoseconds (uses for all object ZC_GUI_ButtonKeyboard)
    static void SetWaitPressTime(long _waitPressNanoseconds);

    bool VIsButtonKeyboard_Obj() override;

    void VStopEventActivity_Obj() override;
    
    bool VKeyboardButtonDown_Obj(float time) override;
    void VKeyboardButtonUp_Obj(float time) override;
};