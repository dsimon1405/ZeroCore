#pragma once

#include <ZC/GUI/ZC_GUI_ButtonState.h>

enum ZC_GUI_MB__Flag
{
    ZC_GUI_MB__None                     = 0,
    ZC_GUI_MB__CursorMoveOnMBLPress     = 1,        //  if the left mouse button is pressed on the button, VCursorMove_Obj() will be called until the button is released
    ZC_GUI_MB__Scroll                   = 1 << 1,   //  if the mouse cursor under the button, VScroll_Obj() will be called until the cursore leave button's area
    ZC_GUI_MB__DoubleCLick              = 1 << 2,   //  mouse left button double click (fast) will call VLeftButtonDoubleClick() on the second click, not VLeftButtonDown_BM()
    ZC_GUI_MB__MBLPress                 = 1 << 3,   //  will call VLeftButtonPressed_BM() event, instead VLeftButtonDown_BM(), if mbl will pressed
};
typedef int ZC_GUI_MB__Flags;


#include <iostream>
struct ZC_GUI_ButtonMouse : public virtual ZC_GUI_ButtonState
{
    const ZC_GUI_MB__Flags mb_flags;

    static inline const long nanosecondLimit = 300000000;   //  uses ass nanosec limit in lmb: ZC_GUI_MB__DoubleCLick, ZC_GUI_MB__MBLPress events evetns

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

    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_MB__Flags _mb_flags);
    ZC_GUI_ButtonMouse(float width, float height, ZC_GUI_MB__Flags _mb_flags, const ZC_GUI_UV& uv);

    bool VIsDrawing_Obj() const noexcept override;

    bool VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept override;
    bool VIsUseScrollEvent_Obj() const noexcept override;

    void VStopEventActivity_Obj() override;

    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
    
    void VCursorCollisionStart_Obj(float time) override;
    void VCursorCollisionEnd_Obj(float time) override;
    
    bool VMouseButtonLeftDown_Obj(float time) override;
    void VMouseButtonLeftUp_Obj(float time) override;
};