#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/GUI/ZC_GUI_ButtonBase.h>

#include <iostream>
struct ZC_GUI_ButtonKeyboard : public virtual ZC_GUI_ButtonBase
{
    ZC_ButtonID buttonId;

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

    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ColorsButton& _colorsbutton = {});
    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ColorsButton& _colorsbutton = {});

    bool operator == (ZC_ButtonID _buttonId) const noexcept override;

    bool VIsButtonKeyboard_Obj() override;

    void VStopEventActivity_Obj() override;
    
    bool VKeyboardButtonDown_Obj(float time) override;
    void VKeyboardButtonUp_Obj(float time) override;
};