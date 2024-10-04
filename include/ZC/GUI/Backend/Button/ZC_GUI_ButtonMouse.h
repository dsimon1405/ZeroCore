#pragma once

#include "ZC_GUI_ButtonBase.h"

struct ZC_GUI_ButtonMouse : public virtual ZC_GUI_ButtonBase
{
    static inline long doubleClickLimit_nanosec = 300000000;   //  uses as nanosec limit in lmb: ZC_GUI_BF_M__DoubleCLick, ZC_GUI_MB__MBLPress events evetns

        //  may be override
    // void VScroll_Obj(float vertical, float time) override {}
    // void VCursorMove_Obj(float rel_x, float rel_y) override {}
    virtual void VLeftButtonDown_BM(float time) {}
    virtual void VLeftButtonDoubleClick_BM(float time) {}
    virtual void VLeftButtonPressed_BM(float time) {}
    virtual void VLeftButtonUp_BM(float time) {}
    virtual void VFocuseChanged_BM(bool isFocused) {}

    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ColorsButton& _colorsButton = {});
    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ColorsButton& _colorsButton = {});

    /*
    Sets nanosecond limit wich desides to call VLeftButtonDown_BM() or VLeftButtonDoubleClick_BM(), counting time from previous click. Default 300000000 nanoseconds.

    Params:
    - _doubleClickLimit_nanosec - must be more then 0.
    */
    static void SetDoubleClickLimit_BM(long _doubleClickLimit_nanosec);

    bool VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept override;
    bool VIsUseScrollEvent_Obj() const noexcept override;

    void VStopEventActivity_Obj() override;

    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
    
    void VCursorCollisionStart_Obj(float time) override;
    void VCursorCollisionEnd_Obj(float time) override;
    
    bool VMouseButtonLeftDown_Obj(float time) override;
    void VMouseButtonLeftUp_Obj(float time) override;
};