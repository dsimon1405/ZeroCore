#pragma once

#include <ZC/GUI/ZC_GUI_ButtonBase.h>

#include <iostream>
struct ZC_GUI_ButtonMouse : public virtual ZC_GUI_ButtonBase
{
    static inline long doubleClickLimit_nanosec = 300000000;   //  uses as nanosec limit in lmb: ZC_GUI_BF_M__DoubleCLick, ZC_GUI_MB__MBLPress events evetns

        //  may be override
    void VScroll_Obj(float vertical, float time) override
    // {}
    {std::cout<<"mouse scroll"<<std::endl;}
    void VCursorMove_Obj(float rel_x, float rel_y) override
    // {}
    {std::cout<<"mouse move"<<std::endl;}
    virtual void VLeftButtonDown_BM(float time)
    // {}
    {std::cout<<"mouse left down"<<std::endl;}
    virtual void VLeftButtonDoubleClick_BM(float time)
    // {}
    {std::cout<<"mouse double click"<<std::endl;}
    virtual void VLeftButtonPressed_BM(float time)
    // {}
    {std::cout<<"mouse pressed"<<std::endl;}
    virtual void VLeftButtonUp_BM(float time)
    // {}
    {std::cout<<"mouse left up"<<std::endl;}

    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ColorsButton& _colorsbutton = {});
    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, const ZC_GUI_UV& uv, const ColorsButton& _colorsbutton = {});

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