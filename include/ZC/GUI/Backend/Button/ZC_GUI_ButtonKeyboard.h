#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include "ZC_GUI_ButtonBase.h"

struct ZC_GUI_ButtonKeyboard : public virtual ZC_GUI_ButtonBase
{
    ZC_ButtonID buttonId;

        //  may be override
    virtual void VKeyboardButtonDown_BK(float time) {}
    virtual void VKeyboardButtonUp_BK(float time) {}
    virtual void VKeyboardButtonPressed_BK(float time) {}

    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ColorsButton& _colorsButton = {});
    ZC_GUI_ButtonKeyboard(ZC_ButtonID _buttonId, float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ColorsButton& _colorsButton = {});

    bool operator == (ZC_ButtonID _buttonId) const noexcept override;

    bool VIsButtonKeyboard_Obj() override;
    void VStopEventActivity_Obj() override;
    
    bool VKeyboardButtonDown_Obj(float time) override;
    void VKeyboardButtonUp_Obj(float time) override;
};